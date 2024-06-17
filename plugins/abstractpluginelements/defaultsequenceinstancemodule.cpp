// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "defaultsequenceinstancemodule.hpp"

#include <QTimer>
#include <QUuid>

#include "bankcoreshared/usebyplugin/icommoninterface.hpp"
#include "testbedcore/parsers/bankjsonparamvalidator.hpp"
#include "testbedcore/testbedglobal.hpp"

#include "defaultbankinterface.hpp"


DefaultSequenceInstanceModule::DefaultSequenceInstanceModule(const QString &moduleClassName,
                                                const QSharedPointer<DefaultBankInterface> &bankLinked,
                                                QObject *parent) :
    ISequenceInstanceModule(parent),
    _moduleName(moduleClassName.toLower()),
    _instanceName(generateInstanceName(_moduleName)),
    _bankLinked(bankLinked)
{
    connect(this, &ISequenceInstanceModule::moduleFinished,
            this, &DefaultSequenceInstanceModule::endProcess);
}

QSharedPointer<IBankInterface> DefaultSequenceInstanceModule::getBankInterface() const
{
    return _bankLinked;
}

bool DefaultSequenceInstanceModule::getModuleName(QString &uniqueName) const
{
    uniqueName = _moduleName;
    return true;
}

bool DefaultSequenceInstanceModule::getInstanceName(QString &instanceName) const
{
    instanceName = _instanceName;
    return true;
}

bool DefaultSequenceInstanceModule::setSequenceToken(const QString &sequenceToken)
{
    _sequenceToken = sequenceToken;
    return true;
}

bool DefaultSequenceInstanceModule::getState(SequenceStateType::Enum &currentState) const
{
    currentState = _sequenceState.getCurrentState();
    return true;
}

bool DefaultSequenceInstanceModule::getModuleParametersFormat(JsonArray &parameters) const
{
    RETURN_IF_FALSE(manageFormatElement(_parametersFormat,
                                        [this](JsonArray &elements) {
                                            return this->initParamsFormat(elements);
                                        },
                                        "parameters"));

    parameters = _parametersFormat.format;
    return true;
}

bool DefaultSequenceInstanceModule::setModuleParameters(const JsonArray &parameters,
                                                        JsonArray &errors)
{
    RETURN_IF_FALSE(validateAndSetParameters(parameters, errors));

    _parameters = parameters;

    return true;
}

bool DefaultSequenceInstanceModule::getModuleParameters(JsonArray &parameters) const
{
    parameters = _parameters;
    return true;
}

bool DefaultSequenceInstanceModule::launchModule(const JsonArray &jsonObjectInputValues)
{
    Q_UNUSED(jsonObjectInputValues)
    _sequenceState.onRunning();

    QTimer::singleShot(0, this, [=](){
        processModule(jsonObjectInputValues);
    });

    return true;
}

bool DefaultSequenceInstanceModule::stopModule(StopAskedType::Enum stopAskedType)
{
    _sequenceState.onStopAsked(stopAskedType);
    return true;
}

bool DefaultSequenceInstanceModule::manageModulePause(bool pauseModule)
{
    _sequenceState.onPauseOrResume(pauseModule);
    return true;
}

bool DefaultSequenceInstanceModule::getInputVariablesFormat(JsonArray &inVariablesFormat) const
{
    RETURN_IF_FALSE(manageFormatElement(_inputsFormat,
                                        [this](JsonArray &elements) {
                                            return this->initInputsFormat(elements);
                                        },
                                        "inputs"));

    inVariablesFormat = _inputsFormat.format;
    return true;
}

bool DefaultSequenceInstanceModule::getOutputVariablesFormat(JsonArray &outVariablesFormat) const
{
    RETURN_IF_FALSE(manageFormatElement(_outputsFormat,
                                        [this](JsonArray &elements) {
                                            return this->initOutputsFormat(elements);
                                        },
                                        "outputs"));

    outVariablesFormat = _outputsFormat.format;
    return true;
}

ICommonInterface &DefaultSequenceInstanceModule::accessCommIntf() const
{
    return _bankLinked->accessCommonInterface();
}

const ICommonInterface &DefaultSequenceInstanceModule::getCommIntf() const
{
    return _bankLinked->getCommonInterface();
}

void DefaultSequenceInstanceModule::onNewSeqLogs(const SequenceLogData &log)
{
    emit newLog(log);
}

bool DefaultSequenceInstanceModule::validateAndSetParameters(const JsonArray &parameters,
                                                             JsonArray &errors)
{
    RETURN_IF_FALSE(manageFormatElement(_parametersFormat,
                                        [this](JsonArray &elements) {
                                            return this->initParamsFormat(elements);
                                        },
                                        "parameters"));

    return BankJsonParamValidator::validateModuleParameters(_parametersFormat.format,
                                                            parameters,
                                                            errors);
}

bool DefaultSequenceInstanceModule::initParamsFormat(JsonArray &paramsFormat) const
{
    paramsFormat.clear();
    return true;
}

bool DefaultSequenceInstanceModule::initInputsFormat(JsonArray &inputsFormat) const
{
    inputsFormat.clear();
    return true;
}

bool DefaultSequenceInstanceModule::initOutputsFormat(JsonArray &outputsFormat) const
{
    outputsFormat.clear();
    return true;
}

void DefaultSequenceInstanceModule::endProcess(bool success, const JsonArray &outputValues)
{
    Q_UNUSED(outputValues)

    _sequenceState.onStop();

    if(!success)
    {
        emit newLog(SequenceLogData(LogMsgType::Critical,
                                    SequenceLogCategory::ApplicationScope,
                                    tr("(TR) The sequence module: %1, has ended in error")
                                                                                .arg(_moduleName)));
    }
}

void DefaultSequenceInstanceModule::fillErrorArray(const QString &parameterName,
                                                   const QString &errorStr,
                                                   JsonArray &errorArray)
{
    BankJsonParamValidator::fillErrorArray(parameterName, errorStr, errorArray);
}

QString DefaultSequenceInstanceModule::generateInstanceName(const QString &moduleName)
{
    QUuid uuid = QUuid::createUuid();
    quint32 hash = qHash(uuid);

    return moduleName + "-" + QString::number(hash, DefaultSequenceInstanceModule::HexBase);
}

bool DefaultSequenceInstanceModule::manageFormatElement(
        DefaultSequenceInstanceModule::FormatElement &element,
        const std::function<bool (JsonArray &)> &initMethod,
        const QString &elementName)
{
    if(element.result == FormatResult::NotLoaded)
    {
        element.result = initMethod(element.format) ?
                                        FormatResult::Success :
                                        FormatResult::Error;
    }

    if(element.result == FormatResult::Error)
    {
        qWarning() << "Can't proceed the validation and " << elementName << " setting, no valid "
                   << elementName << " format have been given";
        return false;
    }

    return true;
}
