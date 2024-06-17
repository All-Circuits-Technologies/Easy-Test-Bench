// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "comparemeasurefromscriptlogs.hpp"

#include "serial-link-lib/serialpluginlibconstants.hpp"

#include "testbedcore/logs/sequencelogformatter.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"

#include "managers/seriallinkbankglobalmanager.hpp"


CompareMeasureFromScriptLogs::CompareMeasureFromScriptLogs(QObject *parent)
    : ACallScriptViaSerialLink{staticMetaObject.className(),
                               true,
                               SerialLinkBankGlobalManager::Instance(),
                               parent}
{
}

CompareMeasureFromScriptLogs::~CompareMeasureFromScriptLogs()
{
}

bool CompareMeasureFromScriptLogs::initParamsFormat(JsonArray &paramsFormat) const
{
    RETURN_IF_FALSE(ACallScriptViaSerialLink::initParamsFormat(paramsFormat));

    JsonObject measurePatternObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(
        MeasurePatternParamName,
        SerialPluginLibConstants::Json::OneCapturingRegExpDesc,
        measurePatternObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamPatternRegex(
        SerialPluginLibConstants::Defaults::OneCapturingParamRegExp,
        measurePatternObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, measurePatternObj));

    paramsFormat.append(measurePatternObj);

    JsonObject infToleranceObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(InfToleranceBoundParamName,
                                                                 InfToleranceBoundParamDesc,
                                                                 0,
                                                                 0,
                                                                 infToleranceObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, infToleranceObj));

    paramsFormat.append(infToleranceObj);

    JsonObject supToleranceObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(SupToleranceBoundParamName,
                                                                 SupToleranceBoundParamDesc,
                                                                 0,
                                                                 0,
                                                                 supToleranceObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, supToleranceObj));

    paramsFormat.append(supToleranceObj);

    return true;
}

bool CompareMeasureFromScriptLogs::validateAndSetParameters(const JsonArray &parameters,
                                                            JsonArray &errors)
{
    RETURN_IF_FALSE(ACallScriptViaSerialLink::validateAndSetParameters(parameters, errors));

    QString error;

    Number infTolerance;
    if(!BankJsonParser::getNumberValue(parameters,
                                       InfToleranceBoundParamName,
                                       infTolerance,
                                       &error))
    {
        fillErrorArray(InfToleranceBoundParamName, error, errors);
        return false;
    }

    Number supTolerance;
    if(!BankJsonParser::getNumberValue(parameters,
                                       SupToleranceBoundParamName,
                                       supTolerance,
                                       &error))
    {
        fillErrorArray(SupToleranceBoundParamName, error, errors);
        return false;
    }

    if(infTolerance >= supTolerance)
    {
        fillErrorArray(SupToleranceBoundParamName,
                       QString("The inferior tolerance: %1, can't superior or equal to the "
                               "superior tolerance: %2")
                           .arg(infTolerance.toString(), supTolerance.toString()),
                       errors);
        qWarning() << "The inferior tolerance: " << infTolerance << ", can't superior or equal to "
                   << "the superior tolerance: " << supTolerance;
        return false;
    }

    _infTolerance = infTolerance;
    _supTolerance = supTolerance;

    QString measurePattern;
    if(!BankJsonParser::getStringValue(parameters,
                                       MeasurePatternParamName,
                                       measurePattern,
                                       &error))
    {
        fillErrorArray(MeasurePatternParamName, error, errors);
        return false;
    }

    if(!setExpectedPattern(measurePattern))
    {
        fillErrorArray(MeasurePatternParamName,
                       QString("A problem occurred when tried to set the expected param: %1")
                           .arg(measurePattern),
                       errors);
        return false;
    }

    return true;
}

bool CompareMeasureFromScriptLogs::manageCapturedElements(quint8 errorCode,
                                                          const QVector<QString> &capturedElements)
{
    const bool hasCapturedMeasure = capturedElements.length() == ExpectedCapturedListLength;
    const Number measuredValue = hasCapturedMeasure ?
                                     Number::fromString(
                                        capturedElements.at(MeasuredIdxInCapturedList)) :
                                     Number();

    if(errorCode != 0)
    {
        if(measuredValue.isValid())
        {
            emit newLog({ LogMsgType::Info,
                         SequenceLogCategory::BusinessScope,
                         QString("The value measured is equal to: %1")
                             .arg(measuredValue.toString()) });
        }
        
        return !isEmittingErrorIfFailed();
    }

    if(!measuredValue.isValid())
    {
        qWarning() << "A problem occurred in the compare measure value from script logs, there is "
                   << "no captured value or it can't be parsed: " << capturedElements;
        emit newLog(SequenceLogFormatter::formatAnApplicationProblem("The script hasn't returned "
                                                                     "the value measured, or it's "
                                                                     "not a number. Check the "
                                                                     "RegExp given to the module "
                                                                     "or the script."));
        return false;
    }

    bool testSuccess = _infTolerance <= measuredValue && measuredValue <= _supTolerance;

    emit newLog(SequenceLogFormatter::formatTest(testSuccess,
                                                 getTestedElement(),
                                                 measuredValue,
                                                 _infTolerance,
                                                 _supTolerance,
                                                 getTestDescription()));
    
    return testSuccess || !isEmittingErrorIfFailed();
}
