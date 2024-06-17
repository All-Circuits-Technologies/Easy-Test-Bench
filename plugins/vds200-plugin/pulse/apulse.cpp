// SPDX-FileCopyrightText: 2021 - 2022 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "apulse.hpp"

#include "abstractpluginelements/defaultbankglobalmanager.hpp"
#include "emtestbenchutility/writeprocess.hpp"
#include "emtestbenchutility/command/startcmd.hpp"
#include "emtestbenchutility/command/terminatecmd.hpp"
#include "emtestbenchutility/response/aresponse.hpp"
#include "emtestbenchutility/response/backresponse.hpp"
#include "emtestbenchutility/response/idresponse.hpp"
#include "testbedcore/logs/sequencelogdata.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/testbedglobal.hpp"
#include "testbedcore/utility/pauseandstophelper.hpp"

#include "vds200-plugin/idncmd.hpp"
#include "vds200-plugin/pulse/triggerenum.hpp"
#include "vds200-plugin/pulseCmd/apulsecmd.hpp"


APulse::APulse(const QVector<PulseFlag::Flag> &parametersFlag,
               const QString &moduleClassName,
               const QString &pulseName,
               QObject *parent) :
    DefaultSequenceInstanceModule(moduleClassName, DefaultBankGlobalManager::Instance(), parent),
    _paramsFlag(PulseFlag::initParametersFlag(parametersFlag)),
    _currentPulseName(pulseName)
{
}

APulse::~APulse()
{
    if(!terminatePulse())
    {
        qWarning() << "Unable to terminate simulation";
    }
}

