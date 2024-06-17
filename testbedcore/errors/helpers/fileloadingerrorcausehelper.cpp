// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "errors/helpers/fileloadingerrorcausehelper.hpp"

#include <QVariant>
#include <QDebug>

#include "definesutility/definesutility.hpp"
#include "errors/errorcause.hpp"
#include "testbedcore/parsers/jsonparsersconstants.hpp"


void FileLoadingErrorCauseHelper::addUnavailableValueError(const QString &attributeName,
                                                           FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::UnavailableValue,
                       *errorHandler,
                       { attributeName });
}

void FileLoadingErrorCauseHelper::addInvalidValueError(const QString &valueType,
                                                       const QString &attributeName,
                                                       FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::InvalidValue,
                       *errorHandler,
                       { valueType, attributeName });
}

void FileLoadingErrorCauseHelper::addUnknownValueTypeError(const QString &valueType,
                                                           const QString &attributeName,
                                                           FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::UnknownValueType,
                       *errorHandler,
                       { valueType, attributeName });
}

void FileLoadingErrorCauseHelper::addUnknownNodeTypeError(FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::UnknownNodeType,
                       *errorHandler);
}

void FileLoadingErrorCauseHelper::addUnknownTypeManagedError(const QString &typeManagedStr,
                                                             FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::UnknownTypeManaged,
                       *errorHandler,
                       { typeManagedStr });
}

void FileLoadingErrorCauseHelper::addUnavailableNodeTypeError(FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::UnavailableNodeType,
                       *errorHandler);
}

void FileLoadingErrorCauseHelper::addUnavailableParentInstanceNameError(
                                                            FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::UnavailableParentInstanceName,
                       *errorHandler);
}

void FileLoadingErrorCauseHelper::addUnavailableAttributeError(
                                                            const QString &attributeName,
                                                            FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::UnavailableAttribute,
                       *errorHandler,
                       { attributeName });

}

void FileLoadingErrorCauseHelper::addUnavailableModuleAttributeError(
                                                            const QString &attributeName,
                                                            FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::UnavailableModuleAttribute,
                       *errorHandler,
                       { attributeName });
}

void FileLoadingErrorCauseHelper::addCannotSetModuleParamsError(
                                                            const QVector<QVariant> &errors,
                                                            FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    if(errors.isEmpty())
    {
        qWarning() << "Plugin error doesn't contain an hash table as first element";
        return;
    }

    QHash<QString, QVariant> pluginError = errors.at(0).toHash();

    if(!pluginError.contains(JsonParsersConstants::Hmi::ErrorLabelAttribute))
    {
        qWarning() << "Plugin error hash table doesn't contain an element associated "
                      "with the key : " << JsonParsersConstants::Hmi::ErrorLabelAttribute;
        return;
    }

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::CannotSetModuleParamsError,
                       *errorHandler,
                       { pluginError[JsonParsersConstants::Hmi::ErrorLabelAttribute] });
}

void FileLoadingErrorCauseHelper::addTargetedNodeNotFound(const QString &nodeName,
                                                          FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::TargetedNodeNotFound,
                       *errorHandler,
                       { nodeName });
}

void FileLoadingErrorCauseHelper::addUnavailableTransitionError(
        FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::UnavailableTransition,
                       *errorHandler);
}

void FileLoadingErrorCauseHelper::addUnavailableInternalNodesSequenceError(
                                                            const QString &refNodeName,
                                                            FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::UnavailableInternalNodesSequence,
                       *errorHandler,
                       { refNodeName });
}

void FileLoadingErrorCauseHelper::addUnavailableExternalNodesSequenceError(
                                                            const QString &refNodeName,
                                                            FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::UnavailableExternalNodesSequence,
                       *errorHandler,
                       { refNodeName });
}

void FileLoadingErrorCauseHelper::addOutputNotFoundError(const QString &refNodeName,
                                                         const QString &outputName,
                                                         FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::OutputNodeNotFound,
                       *errorHandler,
                       { refNodeName, outputName });
}

void FileLoadingErrorCauseHelper::addInputNodeNotFoundError(const QString &refNodeName,
                                                            const QString &inputName,
                                                            FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::InputNodeNotFound,
                       *errorHandler,
                       { refNodeName, inputName });
}

void FileLoadingErrorCauseHelper::addUnknownDecisionalOperatorError(
                                                            const QString &deciNodeName,
                                                            const QString &decisionalOperator,
                                                            FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::UnknownDecisionalOperator,
                       *errorHandler,
                       { deciNodeName, decisionalOperator });
}

