// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "numbertobytearrayconverter.hpp"

#include "abstractpluginelements/defaultbankglobalmanager.hpp"
#include "byteutility/bytearrayhelper.hpp"
#include "testbedcore/logs/sequencelogformatter.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/testbedglobal.hpp"
#include "testbedcore/utility/pauseandstophelper.hpp"


NumberToByteArrayConverter::NumberToByteArrayConverter(QObject *parent) :
    DefaultSequenceInstanceModule(staticMetaObject.className(),
                                  DefaultBankGlobalManager::Instance(),
                                  parent)
{
}

bool NumberToByteArrayConverter::initParamsFormat(JsonArray &paramsFormat) const
{
    JsonObject endianess;

    RETURN_IF_FALSE(BankJsonFormatter::formatParamSelectRequired(endianessParamName,
                                                                 endianessParamDesc,
                                                                 endianess));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamAddSelectOption(
                                                EndianessType::toString(EndianessType::LsbFirst),
                                                EndianessType::toString(EndianessType::LsbFirst),
                                                endianess));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamAddSelectOption(
                                                EndianessType::toString(EndianessType::MsbFirst),
                                                EndianessType::toString(EndianessType::MsbFirst),
                                                endianess));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultOption(
                                                { EndianessType::toString(EndianessType::LsbFirst) },
                                                endianess));

    paramsFormat.append(endianess);

    JsonObject byteSize;

    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(byteSizeParamName,
                                                                 byteSizeParamDesc,
                                                                 0,
                                                                 0,
                                                                 byteSize));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(
                                                        byteSize,
                                                        Number(0),
                                                        Number(std::numeric_limits<quint8>::max()),
                                                        Number(1)));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueNumber(Number(1), byteSize));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, byteSize));

    paramsFormat.append(byteSize);

    JsonObject powerOfTen;

    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(powerOfTenToApplyBeforeConvertName,
                                                                 powerOfTenToApplyBeforeConvertDesc,
                                                                 0,
                                                                 0,
                                                                 powerOfTen));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(powerOfTen,
                                                                   Number(maxPowerOfTen, 0, false),
                                                                   Number(maxPowerOfTen),
                                                                   Number(1)));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueNumber(Number(0), powerOfTen));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, powerOfTen));

    paramsFormat.append(powerOfTen);

    return true;
}

bool NumberToByteArrayConverter::initInputsFormat(JsonArray &inputsFormat) const
{
    JsonObject inputNumber;
    if (!BankJsonFormatter::formatInOutString(inputNumberName,
                                              inputNumber,
                                              inputNumberDesc))
    {
        return false;
    }

    inputsFormat.append(inputNumber);

    return true;
}

bool NumberToByteArrayConverter::initOutputsFormat(JsonArray &outputsFormat) const
{
    JsonObject ouputByteArray;
    if (!BankJsonFormatter::formatInOutString(outputByteArrayName,
                                              ouputByteArray,
                                              outputByteArrayDesc))
    {
        return false;
    }

    outputsFormat.append(ouputByteArray);

    return true;
}

bool NumberToByteArrayConverter::validateAndSetParameters(const JsonArray &parameters,
                                                          JsonArray &errors)
{
    RETURN_IF_FALSE(DefaultSequenceInstanceModule::validateAndSetParameters(parameters, errors));

    QString error;
    QVector<QString> values;
    if(!BankJsonParser::getArrayStringValue(parameters,
                                            endianessParamName,
                                            values,
                                            &error))
    {
        fillErrorArray(endianessParamName, error, errors);
        return false;
    }

    if(values.length() != 1)
    {
        fillErrorArray(endianessParamName,
                       tr("(TR) One select value has to be chosen (only one)"),
                       errors);
        return false;
    }

    EndianessType::Enum type = EndianessType::parseFromString(values.at(0));

    if(type == EndianessType::Unknown)
    {
        fillErrorArray(endianessParamName,
                       tr("(TR) The endianess is unknown: %1").arg(values.at(0)),
                       errors);
        return false;
    }

    _endianess = type;

    Number tmpValue;
    if(!BankJsonParser::getNumberValue(parameters, byteSizeParamName, tmpValue, &error))
    {
        fillErrorArray(byteSizeParamName, error, errors);
        return false;
    }

    // The above validate method already verify if the number is contained in the quint8
    _byteSize = tmpValue.toUint8();

    if(!BankJsonParser::getNumberValue(parameters,
                                       powerOfTenToApplyBeforeConvertName,
                                       tmpValue,
                                       &error))
    {
        fillErrorArray(powerOfTenToApplyBeforeConvertName, error, errors);
        return false;
    }

    _powerOfTen = tmpValue.toInt8();

    return true;
}

void NumberToByteArrayConverter::processModule(const JsonArray &jsonObjectInputValues)
{
    if(!PauseAndStopHelper::managePauseAndStop(getSequenceState()))
    {
        emit moduleFinished(false, JsonArray());
        return;
    }

    JsonArray output;

    if(!convert(jsonObjectInputValues, output))
    {
        emit newLog(SequenceLogFormatter::formatABusinessProblem(tr("(TR) A problem occured while "
                                                                    "converting the number given "
                                                                    "to byte array")));
        emit moduleFinished(false, {});
        return;
    }

    emit newLog(SequenceLogData(LogMsgType::Info,
                                SequenceLogCategory::BusinessScope,
                                "The number has been converted"));

    emit moduleFinished(true, output);
}

bool NumberToByteArrayConverter::convert(const JsonArray &jsonObjectInputValues,
                                         JsonArray &output)
{
    Number input;
    RETURN_IF_FALSE(BankJsonParser::getNumberValue(jsonObjectInputValues,
                                                   inputNumberName,
                                                   input));

    input.multiplyWithPowerOfTen(_powerOfTen);

    if(!input.isValid())
    {
        qWarning() << "The power of ten:" << _powerOfTen << ", multiplying on the number: " << input
                   << ", failed";
        return false;
    }

    QByteArray result;
    RETURN_IF_FALSE(convertToByteArray(input, result));

    QString arrayStr = ByteArrayHelper::toStringHex(result, true, _byteSize);

    return BankJsonParser::setStringValue(outputByteArrayName, arrayStr, output);
}

bool NumberToByteArrayConverter::convertToByteArray(const Number &value, QByteArray &result)
{
    quint64 rawValue = value.toUint64();

    if(_endianess == EndianessType::Unknown)
    {
        qWarning() << "Can't convert a number to byte array with an unknown endianess";
        return false;
    }

    // We verify if the raw value is over the byte size
    if(_byteSize < EndianessHelper::ByteNbInUint64)
    {
        quint64 max = 0;
        RETURN_IF_FALSE(ByteArrayHelper::calculateMaxUnsignedValue(_byteSize, max));

        if(max < rawValue)
        {
            qWarning() << "The number given: " << value << ", overflows the byte size: "
                       << _byteSize;
            return false;
        }
    }

    result.reserve(_byteSize);

    bool isMsbFirst = (_endianess == EndianessType::MsbFirst);

    for(quint8 idx = 0; idx < _byteSize; ++idx)
    {
        quint8 resultIdx = idx;

        if(isMsbFirst)
        {
            resultIdx = (_byteSize - 1) - idx;
        }

        if(idx >= EndianessHelper::ByteNbInUint64)
        {
            result[resultIdx] = 0x00;
        }
        else
        {
            result[resultIdx] = static_cast<char>(EndianessHelper::getByteFromLsbToMsb(rawValue,
                                                                                       idx));
        }
    }

    return true;
}
