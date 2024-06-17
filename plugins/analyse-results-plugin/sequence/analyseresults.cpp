// SPDX-FileCopyrightText: 2020 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "analyseresults.hpp"

#include "abstractpluginelements/defaultbankglobalmanager.hpp"
#include "bankcoreshared/usebyplugin/icommoninterface.hpp"
#include "bankcoreshared/usebyplugin/isequencelogmanager.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/testbedglobal.hpp"
#include "testbedcore/utility/pauseandstophelper.hpp"


AnalyseResults::AnalyseResults(QObject *parent) :
    DefaultSequenceInstanceModule(staticMetaObject.className(),
                                  DefaultBankGlobalManager::Instance(),
                                  parent)
{
}

AnalyseResults::~AnalyseResults()
{
}

void AnalyseResults::processModule(const JsonArray &jsonObjectInputValues)
{
    if(!PauseAndStopHelper::managePauseAndStop(getSequenceState()))
    {
        emit moduleFinished(false, {});
        return;
    }

    bool sequenceSuccess = false;
    QVector<QString> lastLogs;
    
    if(BankJsonParser::getBoolValue(jsonObjectInputValues, InputSuccessName, sequenceSuccess))
    {
        sequenceSuccess = analyseResults(sequenceSuccess, lastLogs);
    }

    JsonArray outputs;
    
    if(!BankJsonParser::setBoolValue(OutputSuccessName, sequenceSuccess, outputs))
    {
        emit moduleFinished(false, {});
        return;
    }

    if(!BankJsonParser::setArrayStringValue(OutputLastErrorLogsName, lastLogs, outputs))
    {
        emit moduleFinished(false, {});
        return;
    }

    emit moduleFinished(true, outputs);
}

bool AnalyseResults::validateAndSetParameters(const JsonArray &parameters, JsonArray &errors)
{
    RETURN_IF_FALSE(DefaultSequenceInstanceModule::validateAndSetParameters(parameters, errors));

    QString error;

    Number tmpTailErrorLogsNb;
    if(!BankJsonParser::getNumberValue(parameters,
                                        TailErrorLogsNbParamName,
                                       tmpTailErrorLogsNb,
                                       &error))
    {
        fillErrorArray(TailErrorLogsNbParamName, error, errors);
        return false;
    }

    // Because we set in the parameter format the max limit of the Number equals to the max limit of
    // an int32, we don't need to test if the variable overflows the limit
    _tailErrorLogsNb = tmpTailErrorLogsNb.toInt32();

    return true;
}

bool AnalyseResults::analyseResults(bool sequenceSuccess, QVector<QString> &lastLogs)
{
    const ICommonInterface &comItf = DefaultBankGlobalManager::Instance()->getCommonInterface();
    const ISequenceLogManager *logManager = comItf.getSequenceLogManager(getSequenceToken());

    if(!logManager)
    {
        qWarning() << "Cannot find a sequence log manager for the sequence: " << getSequenceToken();
        return false;
    }

    QVector<SequenceLogData> lastErrorLogs;
    RETURN_IF_FALSE(logManager->getAllTheSequenceLogs(lastErrorLogs,
                                                      SequenceLogCategory::BusinessScope |
                                                      SequenceLogCategory::ApplicationScope,
                                                      LogMsgType::Warning));

    if(lastErrorLogs.isEmpty())
    {
        return sequenceSuccess;
    }

    QVector<SequenceLogData>::const_iterator citer = lastErrorLogs.cend();
    int logNb = 0;

    while((citer != lastErrorLogs.cbegin()) &&
          ((_tailErrorLogsNb == -1) || (logNb < _tailErrorLogsNb)))
    {
        --citer;
        logNb++;

        lastLogs.prepend(citer->getText());
    }

    if(sequenceSuccess && !lastLogs.isEmpty())
    {
        sequenceSuccess = false;
    }

    return sequenceSuccess;
}

bool AnalyseResults::initParamsFormat(JsonArray &paramFormat) const
{
    JsonArray parameters;

    JsonObject tailErrorParam;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(TailErrorLogsNbParamName,
                                                                 TailErrorLogsNbParamDesc,
                                                                 0,
                                                                 0,
                                                                 tailErrorParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(
                                                            tailErrorParam,
                                                            Number(1, false),
                                                            Number::maxInt32(),
                                                            Number(1)));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, tailErrorParam));

    parameters.append(tailErrorParam);

    paramFormat = parameters;

    return true;
}

bool AnalyseResults::initInputsFormat(JsonArray &inputFormat) const
{
    JsonArray inputs;

    JsonObject inputSuccess;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutBool(InputSuccessName,
                                                       inputSuccess,
                                                       InputSuccessDesc));

    inputs.append(inputSuccess);

    inputFormat = inputs;

    return true;
}

bool AnalyseResults::initOutputsFormat(JsonArray &outputFormat) const
{
    JsonArray outputs;

    JsonObject outputSuccess;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutBool(OutputSuccessName,
                                                       outputSuccess,
                                                       OutputSuccessDesc));

    outputs.append(outputSuccess);

    JsonObject errorLogs;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutStringArray(OutputLastErrorLogsName,
                                                              errorLogs,
                                                              OutputLastErrorLogsDesc));

    outputs.append(errorLogs);

    outputFormat = outputs;

    return true;
}