bool APulse::initParamsFormat(JsonArray &paramsFormat) const
{
    if(_paramsFlag != 0)
    {
        RETURN_IF_FALSE(initNbParamsWithTestFlags(PulseFlag::VbFlag,
                                                  APulse::VbParamName,
                                                  APulse::VbParamDesc,
                                                  Number(APulse::VoltStartMinDv, 1),
                                                  Number(APulse::VoltStartMaxDv, 1),
                                                  Number(APulse::VoltStartStepDv, 1),
                                                  paramsFormat));

        RETURN_IF_FALSE(initNbParamsWithTestFlags(PulseFlag::VsFlag,
                                                  APulse::VsParamName,
                                                  APulse::VsParamDesc,
                                                  Number(APulse::VoltLevelMinDv, 1, false),
                                                  Number(APulse::VoltLevelMaxDv, 1),
                                                  Number(APulse::VoltLevelStepDv, 1),
                                                  paramsFormat));

        RETURN_IF_FALSE(initNbParamsWithTestFlags(PulseFlag::VaFlag,
                                                  APulse::VaParamName,
                                                  APulse::VaParamDesc,
                                                  Number(APulse::VoltLevelMinDv, 1, false),
                                                  Number(APulse::VoltLevelMaxDv, 1),
                                                  Number(APulse::VoltLevelStepDv, 1),
                                                  paramsFormat));

        RETURN_IF_FALSE(initNbParamsWithTestFlags(PulseFlag::VpFlag,
                                                  APulse::VpParamName,
                                                  APulse::VpParamDesc,
                                                  Number(APulse::AmplMinDv, 1, false),
                                                  Number(APulse::AmplMaxDv, 1),
                                                  Number(APulse::AmplStepDv, 1),
                                                  paramsFormat));

        RETURN_IF_FALSE(initNbParamsWithTestFlags(PulseFlag::T6Flag,
                                                  APulse::T6ParamName,
                                                  APulse::T6ParamDesc,
                                                  Number(APulse::TimeMinMs),
                                                  Number(APulse::TimeMaxMs),
                                                  Number(APulse::TimeStepMs),
                                                  paramsFormat));

        RETURN_IF_FALSE(initNbParamsWithTestFlags(PulseFlag::T7Flag,
                                                  APulse::T7ParamName,
                                                  APulse::T7ParamDesc,
                                                  Number(APulse::TimeMinMs),
                                                  Number(APulse::TimeMaxMs),
                                                  Number(APulse::TimeStepMs),
                                                  paramsFormat));

        RETURN_IF_FALSE(initNbParamsWithTestFlags(PulseFlag::TfFlag,
                                                  APulse::TfParamName,
                                                  APulse::TfParamDesc,
                                                  Number(APulse::TimeMinMs),
                                                  Number(APulse::TimeMaxMs),
                                                  Number(APulse::TimeStepMs),
                                                  paramsFormat));

        RETURN_IF_FALSE(initNbParamsWithTestFlags(PulseFlag::T8Flag,
                                                  APulse::T8ParamName,
                                                  APulse::T8ParamDesc,
                                                  Number(APulse::TimeMinS, 1),
                                                  Number(APulse::TimeMaxS, 1),
                                                  Number(APulse::TimeStepS, 1),
                                                  paramsFormat));

        RETURN_IF_FALSE(initNbParamsWithTestFlags(PulseFlag::InterFlag,
                                                  APulse::InterruptionParamName,
                                                  APulse::InterruptionParamDesc,
                                                  Number(APulse::TimeMinS, 1),
                                                  Number(APulse::TimeMaxS, 1),
                                                  Number(APulse::TimeStepS, 1),
                                                  paramsFormat));

        RETURN_IF_FALSE(initNbParamsWithTestFlags(PulseFlag::NbCyclesFlag,
                                                  APulse::NbCyclesParamName,
                                                  APulse::NbCyclesParamDesc,
                                                  Number(APulse::NbCycleMin),
                                                  Number(APulse::NbCycleMax),
                                                  Number(APulse::NbCycleStep),
                                                  paramsFormat));

        RETURN_IF_FALSE(initNbParamsWithTestFlags(PulseFlag::CurrentFlag,
                                                  APulse::CurrentParamName,
                                                  APulse::CurrentParamDesc,
                                                  Number(APulse::CurrentMin),
                                                  Number(APulse::CurrentMax),
                                                  Number(APulse::CurrentStep),
                                                  paramsFormat));

        if(_paramsFlag.testFlag(PulseFlag::TriggerFlag))
        {
            JsonObject trigger;

            RETURN_IF_FALSE(BankJsonFormatter::formatParamSelectRequired(APulse::TriggerParamName,
                                                                         APulse::TriggerParamDesc,
                                                                         trigger));

            const QVector<TriggerEnum::Enum> &triggerEnums = TriggerEnum::getAllEnums();
            for(auto citer = triggerEnums.cbegin(); citer != triggerEnums.cend(); ++citer)
            {
                QString triggerStr = TriggerEnum::toString(*citer);
                RETURN_IF_FALSE(BankJsonFormatter::formatParamAddSelectOption(triggerStr,
                                                                              triggerStr,
                                                                              trigger));
            }

            RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, trigger));

            paramsFormat.append(trigger);
        }
    }

    return initNumberParams(APulse::TmoParamName,
                            APulse::TmoParamDesc,
                            Number(APulse::TmoMinMs, 0, false),
                            Number(APulse::TmoMaxMs),
                            Number(APulse::TmoStepMs),
                            paramsFormat);
}

