// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "sequencemodulenode.hpp"

#include "bankcoreshared/usebylib/ibankinterface.hpp"
#include "bankcoreshared/usebylib/isequenceinstancemodule.hpp"
#include "errors/helpers/fileloadingerrorcausehelper.hpp"
#include "jsonparsers/nodestojsonparser.hpp"
#include "sequence/accesskeys/sequencemodulenodekey.hpp"
#include "sequence/nodessequence.hpp"
#include "sequence/referencenodes/variablenode.hpp"
#include "testbedcore/parsers/jsonparsersconstants.hpp"
#include "testbedcore/testbedglobal.hpp"


SequenceModuleNode::SequenceModuleNode(const QSharedPointer<NodesSequence> &sequenceParent,
                                       QObject *parent) :
    ATargetingNode(NodeType::SequenceModuleNode, sequenceParent, parent)
{
}

SequenceModuleNode::~SequenceModuleNode()
{
    stopAndDeleteSequenceModule();
}

void SequenceModuleNode::stopAndDeleteSequenceModule()
{
    if(!_sequenceItfModule)
    {
        return;
    }

    SequenceStateType::Enum state;
    if(!_sequenceItfModule->getState(state))
    {
        qWarning() << "Cannot get the state of the sequence instance module of: "
                   << getInstanceName();

        // Force deletion
        delete _sequenceItfModule;
        _sequenceItfModule = nullptr;
        return;
    }

    if(state == SequenceStateType::Stopped)
    {
        // Nothing to do the sequence is already stopped
        delete _sequenceItfModule;
        _sequenceItfModule = nullptr;
        return;
    }

    // Detach the module signals from the sequence module
    manageConnectionSeqModuleSignals(_sequenceItfModule, false);

    connect(_sequenceItfModule, &ISequenceInstanceModule::moduleFinished,
            _sequenceItfModule, &ISequenceInstanceModule::deleteLater);

    _sequenceItfModule->stopModule(StopAskedType::EndOfProcess);
}

bool SequenceModuleNode::setSequenceBankIft(ISequenceInstanceModule *seqBankItf,
                                            SequenceLoadingErrorHandler *errorHandler)
{
    if(_sequenceItfModule != seqBankItf)
    {
        manageConnectionSeqModuleSignals(_sequenceItfModule, false);

        if(!seqBankItf)
        {
            // No need to get names and connect signals to a null object
            _sequenceItfModule = nullptr;
            return true;
        }

        QSharedPointer<IBankInterface> bankItf = seqBankItf->getBankInterface();

        if(!bankItf)
        {
            FileLoadingErrorCauseHelper::addUnavailableBankInterfaceError(getInstanceName(),
                                                                          errorHandler);
            qWarning() << "The bank interface linked to the node: " << getInstanceName()
                       << ", is null";
            return false;
        }

        if(!bankItf->getUniqueName(_bankName))
        {
            FileLoadingErrorCauseHelper::addUnavailableBankNameError(getInstanceName(),
                                                                     errorHandler);
            qWarning() << "Can't get the name of the bank interface linked to the node: "
                       << getInstanceName();
            return false;
        }

        if(!seqBankItf->getModuleName(_moduleName))
        {
            FileLoadingErrorCauseHelper::addUnavailableModuleNameError(getInstanceName(),
                                                                       errorHandler);
            qWarning() << "Can't get the name of the module linked to the node: "
                       << getInstanceName();
            return false;
        }

        _sequenceItfModule = seqBankItf;

        manageConnectionSeqModuleSignals(_sequenceItfModule, true);
    }

    return true;
}

void SequenceModuleNode::nodeActivity()
{
    ATargetingNode::nodeActivity();

    qDebug() << "On module start: " << getInstanceName();

    if(!testStoppingAndPausingAtRunTime())
    {
        return;
    }

    if(!_enable)
    {
        qInfo() << "The sequence module: " << getInstanceName() << ", is disabled. "
                << "Go to the next node";
        exitProcess();
        return;
    }

    if(!_sequenceItfModule)
    {
        errorProcess(*this, tr("(TR) No sequence interface module has been linked to this module"));
        return;
    }

    JsonArray inputs;

    if(!NodesToJsonParser::sequenceModuleInputValuesToJsonArray(*this, inputs))
    {
        errorProcess(*this, tr("(TR) Can't get the input values of the node"));
        return;
    }

    connect(&getInstanceParent()->getSequenceState(), &SequenceStateHelper::newState,
            this,                                     &SequenceModuleNode::onNewSequenceState);
    connect(&getInstanceParent()->getSequenceState(), &SequenceStateHelper::stopAsked,
            this,                                     &SequenceModuleNode::onStopAskedInSequence);

    if(!_sequenceItfModule->launchModule(inputs))
    {
        errorProcess(*this,
                   tr("(TR) The module linked: %1(bank: %2), can't be started").arg(_moduleName,
                                                                                    _bankName));
        return;
    }
}

