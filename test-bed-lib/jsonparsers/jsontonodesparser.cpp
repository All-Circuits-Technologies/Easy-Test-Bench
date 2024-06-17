// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "jsontonodesparser.hpp"

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>

#include "bank/bankmanager.hpp"
#include "bankcoreshared/usebylib/isequenceinstancemodule.hpp"
#include "core/globalmanager.hpp"
#include "errors/helpers/fileloadingerrorcausehelper.hpp"
#include "errors/sequenceloadingerrorhandler.hpp"
#include "jsonutility/jsonhelper.hpp"
#include "jsonutility/jsontypedefs.hpp"
#include "sequence/nodes/sequencemodulenode.hpp"
#include "sequence/nodessequence.hpp"
#include "sequence/referencenodes/constantnode.hpp"
#include "sequence/referencenodes/variablenode.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/parsers/commonjsonparser.hpp"
#include "testbedcore/parsers/jsonparsersconstants.hpp"
#include "testbedcore/types/typemanaged.hpp"
#include "testbedcore/types/unit.hpp"


bool JsonToNodesParser::loadNodesSequenceReferences(const QJsonObject &nodeObject,
                                                    NodesSequence &nodesSequence,
                                                    const NodesSequence *parentNodesSequence,
                                                    SequenceLoadingErrorHandler *errorHandler)
{
    RETURN_IF_FALSE(loadInputReferencesHolder(nodeObject,
                                              nodesSequence.accessReferencesHolder(),
                                              &nodesSequence,
                                              parentNodesSequence,
                                              errorHandler));

    RETURN_IF_FALSE(loadOutputReferencesHolder(nodeObject,
                                               nodesSequence.accessReferencesHolder(),
                                               &nodesSequence,
                                               parentNodesSequence,
                                               errorHandler));

    return true;
}

bool JsonToNodesParser::getParentInstanceName(const QJsonObject &node,
                                              QString &parentName,
                                              SequenceLoadingErrorHandler */*errorHandler*/)
{
    return JsonHelper::getString(node,
                                 JsonParsersConstants::Sequence::ParentInstanceNameAttribute,
                                 parentName);
}

bool JsonToNodesParser::getNodeType(const QJsonObject &node, NodeType::Enum &nodeType,
                                    SequenceLoadingErrorHandler *errorHandler)
{
    QString typeStr;

    TEST_AND_RETURN_IF_FALSE(JsonHelper::getString(node,
                                                   JsonParsersConstants::Generic::TypeName,
                                                   typeStr),
                             FileLoadingErrorCauseHelper::addUnavailableNodeTypeError(
                                 errorHandler););

    nodeType = NodeType::parseFromString(typeStr);

    return true;
}

bool JsonToNodesParser::loadAbstractNode(const QJsonObject &nodeObject,
                                         ANode &node,
                                         SequenceLoadingErrorHandler *errorHandler)
{
    QString uniqueName;

    TEST_AND_RETURN_IF_FALSE(JsonHelper::getString(
                                 nodeObject,
                                 JsonParsersConstants::Generic::UniqueNameAttribute,
                                 uniqueName),
                             FileLoadingErrorCauseHelper::addUnavailableAttributeError(
                                 JsonParsersConstants::Generic::UniqueNameAttribute,
                                 errorHandler););


    node.setInstanceName(uniqueName);

    QString description;

    TEST_AND_RETURN_IF_FALSE(JsonHelper::getString(
                                 nodeObject,
                                 JsonParsersConstants::Generic::DescriptionName,
                                 description,
                                 true),
                             FileLoadingErrorCauseHelper::addUnavailableAttributeError(
                                 JsonParsersConstants::Generic::DescriptionName,
                                 errorHandler););

    node.setDescription(description);

    return true;
}