void FileLoadingErrorCauseHelper::addGroupNodeOutputNotBoolError(
                                                            const QString &groupNodeName,
                                                            const QString &outputName,
                                                            FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::GroupNodeOutputNotBool,
                       *errorHandler,
                       { groupNodeName, outputName });
}

void FileLoadingErrorCauseHelper::addCompulsaryOutputGroupNodeError(
                                                            const QString &groupNodeName,
                                                            const QString &outputName,
                                                            const QString &attributeName,
                                                            FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::CompulsaryOutputGroupNode,
                       *errorHandler,
                       { outputName, groupNodeName, attributeName });
}

void FileLoadingErrorCauseHelper::addBankNotFoundError(const QString &bankName,
                                                       const QString &sequenceModuleName,
                                                       FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::BankNotFound,
                       *errorHandler,
                       { bankName, sequenceModuleName });
}

void FileLoadingErrorCauseHelper::addCannotCreateSequenceModuleInBank(
                                                            const QString &sequenceModuleName,
                                                            const QString &bankName,
                                                            FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::CannotCreateSequenceModuleInBank,
                       *errorHandler,
                       { bankName, sequenceModuleName });
}

void FileLoadingErrorCauseHelper::addUnavailableBankInterfaceError(
                                                            const QString &nodeName,
                                                            FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::UnavailableBankInterface,
                       *errorHandler,
                       { nodeName });
}

void FileLoadingErrorCauseHelper::addUnavailableModuleParamsFormatError(
                                                            FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::UnavailableModuleParamsFormat,
                       *errorHandler);
}

void FileLoadingErrorCauseHelper::addUnavailableModuleParamsError(
                                                            FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause, ErrorCause::UnavailableModuleParams, *errorHandler);
}

void FileLoadingErrorCauseHelper::addUnavailableBankNameError(const QString &nodeName,
                                                              FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::UnavailableBankName,
                       *errorHandler,
                       { nodeName });
}

void FileLoadingErrorCauseHelper::addUnavailableModuleNameError(
                                                            const QString &nodeName,
                                                            FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::UnavailableModuleName,
                       *errorHandler,
                       { nodeName });
}

void FileLoadingErrorCauseHelper::addForkNodeAlreadyLinkedToJoinNodeError(
                                                        const QString &joinNodeName,
                                                        const QString &joinNodeAlreadyLinkedName,
                                                        const QString &forkNodeName,
                                                        FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::ForkNodeAlreadyLinkedToJoinNode,
                       *errorHandler,
                       { joinNodeName, forkNodeName, joinNodeAlreadyLinkedName });
}

void FileLoadingErrorCauseHelper::addJoinNodeAlreadyLinkedToForkNodeError(
                                                            const QString &joinNodeName,
                                                            const QString &forkNodeName,
                                                            FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::JoinNodeAlreadyLinkedToForkNode,
                       *errorHandler,
                       { joinNodeName, forkNodeName });
}

void FileLoadingErrorCauseHelper::addUnitPowerOfTenOverflowError(
        FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::UnitPowerOfTenOverflow,
                       *errorHandler);
}

void FileLoadingErrorCauseHelper::addSuffixError(FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::SuffixError,
                       *errorHandler);
}

void FileLoadingErrorCauseHelper::addMissingSequenceFileError(const QString &filePath,
                                                              FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    QVector<QVariant> parameters;
    parameters.append(filePath);
    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::MissingSequenceFileError,
                       *errorHandler,
                       parameters);
}

void FileLoadingErrorCauseHelper::addInputsArrayNotWellFormedError(
        FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::InputsArrayNotWellFormed,
                       *errorHandler);
}

void FileLoadingErrorCauseHelper::addUnitPowerOfTenNotIntegerError(
        FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::UnitPowerOfTenNotInteger,
                       *errorHandler);
}

void FileLoadingErrorCauseHelper::addElementsNotObjectsError(FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::ElementsNotObjects,
                       *errorHandler);
}

void FileLoadingErrorCauseHelper::addForkNodeNotFoundError(const QString &forkNodeName,
                                                           FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::ForkNodeNotFound,
                       *errorHandler,
                       { forkNodeName });
}