bool APulse::validateAndSetParameters(const JsonArray &parameters, JsonArray &errors)
{
    RETURN_IF_FALSE(DefaultSequenceInstanceModule::validateAndSetParameters(parameters, errors));

    QString error;
    Number tmpValue;

    if(_paramsFlag != 0)
    {
        RETURN_IF_FALSE(validateAndSetFlagParam(parameters,
                                                PulseFlag::VbFlag,
                                                APulse::VbParamName,
                                                &APulseCmd::setVb,
                                                errors,
                                                Number(10)));

        RETURN_IF_FALSE(validateAndSetFlagParam(parameters,
                                                PulseFlag::VsFlag,
                                                APulse::VsParamName,
                                                &APulseCmd::setVs,
                                                errors,
                                                Number(10),
                                                Number(28)));

        RETURN_IF_FALSE(validateAndSetFlagParam(parameters,
                                                PulseFlag::VaFlag,
                                                APulse::VaParamName,
                                                &APulseCmd::setVa,
                                                errors,
                                                Number(10),
                                                Number(28)));

        RETURN_IF_FALSE(validateAndSetFlagParam(parameters,
                                                PulseFlag::VpFlag,
                                                APulse::VpParamName,
                                                &APulseCmd::setVp,
                                                errors,
                                                Number(10)));

        RETURN_IF_FALSE(validateAndSetFlagParam(parameters,
                                                PulseFlag::T6Flag,
                                                APulse::T6ParamName,
                                                &APulseCmd::setT6,
                                                errors));

        RETURN_IF_FALSE(validateAndSetFlagParam(parameters,
                                                PulseFlag::T7Flag,
                                                APulse::T7ParamName,
                                                &APulseCmd::setT7,
                                                errors));

        RETURN_IF_FALSE(validateAndSetFlagParam(parameters,
                                                PulseFlag::TfFlag,
                                                APulse::TfParamName,
                                                &APulseCmd::setTf,
                                                errors));

        RETURN_IF_FALSE(validateAndSetFlagParam(parameters,
                                                PulseFlag::T8Flag,
                                                APulse::T8ParamName,
                                                &APulseCmd::setT8,
                                                errors,
                                                Number(10)));

        RETURN_IF_FALSE(validateAndSetFlagParam(parameters,
                                                PulseFlag::InterFlag,
                                                APulse::InterruptionParamName,
                                                &APulseCmd::setInterruption,
                                                errors,
                                                Number(10)));

        RETURN_IF_FALSE(validateAndSetFlagParam(parameters,
                                                PulseFlag::NbCyclesFlag,
                                                APulse::NbCyclesParamName,
                                                &APulseCmd::setNbCycles,
                                                errors));

        if(_paramsFlag.testFlag(PulseFlag::TriggerFlag))
        {
            quint16 triggerValue;
            RETURN_IF_FALSE(manageTriggerSelection(parameters,
                                                   APulse::TriggerParamName,
                                                   errors,
                                                   triggerValue));
            _aPulseCmd->setTrigger(triggerValue);
        }

        RETURN_IF_FALSE(validateAndSetFlagParam(parameters,
                                                PulseFlag::CurrentFlag,
                                                APulse::CurrentParamName,
                                                &APulseCmd::setCurrent,
                                                errors));
    }

    if(!BankJsonParser::getNumberValue(parameters,
                                       APulse::TmoParamName,
                                       tmpValue,
                                       &error))
    {
        fillErrorArray(APulse::TmoParamName, error, errors);
        return false;
    }
    _timeout = tmpValue.toInt32();

    return true;
}

bool APulse::initInputsFormat(JsonArray &inputsFormat) const
{
    JsonObject interfaceId;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(APulse::InputInterfaceIdName,
                                                         interfaceId,
                                                         APulse::InputInterfaceIdDesc));

    inputsFormat.append(interfaceId);

    return true;
}

bool APulse::initNumberParams(const QString &paramName,
                              const QString &paramDesc,
                              const Number &min,
                              const Number &max,
                              const Number &step,
                              JsonArray &paramsFormat) const
{
    JsonObject paramObject;

    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(paramName,
                                                                 paramDesc,
                                                                 Number::MaxAllowedCommaShift,
                                                                 Number::MaxAllowedCommaShift,
                                                                 paramObject));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(paramObject,
                                                                   min,
                                                                   max,
                                                                   step));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, paramObject));

    paramsFormat.append(paramObject);

    return true;
}

bool APulse::initNbParamsWithTestFlags(PulseFlag::Flag paramFlag,
                                       const QString &paramName,
                                       const QString &paramDesc,
                                       const Number &min,
                                       const Number &max,
                                       const Number &step,
                                       JsonArray &paramsFormat) const
{
    if(_paramsFlag.testFlag(paramFlag))
    {
        return initNumberParams(paramName, paramDesc, min, max, step, paramsFormat);
    }

    return true;
}