bool JsonToNodesParser::loadSequenceModuleNode(const QJsonObject &sequenceModuleNode,
                                               const NodesSequence &nodesSequence,
                                               SequenceModuleNode &sequenceNode,
                                               SequenceLoadingErrorHandler *errorHandler)
{
    RETURN_IF_FALSE(loadAbstractNode(sequenceModuleNode, sequenceNode));

    RETURN_IF_FALSE(loadInputReferencesHolder(sequenceModuleNode,
                                              sequenceNode.accessReferencesHolder(),
                                              nullptr,
                                              &nodesSequence,
                                              errorHandler));

    RETURN_IF_FALSE(loadOutputReferencesHolder(sequenceModuleNode,
                                               sequenceNode.accessReferencesHolder(),
                                               nullptr,
                                               &nodesSequence,
                                               errorHandler));

    // Load all the things linked with Bank module

    QString bankName;

    TEST_AND_RETURN_IF_FALSE(JsonHelper::getString(
                                 sequenceModuleNode,
                                 JsonParsersConstants::Sequence::BankNameAttribute,
                                 bankName),
                             FileLoadingErrorCauseHelper::addUnavailableAttributeError(
                                 JsonParsersConstants::Sequence::BankNameAttribute,
                                 errorHandler););

    QString moduleName;

    TEST_AND_RETURN_IF_FALSE(JsonHelper::getString(
                                 sequenceModuleNode,
                                 JsonParsersConstants::Sequence::ModuleNameAttribute,
                                 moduleName),
                             FileLoadingErrorCauseHelper::addUnavailableAttributeError(
                                 JsonParsersConstants::Sequence::ModuleNameAttribute,
                                 errorHandler););

    bool disable = !sequenceNode.isEnable();

    TEST_AND_RETURN_IF_FALSE(JsonHelper::getBoolean(
                                 sequenceModuleNode,
                                 JsonParsersConstants::Generic::DisableAttributeName,
                                 disable,
                                 true),
                             FileLoadingErrorCauseHelper::addUnavailableAttributeError(
                                 JsonParsersConstants::Generic::DisableAttributeName,
                                 errorHandler););

    sequenceNode.setEnable(!disable);

    QSharedPointer<IBankInterface> bankItf =
                                GlobalManager::instance().getBankMngr().getBankInterface(bankName);

    if(!bankItf)
    {
        FileLoadingErrorCauseHelper::addBankNotFoundError(bankName,
                                                          sequenceNode.getInstanceName(),
                                                          errorHandler);
        qWarning() << "The bank: " << bankName << ", linked to the sequence module: "
                   << sequenceNode.getInstanceName() << ", doesn't exist";
        return false;
    }

    ISequenceInstanceModule *iSeqModule = bankItf->createSequenceInstanceModule(
                                                            moduleName,
                                                            nodesSequence.getUniqueToken());

    if(!iSeqModule)
    {
        FileLoadingErrorCauseHelper::addCannotCreateSequenceModuleInBank(
            sequenceNode.getInstanceName(),
            bankName,
            errorHandler);
        qWarning() << "Can't create the sequence module instance named: " << moduleName << ", in "
                   << "bank: " << bankName;
        return false;
    }

    if(!sequenceNode.setSequenceBankIft(iSeqModule, errorHandler))
    {
        delete iSeqModule;
        return false;
    }

    QJsonArray moduleParams;

    TEST_AND_RETURN_IF_FALSE(JsonHelper::getArray(
                                 sequenceModuleNode,
                                 JsonParsersConstants::Sequence::ModuleParametersNameAttribute,
                                 moduleParams),
                             FileLoadingErrorCauseHelper::addUnavailableAttributeError(
                                 JsonParsersConstants::Sequence::ModuleParametersNameAttribute,
                                 errorHandler););

    RETURN_IF_FALSE(loadModuleParameters(moduleParams, iSeqModule, errorHandler));

    return true;
}