void FileLoadingErrorCauseHelper::addUnknownTypeManagedError(TypeManaged::Enum typeManaged,
                                                             FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::UnknownTypeManaged,
                       *errorHandler,
                       { TypeManaged::toString(typeManaged) });
}

void FileLoadingErrorCauseHelper::addFirstNodeWrongTypeError(const QString &firstNodeType,
                                                             FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::FirstNodeWrongType,
                       *errorHandler,
                       { firstNodeType });
}

void FileLoadingErrorCauseHelper::addNodeCannotTargetItselfError(
        FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::NodeCannotTargetItself,
                       *errorHandler);
}

void FileLoadingErrorCauseHelper::addSetConstantValueWhileSequenceRunningError(
                                                            const QString &attributeName,
                                                            FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::SetConstantValueWhileSequenceRunningError,
                       *errorHandler,
                       { attributeName });
}

void FileLoadingErrorCauseHelper::addNodeFoundIsNotNodeSequenceError(const QString &nodeName,
        FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::NodeFoundIsNotNodeSequence,
                       *errorHandler,
                       { nodeName });
}

void FileLoadingErrorCauseHelper::addUnavailableElementsError(FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::UnavailableElements,
                       *errorHandler);
}

void FileLoadingErrorCauseHelper::addUnavailableTargetedNodeError(
                                                            const QString &nodeName,
                                                            FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::UnavailableTargetedNode,
                       *errorHandler,
                       { nodeName });
}

void FileLoadingErrorCauseHelper::addNodeCannotBeTargetedError(
        FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::NodeCannotBeTargeted,
                       *errorHandler);
}

void FileLoadingErrorCauseHelper::addInvalidTransitionError(FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::InvalidTransition,
                       *errorHandler);
}

void FileLoadingErrorCauseHelper::addNodePortAndRefNodeIncompatibleError(
                                                        const QString &nodePortName,
                                                        TypeManaged::Enum nodePortTypeManaged,
                                                        const QString &nodePortUnit,
                                                        const QString &referenceNodeName,
                                                        TypeManaged::Enum referenceNodeTypeManaged,
                                                        const QString &referenceNodeUnit,
                                                        FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::NodePortAndRefNodeIncompatible,
                       *errorHandler,
                       { nodePortName,
                         TypeManaged::toString(nodePortTypeManaged),
                         nodePortUnit,
                         referenceNodeName,
                         referenceNodeTypeManaged,
                         referenceNodeUnit });
}

void FileLoadingErrorCauseHelper::addJoinNodeNotFoundError(const QString &joinNodeName,
                                                           FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::JoinNodeNotFound,
                       *errorHandler,
                       { joinNodeName });
}

void FileLoadingErrorCauseHelper::addUnavailableFirstNodeError(
        FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::UnavailableFirstNode,
                       *errorHandler);
}

void FileLoadingErrorCauseHelper::addUnknownFirstNodeTypeError(
        FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::UnknownFirstNodetype,
                       *errorHandler);
}

void FileLoadingErrorCauseHelper::addFirstNodeMissingAttributeError(
                                                            const QString &attributeName,
                                                            FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::FirstNodeMissingAttribute,
                       *errorHandler,
                       { attributeName });
}

void FileLoadingErrorCauseHelper::addtwoOutputsSameNameDifferentUnitError(
                                                            const QString &outputName,
                                                            FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::TwoOutputsSameNameDifferentUnit,
                       *errorHandler,
                       { outputName });
}

void FileLoadingErrorCauseHelper::addTwoOutputsSameNameDifferentTypeError(
                                                            const QString &outputName,
                                                            FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::TwoOutputsSameNameDifferentType,
                       *errorHandler,
                       { outputName });
}

void FileLoadingErrorCauseHelper::addTwoOutputsSameNameDifferentVariableError(
                                                            const QString &outputName,
                                                            const QString &variableNodeName1,
                                                            const QString &variableNodeName2,
                                                            FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::TwoOutputsSameNameDifferentVariable,
                       *errorHandler,
                       { outputName, variableNodeName1, variableNodeName2 });
}

void FileLoadingErrorCauseHelper::addtwoOutputsSameNameDifferentReferenceError(
                                                            const QString &outputName,
                                                            const QString &ReferenceNodeName1,
                                                            const QString &ReferenceNodeName2,
                                                            FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorCause,
                       ErrorCause::TwoOutputsSameNameDifferentReference,
                       *errorHandler,
                       { outputName, ReferenceNodeName1, ReferenceNodeName2 });
}
