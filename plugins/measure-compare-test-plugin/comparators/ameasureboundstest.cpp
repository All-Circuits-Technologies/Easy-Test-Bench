// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "ameasureboundstest.hpp"

#include "abstractpluginelements/defaultbankglobalmanager.hpp"
#include "testbedcore/logs/sequencelogformatter.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/testbedglobal.hpp"
#include "testbedcore/utility/pauseandstophelper.hpp"


AMeasureBoundsTest::AMeasureBoundsTest(const QString &moduleClassName,
                                       QObject *parent) :
    DefaultSequenceInstanceModule(moduleClassName,
                                  DefaultBankGlobalManager::Instance(),
                                  parent)
{
}

void AMeasureBoundsTest::processModule(const JsonArray &jsonObjectInputValues)
{
    if(!PauseAndStopHelper::managePauseAndStop(getSequenceState()))
    {
        emit moduleFinished(false, {});
        return;
    }

    bool testSuccess = false;
    if(!processTest(jsonObjectInputValues, testSuccess))
    {
        emit newLog(SequenceLogFormatter::formatABusinessProblem(tr("(TR) A problem occured while "
                                                                    "trying to test the measure "
                                                                    "got")));
        emit moduleFinished(false, {});
        return;
    }

    if(!testSuccess)
    {
        emit newLog(SequenceLogFormatter::formatABusinessProblem(tr("(TR) The measure given isn't "
                                                                    "in the expected boundaries")));

        if(_fatalError)
        {
            emit moduleFinished(false, {});
            return;
        }
    }
    else
    {
        emit newLog(SequenceLogData(LogMsgType::Info,
                                    SequenceLogCategory::BusinessScope,
                                    "The measure is included in the expected boundaries"));
    }

    JsonArray output;
    if(!BankJsonParser::setBoolValue(OutputSuccessName,
                                     testSuccess,
                                     output))
    {
        qWarning() << "A problem occured when tryied to set the test result on output";
        emit newLog(SequenceLogFormatter::formatABusinessProblem(tr("(TR) A problem occured when "
                                                                    "tryied to set the test result "
                                                                    "on output")));
        emit moduleFinished(false, {});
        return;
    }

    emit moduleFinished(true, output);
}

bool AMeasureBoundsTest::initParamsFormat(JsonArray &paramsFormat) const
{
    JsonObject elementTested;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(ElementTestedParamName,
                                                                 ElementTestedParamDesc,
                                                                 elementTested));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, elementTested));

    paramsFormat.append(elementTested);

    JsonObject testDescription;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(TestDescriptionParamName,
                                                                 TestDescriptionParamDesc,
                                                                 testDescription));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, testDescription));

    paramsFormat.append(testDescription);

    JsonObject fatalError;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamBoolRequired(FatalErrorParamName,
                                                               FatalErrorParamDesc,
                                                               fatalError));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueBool(DefaultFatalErrorValue,
                                                                   fatalError));

    paramsFormat.append(fatalError);

    JsonObject isInfBoundaryIncluded;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamBoolRequired(InfBoundaryIncludedParamName,
                                                               InfBoundaryIncludedParamDesc,
                                                               isInfBoundaryIncluded));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueBool(DefaultInfBoundaryIncludedValue,
                                                                   isInfBoundaryIncluded));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, isInfBoundaryIncluded));

    paramsFormat.append(isInfBoundaryIncluded);

    JsonObject isSupBoundaryIncluded;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamBoolRequired(SupBoundaryIncludedParamName,
                                                               SupBoundaryIncludedParamDesc,
                                                               isSupBoundaryIncluded));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueBool(DefaultSupBoundaryIncludedValue,
                                                                   isSupBoundaryIncluded));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, isSupBoundaryIncluded));

    paramsFormat.append(isSupBoundaryIncluded);

    return true;
}

bool AMeasureBoundsTest::initInputsFormat(JsonArray &inputsFormat) const
{
    JsonObject measure;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutNumber(InputMeasureDoneName,
                                                         measure,
                                                         InputMeasureDoneDesc));

    inputsFormat.append(measure);

    return true;
}

bool AMeasureBoundsTest::initOutputsFormat(JsonArray &outputsFormat) const
{
    JsonObject successOut;
    if (!BankJsonFormatter::formatInOutBool(OutputSuccessName,
                                            successOut,
                                            OutputSuccessDesc))
    {
        return false;
    }

    outputsFormat.append(successOut);

    return true;
}

bool AMeasureBoundsTest::validateAndSetParameters(const JsonArray &parameters, JsonArray &errors)
{
    RETURN_IF_FALSE(DefaultSequenceInstanceModule::validateAndSetParameters(parameters, errors));

    QString error;
    if(!BankJsonParser::getStringValue(parameters, ElementTestedParamName, _elementTested, &error))
    {
        fillErrorArray(ElementTestedParamName, error, errors);
        return false;
    }

    if(!BankJsonParser::getStringValue(parameters,
                                        TestDescriptionParamName,
                                       _testDescription,
                                       &error,
                                       true))
    {
        fillErrorArray(TestDescriptionParamName, error, errors);
        return false;
    }

    if(!BankJsonParser::getBoolValue(parameters, FatalErrorParamName, _fatalError, &error))
    {
        fillErrorArray(FatalErrorParamName, error, errors);
        return false;
    }

    if(!BankJsonParser::getBoolValue(parameters,
                                     InfBoundaryIncludedParamName,
                                     _isInfBoundaryIncluded,
                                     &error,
                                     true))
    {
        fillErrorArray(InfBoundaryIncludedParamName, error, errors);
        return false;
    }

    if(!BankJsonParser::getBoolValue(parameters,
                                     SupBoundaryIncludedParamName,
                                     _isSupBoundaryIncluded,
                                     &error,
                                     true))
    {
        fillErrorArray(SupBoundaryIncludedParamName, error, errors);
        return false;
    }

    return true;
}

bool AMeasureBoundsTest::compare(const JsonArray &jsonObjectInputValues,
                                 const Number &infBound,
                                 const Number &supBound,
                                 bool &testSuccess)
{
    Number measure;
    RETURN_IF_FALSE(BankJsonParser::getNumberValue(jsonObjectInputValues,
                                                   InputMeasureDoneName,
                                                   measure));

    if(!measure.isValid())
    {
        qWarning() << "The measure given isn't valid, can't test it";
        return false;
    }

    testSuccess = (_isInfBoundaryIncluded ? (infBound <= measure) : (infBound < measure)) &&
                  (_isSupBoundaryIncluded ? (measure <= supBound) : (measure < supBound));

    emit newLog(SequenceLogFormatter::formatTest(testSuccess,
                                                 _elementTested,
                                                 measure,
                                                 infBound,
                                                 supBound,
                                                 _testDescription));

    return true;
}