bool JsonToNodesParser::loadModuleParameters(const QJsonArray &moduleParams,
                                             ISequenceInstanceModule *&module,
                                             SequenceLoadingErrorHandler *errorHandler)
{
    JsonArray paramFormat;
    TEST_AND_RETURN_IF_FALSE(module->getModuleParametersFormat(paramFormat),
                             FileLoadingErrorCauseHelper::addUnavailableModuleParamsFormatError(
                                 errorHandler););

    JsonArray paramsValues;
    for(int idx = 0; idx < moduleParams.size(); idx++)
    {
        QJsonObject paramObject;
        TEST_AND_RETURN_IF_FALSE(JsonHelper::getObject(moduleParams, idx, paramObject),
                                 FileLoadingErrorCauseHelper::addUnavailableModuleParamsError(
                                     errorHandler););

        QString name;
        TEST_AND_RETURN_IF_FALSE(JsonHelper::getString(
                                     paramObject,
                                     JsonParsersConstants::Generic::NameAttribute,
                                     name),
                                 FileLoadingErrorCauseHelper::addUnavailableModuleAttributeError(
                                     JsonParsersConstants::Generic::NameAttribute,
                                     errorHandler););

        QString formatType;
        if(!BankJsonParser::getSpecificAttributeValue<QString>(
                paramFormat,
                name,
                JsonParsersConstants::Generic::TypeName,
                formatType))
        {
            //This element is not known by the module, continue
            continue;
        }

        TypeManaged::Enum typeManaged;
        RETURN_IF_FALSE(CommonJsonParser::castParamFormatTypeToManagedType(formatType,
                                                                           typeManaged,
                                                                           errorHandler));

        QString valueAttr = CommonJsonParser::getValueAttributeName(typeManaged);

        QVariant value;
        RETURN_IF_FALSE(CommonJsonParser::getValue(paramObject,
                                                   valueAttr,
                                                   typeManaged,
                                                   value,
                                                   false,
                                                   errorHandler));

        paramsValues.append(QVariant::fromValue(JsonObject({
            { JsonParsersConstants::Generic::NameAttribute, name },
            { valueAttr, value }
        })));
    }

    JsonArray errors;
    if(!module->setModuleParameters(paramsValues, errors))
    {
        FileLoadingErrorCauseHelper::addCannotSetModuleParamsError(errors, errorHandler);
        qWarning() << errors;
        return false;
    }

    return true;
}

bool JsonToNodesParser::loadConstantNode(const QJsonObject &constantObject,
                                         ConstantNode &constantNode,
                                         SequenceLoadingErrorHandler *errorHandler)
{
    RETURN_IF_FALSE(loadReferenceNode(constantObject, constantNode, errorHandler));

    QString attributeName = CommonJsonParser::getValueAttributeName(constantNode.getTypeManaged());

    QVariant value;

    RETURN_IF_FALSE(CommonJsonParser::getValue(constantObject,
                                               attributeName,
                                               constantNode.getTypeManaged(),
                                               value,
                                               false,
                                               errorHandler));

    RETURN_IF_FALSE(constantNode.setConstantValue(value));

    return true;
}

bool JsonToNodesParser::loadVariableNode(const QJsonObject &variableObject,
                                         VariableNode &variableNode,
                                         SequenceLoadingErrorHandler *errorHandler)
{
    return loadReferenceNode(variableObject, variableNode, errorHandler);
}

bool JsonToNodesParser::loadReferenceNode(const QJsonObject &referenceObject,
                                          AReferenceNode &referenceNode,
                                          SequenceLoadingErrorHandler *errorHandler)
{
    RETURN_IF_FALSE(loadAbstractNode(referenceObject, referenceNode, errorHandler));

    QString managedTypeStr;

    TEST_AND_RETURN_IF_FALSE(JsonHelper::getString(
                                 referenceObject,
                                 JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                 managedTypeStr),
                             FileLoadingErrorCauseHelper::addUnavailableAttributeError(
                                 JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                 errorHandler););

    TypeManaged::Enum managedType = TypeManaged::parseFromString(managedTypeStr);

    if(managedType == TypeManaged::Unknown)
    {
        FileLoadingErrorCauseHelper::addUnknownTypeManagedError(managedTypeStr, errorHandler);
        qWarning() << "A problem occurred with the reference node, the type managed isn't known: "
                   << managedTypeStr;
        return false;
    }

    referenceNode.setTypeManaged(managedType);

    Unit unit;
    RETURN_IF_FALSE(CommonJsonParser::getUnit(referenceObject, unit, errorHandler));
    referenceNode.setUnit(unit);

    return true;
}

