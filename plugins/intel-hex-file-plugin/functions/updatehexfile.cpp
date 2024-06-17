// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "updatehexfile.hpp"

#include "byteutility/bytearrayhelper.hpp"
#include "testbedcore/logs/sequencelogformatter.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/testbedglobal.hpp"
#include "testbedcore/utility/pauseandstophelper.hpp"


UpdateHexFile::UpdateHexFile(QObject *parent) :
    AUpdateHexFile(staticMetaObject.className(), parent)
{
}

bool UpdateHexFile::initInputsFormat(JsonArray &inputsFormat) const
{
    RETURN_IF_FALSE(AUpdateHexFile::initInputsFormat(inputsFormat));

    JsonObject dataInput;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(InputDataValueName,
                                                         dataInput,
                                                         InputDataValueDesc));

    inputsFormat.append(dataInput);

    return true;
}

bool UpdateHexFile::initParamsFormat(JsonArray &paramsFormat) const
{
    JsonObject byteAddress;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(AddressParamName,
                                                                 AddressParamDesc,
                                                                 0,
                                                                 0,
                                                                 byteAddress));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(byteAddress,
                                                                   Number(0),
                                                                   Number::maxUint16(),
                                                                   Number(1)));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, byteAddress));

    paramsFormat.append(byteAddress);

    return true;
}

void UpdateHexFile::processModule(const JsonArray &jsonObjectInputValues)
{
    if(!PauseAndStopHelper::managePauseAndStop(getSequenceState()))
    {
        emit moduleFinished(false, {});
        return;
    }

    bool success = processUpdate(jsonObjectInputValues);

    if(!success)
    {
        emit newLog(SequenceLogFormatter::formatABusinessProblem(tr("(TR) A problem occured while "
                                                                    "writing hex value in the "
                                                                    "intel hex file")));
    }
    else
    {

        emit newLog(SequenceLogData(LogMsgType::Info,
                                    SequenceLogCategory::BusinessScope,
                                    "The intel hex file has been updated"));
    }

    emit moduleFinished(success, JsonArray());
}

bool UpdateHexFile::validateAndSetParameters(const JsonArray &parameters, JsonArray &errors)
{
    RETURN_IF_FALSE(AUpdateHexFile::validateAndSetParameters(parameters, errors));
    QString error;

    Number value;
    if(!BankJsonParser::getNumberValue(parameters, AddressParamName, value, &error))
    {
        fillErrorArray(AddressParamName, error, errors);
        return false;
    }

    // The number limits are tests in the validate module parameters
    _address = value.toUint16();

    return true;
}

bool UpdateHexFile::processUpdate(const JsonArray &jsonObjectInputValues)
{
    QString dataValue;
    RETURN_IF_FALSE(BankJsonParser::getStringValue(jsonObjectInputValues,
                                                   InputDataValueName,
                                                   dataValue));

    QByteArray value;
    RETURN_IF_FALSE(ByteArrayHelper::parseFromHex(dataValue, value));

    return updateHexProcess(jsonObjectInputValues, _address, value);
}