bool APulse::validateAndSetFlagParam(const JsonArray &parameters,
                                     PulseFlag::Flag paramFlag,
                                     const QString &paramName,
                                     void (APulseCmd::*setter)(quint16),
                                     JsonArray &errors,
                                     const Number &unitFactor,
                                     const Number &offset)
{
    if(!_paramsFlag.testFlag(paramFlag))
    {
        // Nothing to do
        return true;
    }

    QString error;
    Number tmpValue;

    if(!BankJsonParser::getNumberValue(parameters,
                                       paramName,
                                       tmpValue,
                                       &error))
    {
        fillErrorArray(paramName, error, errors);
        return false;
    }

    if(offset.isValid())
    {
        tmpValue += offset;
    }

    if(unitFactor.isValid())
    {
        tmpValue *= unitFactor;
    }

    std::invoke(setter, _aPulseCmd, tmpValue.toUint16());

    return true;
}

bool APulse::manageTriggerSelection(const JsonArray &parameters,
                                    const char *paramName,
                                    JsonArray &errors,
                                    quint16 &trigger)
{
    QString error;
    QVector<QString> triggerValue;

    if(!BankJsonParser::getArrayStringValue(parameters,paramName, triggerValue, &error))
    {
        fillErrorArray(paramName, error, errors);
        return false;
    }

    if(triggerValue.isEmpty())
    {
        fillErrorArray(paramName, "No Enum type selected", errors);
        return false;
    }

    trigger = TriggerEnum::parseFromString(triggerValue.first());
    if(trigger == TriggerEnum::Unknown)
    {
        fillErrorArray(paramName,
                       "The trigger mode is unknown: " + triggerValue.first(),
                       errors);
        return false;
    }

    return true;
}

void APulse::manageConnection(bool toConnect)
{
    if(toConnect && !_connectedToComProcess)
    {
        connect(_writeProcess, &WriteProcess::responseReceived,
                this,          &APulse::onResponseReceived);
        connect(_writeProcess, &WriteProcess::errorOccured,
                this,          &APulse::onError);

        _connectedToComProcess = true;
    }
    else if(!toConnect && _connectedToComProcess)
    {
        disconnect(_writeProcess, &WriteProcess::responseReceived,
                   this,          &APulse::onResponseReceived);
        disconnect(_writeProcess, &WriteProcess::errorOccured,
                   this,          &APulse::onError);

        _connectedToComProcess = false;
    }
}

bool APulse::setupPulse()
{
    if(_writeProcess == nullptr)
    {
        return false;
    }

    if(_aPulseCmd == nullptr)
    {
        return false;
    }

    RETURN_IF_FALSE(_writeProcess->write(_aPulseCmd));

    _processStep = SetupPulse;

    emit newLog(SequenceLogData(LogMsgType::Info,
                                SequenceLogCategory::BusinessScope,
                                "--- CONFIGURE:  " + _currentPulseName +"  ---"));
    return true;
}

bool APulse::startPulse()
{
    if(_writeProcess == nullptr)
    {
        return false;
    }

    if(_aPulseCmd == nullptr)
    {
        return false;
    }

    RETURN_IF_FALSE(_writeProcess->query(new StartCmd()));

    _processStep = StartPulse;

    emit newLog(SequenceLogData(LogMsgType::Info,
                                SequenceLogCategory::BusinessScope,
                                "Pulse started"));

    return true;
}

bool APulse::terminatePulse()
{
    if(_writeProcess == nullptr)
    {
        return false;
    }

    if(_aPulseCmd == nullptr)
    {
        return false;
    }

    RETURN_IF_FALSE(_writeProcess->write(new TerminateCmd()));

    return true;
}

bool APulse::askInstrumentId()
{
    if(_writeProcess == nullptr)
    {
        return false;
    }

    if(_aPulseCmd == nullptr)
    {
        return false;
    }

    RETURN_IF_FALSE(_writeProcess->query(new IdnCmd()));

    _processStep = AskForIdn;

    return true;
}