bool JsonToNodesParser::loadANodePort(const QJsonObject &nodePortObject,
                                      ANodePort &aNodePort,
                                      SequenceLoadingErrorHandler *errorHandler)
{
    QString name;

    TEST_AND_RETURN_IF_FALSE(JsonHelper::getString(
                                 nodePortObject,
                                 JsonParsersConstants::Generic::NameAttribute,
                                 name),
                             FileLoadingErrorCauseHelper::addUnavailableAttributeError(
                                 JsonParsersConstants::Generic::NameAttribute,
                                 errorHandler););

    aNodePort.setName(name);

    QString typeManagedStr;

    TEST_AND_RETURN_IF_FALSE(JsonHelper::getString(
                                 nodePortObject,
                                 JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                 typeManagedStr),
                             FileLoadingErrorCauseHelper::addUnavailableAttributeError(
                                 JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                 errorHandler););

    TypeManaged::Enum typeManaged = TypeManaged::parseFromString(typeManagedStr);

    if(typeManaged == TypeManaged::Unknown)
    {
        FileLoadingErrorCauseHelper::addUnknownTypeManagedError(typeManaged, errorHandler);
        qWarning() << "The type managed is unknown: " << typeManagedStr;
        return false;
    }

    aNodePort.setTypeManaged(typeManaged);

    Unit unit;
    RETURN_IF_FALSE(CommonJsonParser::getUnit(nodePortObject, unit));
    aNodePort.setUnit(unit);

    return true;
}