void SequenceModuleNode::onModuleFinished(bool success, const JsonArray &jsonObjectOutputValues)
{
    disconnect(&getInstanceParent()->getSequenceState(), &SequenceStateHelper::newState,
               this,                                     &SequenceModuleNode::onNewSequenceState);
    disconnect(&getInstanceParent()->getSequenceState(),
               &SequenceStateHelper::stopAsked,
               this,
               &SequenceModuleNode::onStopAskedInSequence);

    qDebug() << "On module finished: " << getInstanceName();

    if(!success)
    {
        errorProcess(*this,
                   tr("(TR) An error occurred in the module:  %1 (bank: %2)").arg(_moduleName,
                                                                                  _bankName));
        return;
    }

    if(!manageWriteOutputValues(jsonObjectOutputValues))
    {
        errorProcess(*this, tr("(TR) An error occurred when trying to set output values to "
                               "variables"));
        return;
    }

    exitProcess();
}

void SequenceModuleNode::onNewModuleState(SequenceStateType::Enum newModuleState,
                                          SequenceStateType::Enum previousModuleState)
{
    Q_UNUSED(newModuleState)
    Q_UNUSED(previousModuleState)
}

void SequenceModuleNode::onNewSequenceState(SequenceStateType::Enum newState,
                                            SequenceStateType::Enum previousState)
{
    if(!_sequenceItfModule)
    {
        return;
    }

    if(newState == SequenceStateType::InPause)
    {
        _sequenceItfModule->manageModulePause(true);
    }
    else if(previousState == SequenceStateType::InPause && newState == SequenceStateType::Running)
    {
        _sequenceItfModule->manageModulePause(false);
    }
}

void SequenceModuleNode::onStopAskedInSequence(StopAskedType::Enum stopAskedType)
{
    if(!_sequenceItfModule)
    {
        return;
    }

    _sequenceItfModule->stopModule(stopAskedType);
}

void SequenceModuleNode::manageConnectionSeqModuleSignals(ISequenceInstanceModule *seqItfModule,
                                                          bool connectSignals)
{
    if(!seqItfModule)
    {
        return;
    }

    if(connectSignals)
    {
        connect(seqItfModule, &ISequenceInstanceModule::newState,
                this,         &SequenceModuleNode::onNewModuleState);
        connect(seqItfModule, &ISequenceInstanceModule::moduleFinished,
                this,         &SequenceModuleNode::onModuleFinished);
        connect(seqItfModule, &ISequenceInstanceModule::newLog,
                this,         &SequenceModuleNode::newLog);
    }
    else
    {
        disconnect(seqItfModule, &ISequenceInstanceModule::newState,
                   this,         &SequenceModuleNode::onNewModuleState);
        disconnect(seqItfModule, &ISequenceInstanceModule::moduleFinished,
                   this,         &SequenceModuleNode::onModuleFinished);
        disconnect(seqItfModule, &ISequenceInstanceModule::newLog,
                   this,         &SequenceModuleNode::newLog);
    }
}

bool SequenceModuleNode::manageWriteOutputValues(const JsonArray &outputValues)
{
    QHash<QString, QVariant> values;

    JsonArray::const_iterator iter = outputValues.cbegin();

    for(;iter != outputValues.cend(); ++iter)
    {
        if(!iter->canConvert<JsonObject>())
        {
            qWarning() << "The element of the JsonArray returned by the sequence module: "
                       << _moduleName << " (bank: " << _bankName << ") is not a JsonObject";
            return false;
        }

        JsonObject obj = iter->value<JsonObject>();
        
        if(!obj.contains(JsonParsersConstants::Generic::NameAttribute))
        {
            qWarning() << "The element of the JsonArray returned by the sequence module: "
                       << _moduleName << " (bank: " << _bankName << ") has no attribute: "
                       << JsonParsersConstants::Generic::NameAttribute;
            return false;
        }
        
        QVariant nameVar = obj.value(JsonParsersConstants::Generic::NameAttribute);

        if(nameVar.isNull() || !nameVar.canConvert<QString>())
        {
            qWarning() << "The element of the JsonArray returned by the sequence module: "
                       << _moduleName << " (bank: " << _bankName << ") has a wrong format, it's "
                       << "not a QString";
            return false;
        }

        QString name = nameVar.toString();

        QVariant value;

        if(obj.contains(JsonParsersConstants::Sequence::ValueNameAttribute))
        {
            value = obj.value(JsonParsersConstants::Sequence::ValueNameAttribute);
        }
        else if(obj.contains(JsonParsersConstants::Sequence::ValuesNameAttribute))
        {
            value = obj.value(JsonParsersConstants::Sequence::ValuesNameAttribute);
        }
        else
        {
            qWarning() << "The element of the JsonArray returned by the sequence module: "
                       << _moduleName << " (bank: " << _bankName << ") has no attributes: "
                       << JsonParsersConstants::Sequence::ValueNameAttribute << " or "
                       << JsonParsersConstants::Sequence::ValuesNameAttribute;
            return false;
        }

        values.insert(name, value);
    }

    for(const OutNodePort &outPort : _referencesHolder.getOutputs())
    {
        if(outPort.getOutputNode().isNull())
        {
            // In this case, we had a tolerance: the output value isn't not expected by a variable
            // so even if the output value is not present, we don't raise an error
            continue;
        }

        if(!values.contains(outPort.getName()))
        {
            qWarning() << "The output: " << outPort.getName() << ", was expected but nothing has "
                       << "been returned by the sequence module: " << _moduleName << " (bank: "
                       << _bankName << ")";
            return false;
        }

        outPort.getOutputNode()->setVariableValue(SequenceModuleNodeKey(),
                                                  values.value(outPort.getName()));
    }

    return true;
}