bool APulse:: checkIfIdn(const QSharedPointer<AResponse>& response)
{
    QString strResponse;

    if(response->responseType != AResponse::IdResponse)
    {
        if(response->responseType != AResponse::Unknown)
        {
            QSharedPointer<BackResponse> backResponse = qSharedPointerCast<BackResponse>(response);
            strResponse = BackResponseType::reponseToString(backResponse->getBackResponseEnum());

            emit newLog(SequenceLogData(LogMsgType::Warning,
                                        SequenceLogCategory::BusinessScope,
                                        strResponse));
        }
        else
        {
            emit newLog(SequenceLogData(LogMsgType::Warning,
                                        SequenceLogCategory::BusinessScope,
                                        "/!\\ Unknown response received"));
        }

        return false;
    }

    QSharedPointer<IdResponse> idResponse = qSharedPointerCast<IdResponse>(response);
    strResponse = InstrumentId::toString(idResponse->getIdResponseEnum());
    emit newLog(SequenceLogData(LogMsgType::Info,
                                SequenceLogCategory::BusinessScope,
                                "\n--- INSTRUMENT ID:  " + strResponse + "  ---"));

    return true;
}

bool APulse::checkIfInfo(const QSharedPointer<AResponse> &response)
{
    if(response->responseType != AResponse::BackResponse)
    {
        return false;
    }

    QString strResponse;

    QSharedPointer<BackResponse> backResponse = qSharedPointerCast<BackResponse>(response);
    strResponse = BackResponseType::reponseToString(backResponse->getBackResponseEnum());

    if(backResponse->getBackResponseCategory() != BackResponseCategory::InfoResponse)
    {
        if(backResponse->getBackResponseCategory() != BackResponseCategory::Unknown)
        {
            emit newLog(SequenceLogData(LogMsgType::Warning,
                                        SequenceLogCategory::BusinessScope,
                                        strResponse));
        }
        else
        {
            emit newLog(SequenceLogData(LogMsgType::Warning,
                                        SequenceLogCategory::BusinessScope,
                                        strResponse));
        }

        return false;
    }

    emit newLog(SequenceLogData(LogMsgType::Info,
                                SequenceLogCategory::BusinessScope,
                                strResponse));

    return true;
}

void APulse::processModule(const JsonArray &jsonObjectInputValues)
{
    if(!PauseAndStopHelper::managePauseAndStop(getSequenceState()))
    {
        emit moduleFinished(false, {});
        return;
    }

    QString interfaceId;
    if(!BankJsonParser::getStringValue(jsonObjectInputValues,
                                       APulse::InputInterfaceIdName,
                                       interfaceId))
    {
        emit moduleFinished(false, {});
        return;
    }

    _writeProcess = new WriteProcess(interfaceId, _timeout, this);

    manageConnection(true);

    if(!askInstrumentId())
    {
        emit moduleFinished(false, {});
    }
}

void APulse::onResponseReceived(const QSharedPointer<AResponse> &response)
{

    if(response->responseType == AResponse::Unknown)
    {
        emit newLog(SequenceLogData(LogMsgType::Critical,
                                    SequenceLogCategory::BusinessScope,
                                    "--- INVALID RESPONSE ---"));
        emit moduleFinished(false, {});
        return;
    }

    if(_processStep == AskForIdn)
    {
        if(!checkIfIdn(response))
        {
            emit moduleFinished(false, {});
            return;
        }

        if(!setupPulse())
        {
            emit moduleFinished(false, {});
            return;
        }
    }

    if(_processStep == SetupPulse)
    {
        if(!startPulse())
        {
            emit moduleFinished(false, {});
            return;
        }

        return;
    }

    if(_processStep == StartPulse)
    {
        if(!checkIfInfo(response))
        {
            emit newLog(SequenceLogData(LogMsgType::Warning,
                                        SequenceLogCategory::BusinessScope,
                                        "Pulse ended with error"));
            emit moduleFinished(false, {});
            return;
        }
    }

    emit newLog(SequenceLogData(LogMsgType::Info,
                                SequenceLogCategory::BusinessScope,
                                "Pulse ended successfully"));

    manageConnection(false);

    emit moduleFinished(true, {});
}

void APulse::onError()
{
    emit newLog(SequenceLogData(LogMsgType::Critical,
                                SequenceLogCategory::BusinessScope,
                                "An error occured during visa communication !!!"));

    manageConnection(false);

    emit moduleFinished(false, {});
}