bool JsonToNodesParser::loadInputReferencesHolder(const QJsonObject &nodeObject,
                                                  ReferencesHolder &referencesHolder,
                                                  const NodesSequence *internalNodesSequence,
                                                  const NodesSequence *parentNodesSequence,
                                                  SequenceLoadingErrorHandler *errorHandler)
{
    QJsonArray inputsArray;

    TEST_AND_RETURN_IF_FALSE(JsonHelper::getArray(
                                 nodeObject,
                                 JsonParsersConstants::Sequence::InputsNameAttribute,
                                 inputsArray),
                             FileLoadingErrorCauseHelper::addUnavailableAttributeError(
                                 JsonParsersConstants::Sequence::InputsNameAttribute,
                                 errorHandler););

    QJsonArray::const_iterator iter = inputsArray.constBegin();

    for(;iter != inputsArray.constEnd(); ++iter)
    {
        if(!iter->isObject())
        {
            FileLoadingErrorCauseHelper::addInputsArrayNotWellFormedError(errorHandler);
            qWarning() << "The inputs array is not well formed";
            return false;
        }

        QJsonObject inputObj = iter->toObject();

        InNodePort input;

        RETURN_IF_FALSE(loadANodePort(inputObj, input));

        QString refNodeName;

        TEST_AND_RETURN_IF_FALSE(JsonHelper::getString(
                                     inputObj,
                                     JsonParsersConstants::Sequence::ReferenceNodeNameAttribute,
                                     refNodeName,
                                     true),
                                 FileLoadingErrorCauseHelper::addUnavailableAttributeError(
                                     JsonParsersConstants::Sequence::ReferenceNodeNameAttribute,
                                     errorHandler););

        if(!refNodeName.isEmpty())
        {
            if(parentNodesSequence == nullptr)
            {
                FileLoadingErrorCauseHelper::addUnavailableExternalNodesSequenceError(refNodeName,
                                                                                      errorHandler);
                qWarning() << "Cannot load the reference node: " << refNodeName << ", the external "
                           << "nodes sequence given is null";
                return false;
            }

            QSharedPointer<AReferenceNode> refNode =
                                            parentNodesSequence->getReferenceNode(refNodeName);

            if(refNode.isNull())
            {
                FileLoadingErrorCauseHelper::addInputNodeNotFoundError(refNodeName,
                                                                       input.getName(),
                                                                       errorHandler);
                qWarning() << "Can't find the input reference named: " << refNodeName
                           << "for the entry: " << input.getName();
                return false;
            }

            RETURN_IF_FALSE(input.setInputNode(refNode));
        }

        QString internalRefNodeName;

        TEST_AND_RETURN_IF_FALSE(JsonHelper::getString(
                                     inputObj,
                                     JsonParsersConstants::Sequence::
                                        InternalBindingRefNodeNameAttribute,
                                     internalRefNodeName,
                                     true),
                                FileLoadingErrorCauseHelper::addUnavailableAttributeError(
                                     JsonParsersConstants::Sequence::
                                        InternalBindingRefNodeNameAttribute,
                                     errorHandler););

        if(!internalRefNodeName.isEmpty())
        {
            if(internalNodesSequence == nullptr)
            {
                FileLoadingErrorCauseHelper::addUnavailableInternalNodesSequenceError(
                    internalRefNodeName,
                    errorHandler);
                qWarning() << "Cannot load the reference node: " << internalRefNodeName << ", the "
                           << "internal nodes sequence given is null";
                return false;
            }

            QSharedPointer<AReferenceNode> internalRefNode =
                                     internalNodesSequence->getReferenceNode(internalRefNodeName);

            if(internalRefNode.isNull())
            {
                FileLoadingErrorCauseHelper::addInputNodeNotFoundError(internalRefNodeName,
                                                                       input.getName(),
                                                                       errorHandler);
                qWarning() << "Can't find the internal input reference named: "
                           << internalRefNodeName << "for the entry: " << input.getName();
                return false;
            }

            RETURN_IF_FALSE(input.setInternalInputNode(internalRefNode));
        }

        bool refCanBeUndefined = false;

        TEST_AND_RETURN_IF_FALSE(JsonHelper::getBoolean(
                                     inputObj,
                                     JsonParsersConstants::Sequence::CanBeUndefinedAttribute,
                                     refCanBeUndefined,
                                     true),
                                 FileLoadingErrorCauseHelper::addUnavailableAttributeError(
                                     JsonParsersConstants::Sequence::CanBeUndefinedAttribute,
                                     errorHandler););

        input.setCanReferenceBeUndefined(refCanBeUndefined);

        addOrUpdateInputReferences(input, referencesHolder);
    }

    return true;
}

