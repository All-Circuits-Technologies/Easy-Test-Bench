// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "simplesequenceinstancemodule.hpp"

#include "basicbankinterface.hpp"

#include <QTimer>

QString SimpleSequenceInstanceModule::moduleName = "simplesequenceinstancemodule";


SimpleSequenceInstanceModule::SimpleSequenceInstanceModule(QObject *parent) :
    ISequenceInstanceModule(parent)
{
}

QSharedPointer<IBankInterface> SimpleSequenceInstanceModule::getBankInterface() const
{
    return BasicBankInterface::Instance();
}

bool SimpleSequenceInstanceModule::getModuleName(QString &uniqueName) const
{
    uniqueName = SimpleSequenceInstanceModule::moduleName;
    return true;
}

bool SimpleSequenceInstanceModule::getInstanceName(QString &instanceName) const
{
    instanceName = "InstanceModule";
    return true;
}

bool SimpleSequenceInstanceModule::setSequenceToken(const QString &sequenceToken)
{
    Q_UNUSED(sequenceToken)
    return true;
}

bool SimpleSequenceInstanceModule::launchModule(const JsonArray &jsonObjectInputValues)
{
    Q_UNUSED(jsonObjectInputValues)

    _running = true;

    QTimer::singleShot(0, this, [=]()
    {
        _running = false;
        emit moduleFinished(true, JsonArray());
    });

    return true;
}

bool SimpleSequenceInstanceModule::stopModule(StopAskedType::Enum stopAskedType)
{
    return true;
}

bool SimpleSequenceInstanceModule::manageModulePause(bool pauseModule)
{
    Q_UNUSED(pauseModule)
    return false;
}

bool SimpleSequenceInstanceModule::getState(SequenceStateType::Enum &currentState) const
{
    currentState = _running ? SequenceStateType::Running : SequenceStateType::Stopped;
    return true;
}

bool SimpleSequenceInstanceModule::getModuleParametersFormat(JsonArray &parameters) const
{
    parameters = JsonArray();
    return true;
}

bool SimpleSequenceInstanceModule::setModuleParameters(const JsonArray &parameters,
                                                       JsonArray &errors)
{
    Q_UNUSED(parameters)
    errors = JsonArray();
    return true;
}

bool SimpleSequenceInstanceModule::getModuleParameters(JsonArray &parameters) const
{
    parameters = JsonArray();
    return true;
}

bool SimpleSequenceInstanceModule::getInputVariablesFormat(JsonArray &inVariablesFormat) const
{
    inVariablesFormat = JsonArray();
    return true;
}

bool SimpleSequenceInstanceModule::getOutputVariablesFormat(JsonArray &outVariablesFormat) const
{
    outVariablesFormat = JsonArray();
    return true;
}