bool JsonToNodesParser::addOrUpdateInputReferences(const InNodePort &inPort,
                                                   ReferencesHolder &referencesHolder,
                                                   SequenceLoadingErrorHandler */*errorHandler*/)
{
    PRINT_WARNING(Need to implement the errorHandler in the input variable method)
    for(InNodePort &input : referencesHolder.accessInputs())
    {
        if(input.getName() == inPort.getName())
        {
            if(input.getTypeManaged() != inPort.getTypeManaged())
            {
                qWarning() << "There are two input node ports with the same name: "
                           << input.getName() << ", but with a different managed type";
                return false;
            }

            if(input.getUnit() != inPort.getUnit())
            {
                qWarning() << "There are two input node ports with the same name: "
                           << input.getName() << ", but with different units";
                return false;
            }

            if(input.isReferenceCanBeUndefined() != inPort.isReferenceCanBeUndefined())
            {
                qWarning() << "There are two input node ports with the same name: "
                           << input.getName() << ", but with a different value for the option: "
                           << "\"canBeUndefined\"";
                return false;
            }

            if(!input.getInputNode().isNull()   &&
               !inPort.getInputNode().isNull()  &&
               input.getInputNode() != inPort.getInputNode())
            {
                qWarning() << "There are two input node ports with the same name: "
                           << input.getName() << ", but with a different input node attached, one "
                           << "is: " << input.getInputNode()->getInstanceName() << ", and the "
                           << "other is: " << inPort.getInputNode()->getInstanceName();
                return false;
            }

            if(input.getInputNode().isNull() && !inPort.getInputNode().isNull())
            {
                input.setInputNode(inPort.getInputNode());
            }

            if(!input.getInternalInputNode().isNull()   &&
               !inPort.getInternalInputNode().isNull()  &&
               input.getInternalInputNode() != inPort.getInternalInputNode())
            {
                qWarning() << "There are two input node ports with the same name: "
                           << input.getName() << ", but with a different internal input node "
                           << "attached, one is: "
                           << input.getInternalInputNode()->getInstanceName() << ", and the other "
                           << "is: " << inPort.getInternalInputNode()->getInstanceName();
                return false;
            }

            if(input.getInternalInputNode().isNull() && !inPort.getInternalInputNode().isNull())
            {
                input.setInternalInputNode(inPort.getInternalInputNode());
            }

            return true;
        }
    }

    // If we are here, it means that the inNodePort doesn't exist in the referencesHolder. Therefore
    // we add it.

    referencesHolder.addInput(inPort);

    return true;
}

bool JsonToNodesParser::loadOutputReferencesHolder(const QJsonObject &nodeObject,
                                                   ReferencesHolder &referencesHolder,
                                                   const NodesSequence *internalNodesSequence,
                                                   const NodesSequence *externalNodesSequence,
                                                   SequenceLoadingErrorHandler *errorHandler)
{
    QJsonArray outputsArray;

    TEST_AND_RETURN_IF_FALSE(JsonHelper::getArray(
                                 nodeObject,
                                 JsonParsersConstants::Sequence::OutputsNameAttribute,
                                 outputsArray),
                             FileLoadingErrorCauseHelper::addUnavailableAttributeError(
                                 JsonParsersConstants::Sequence::OutputsNameAttribute,
                                 errorHandler););

    QJsonArray::const_iterator iter = outputsArray.constBegin();

    for(;iter != outputsArray.constEnd(); ++iter)
    {
        if(!iter->isObject())
        {
            qWarning() << "The outputs array is not well formed";
            return false;
        }

        QJsonObject outputObj = iter->toObject();

        OutNodePort output;

        RETURN_IF_FALSE(loadANodePort(outputObj, output));

        QString varNodeName;

        TEST_AND_RETURN_IF_FALSE(JsonHelper::getString(
                                     outputObj,
                                     JsonParsersConstants::Sequence::VariableNodeNameAttribute,
                                     varNodeName,
                                     true),
                                 FileLoadingErrorCauseHelper::addUnavailableAttributeError(
                                     JsonParsersConstants::Sequence::VariableNodeNameAttribute,
                                     errorHandler););

        if(!varNodeName.isEmpty())
        {
            if(externalNodesSequence == nullptr)
            {
                FileLoadingErrorCauseHelper::addUnavailableExternalNodesSequenceError(varNodeName,
                                                                                      errorHandler);
                qWarning() << "Cannot load the reference node: " << varNodeName << ", the external "
                           << "nodes sequence given is null";
                return false;
            }

            QSharedPointer<VariableNode> varNode =
                                                externalNodesSequence->getVariableNode(varNodeName);

            if(varNode.isNull())
            {
                FileLoadingErrorCauseHelper::addOutputNotFoundError(varNodeName,
                                                                    output.getName(),
                                                                    errorHandler);
                qWarning() << "Can't find the output variable named: " << varNodeName
                           << "for the entry: " << output.getName();
                return false;
            }

            RETURN_IF_FALSE(output.setOutputNode(varNode));
        }

        QString internalVarNodeName;

        TEST_AND_RETURN_IF_FALSE(JsonHelper::getString(
                                     outputObj,
                                     JsonParsersConstants::Sequence::
                                        InternalBindingVarNodeNameAttribute,
                                     internalVarNodeName,
                                     true),
                                 FileLoadingErrorCauseHelper::addUnavailableAttributeError(
                                     JsonParsersConstants::Sequence::VariableNodeNameAttribute,
                                     errorHandler););

        if(!internalVarNodeName.isEmpty())
        {
            if(internalNodesSequence == nullptr)
            {
                FileLoadingErrorCauseHelper::addUnavailableInternalNodesSequenceError(
                                                                                internalVarNodeName,
                                                                                errorHandler);
                qWarning() << "Cannot load the reference node: " << internalVarNodeName << ", the "
                           << "internal nodes sequence given is null";
                return false;
            }

            QSharedPointer<VariableNode> internalVarNode =
                                        internalNodesSequence->getVariableNode(internalVarNodeName);

            if(internalVarNode.isNull())
            {
                FileLoadingErrorCauseHelper::addOutputNotFoundError(varNodeName,
                                                                    output.getName(),
                                                                    errorHandler);
                qWarning() << "Can't find the internal output reference named: "
                           << internalVarNodeName << "for the entry: " << output.getName();
                return false;
            }

            RETURN_IF_FALSE(output.setInternalOutputNode(internalVarNode));
        }

        RETURN_IF_FALSE(addOrUpdateOutputVariables(output, referencesHolder, errorHandler));
    }

    return true;
}

bool JsonToNodesParser::addOrUpdateOutputVariables(const OutNodePort &outPort,
                                                   ReferencesHolder &referencesHolder,
                                                   SequenceLoadingErrorHandler */*errorHandler*/)
{
    PRINT_WARNING(Need to implement the errorHandler in the output variables method)
    for(OutNodePort &output : referencesHolder.accessOutputs())
    {
        if(output.getName() == outPort.getName())
        {
            if(output.getTypeManaged() != outPort.getTypeManaged())
            {
                qWarning() << "There are two output node ports with the same name: "
                           << output.getName() << ", but with a different managed type";
                return false;
            }

            if(output.getUnit() != outPort.getUnit())
            {
                qWarning() << "There are two output node ports with the same name: "
                           << output.getName() << ", but with different units";
                return false;
            }

            if(!output.getOutputNode().isNull()   &&
               !outPort.getOutputNode().isNull()  &&
               output.getOutputNode() != outPort.getOutputNode())
            {
                qWarning() << "There are two output node ports with the same name: "
                           << output.getName() << ", but with a different variable attached, one "
                           << "is: " << output.getOutputNode()->getInstanceName() << ", and the "
                           << "other is: " << outPort.getOutputNode()->getInstanceName();
                return false;
            }

            if(output.getOutputNode().isNull() && !outPort.getOutputNode().isNull())
            {
                output.setOutputNode(outPort.getOutputNode());
            }

            if(!output.getInternalOutputNode().isNull()   &&
               !outPort.getInternalOutputNode().isNull()  &&
               output.getInternalOutputNode() != outPort.getInternalOutputNode())
            {
                qWarning() << "There are two input node ports with the same name: "
                           << output.getName() << ", but with a different reference attached, one "
                           << "is: " << output.getInternalOutputNode()->getInstanceName() << ", "
                           << "and the other is: "
                           << outPort.getInternalOutputNode()->getInstanceName();
                return false;
            }

            if(output.getInternalOutputNode().isNull() && !outPort.getInternalOutputNode().isNull())
            {
                output.setInternalOutputNode(outPort.getInternalOutputNode());
            }

            return true;
        }
    }

    // If we are here, it means that the inNodePort doesn't exist in the referencesHolder. Therefore
    // we add it.

    referencesHolder.addOutput(outPort);

    return true;
}
