// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "errors/helpers/fileloadingerrorhelper.hpp"

#include "types/typemanaged.hpp"


class FileLoadingErrorCauseHelper : public FileLoadingErrorHelper
{
    public:
        /* Values and types */

        /** @brief Used to add an error when a value cannot be read
            @param attributeName The name of the value attribute
            @param errorHandler An ErrorHandler pointer */
        static void addUnavailableValueError(const QString &attributeName,
                                             FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when a value is invalid
            @param valueType The type of the value
            @param attributeName The type of the value
            @param errorHandler An ErrorHandler pointer */
        static void addInvalidValueError(const QString &valueType,
                                         const QString &attributeName,
                                         FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when a value type is unknown
            @param valueType The type of the value
            @param attributeName The type of the value
            @param errorHandler An ErrorHandler pointer */
        static void addUnknownValueTypeError(const QString &valueType,
                                             const QString &attributeName,
                                             FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when the type of a node is unknown
            @param errorHandler An ErrorHandler pointer */
        static void addUnknownNodeTypeError(FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when a type managed is unknown
            @param typeManaged The type managed
            @param errorHandler An ErrorHandler pointer */
        static void addUnknownTypeManagedError(TypeManaged::Enum typeManaged,
                                               FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when a type managed is unknown
            @param typeManaged The type managed in string format
            @param errorHandler An ErrorHandler pointer */
        static void addUnknownTypeManagedError(const QString &typeManagedStr,
                                               FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when a typeManaged cannot be read
            @param errorHandler An ErrorHandler pointer */
        static void addUnavailableNodeTypeError(FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when the parent instance name cannot be read
            @param errorHandler An ErrorHandler pointer */
        static void addUnavailableParentInstanceNameError(FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when an attribute cannot be read
            @param attributeName The name of the attribute
            @param errorHandler An ErrorHandler pointer */
        static void addUnavailableAttributeError(const QString &attributeName,
                                                 FileLoadingErrorHandler *errorHandler);

        /* Units */
        /** @brief Used to add an error when a unit power of ten is not an integer
            @param errorHandler An ErrorHandler pointer */
        static void addUnitPowerOfTenNotIntegerError(FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when a unit power of ten exceeds the authorized limit
            @param errorHandler An ErrorHandler pointer */
        static void addUnitPowerOfTenOverflowError(FileLoadingErrorHandler *errorHandler);

        /* Nodes sequence */
        /** @brief Used to add an error when an internal nodes sequence cannot be read
            @param refNodeName The nodes sequence name
            @param errorHandler An ErrorHandler pointer */
        static void addUnavailableInternalNodesSequenceError(const QString &refNodeName,
                                                             FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when an external nodes sequence cannot be read
            @param refNodeName The nodes sequence name
            @param errorHandler An ErrorHandler pointer */
        static void addUnavailableExternalNodesSequenceError(const QString &refNodeName,
                                                             FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when an internal nodes sequence is missing
            @param refNodeName The nodes sequence name
            @param errorHandler An ErrorHandler pointer */
        static void addInternalNodesSequenceNotFoundError(const QString &refNodeName,
                                                          const QString &inputName,
                                                          FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when a nodes sequence was excepted
                   but a node with an other type is found
            @param nodeName The node found name
            @param errorHandler An ErrorHandler pointer */
        static void addNodeFoundIsNotNodeSequenceError(const QString &nodeName,
                                                       FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when a constant value is set while sequence is running
            @param attributeName The value attribute name
            @param errorHandler An ErrorHandler pointer */
        static void addSetConstantValueWhileSequenceRunningError(
                                                            const QString &attributeName,
                                                            FileLoadingErrorHandler *errorHandler);

        /* Inputs and outputs */

        /** @brief Used to add an error when two outputs with the same name have a different unit
            @param outputName The output name
            @param errorHandler An ErrorHandler pointer */
        static void addtwoOutputsSameNameDifferentUnitError(const QString &outputName,
                                                            FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when two outputs with the same name have a different type
            @param outputName The output name
            @param errorHandler An ErrorHandler pointer */
        static void addTwoOutputsSameNameDifferentTypeError(const QString &outputName,
                                                            FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when two outputs with the same name have are linked to a
                   different variable node
            @param outputName The output name
            @param variableNodeName1 Name of the first variable node
            @param variableNodeName2 Name of the seconde variable node
            @param errorHandler An ErrorHandler pointer */
        static void addTwoOutputsSameNameDifferentVariableError(
                                                            const QString &outputName,
                                                            const QString &variableNodeName1,
                                                            const QString &variableNodeName2,
                                                            FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when two outputs with the same name have are linked to a
                   different reference node
            @param outputName The output name
            @param referenceNodeName1 Name of the first reference node
            @param referenceNodeName2 Name of the seconde reference node
            @param errorHandler An ErrorHandler pointer */
        static void addtwoOutputsSameNameDifferentReferenceError(
                                                            const QString &outputName,
                                                            const QString &ReferenceNodeName1,
                                                            const QString &ReferenceNodeName2,
                                                            FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when the format of an input array is invalid
            @param errorHandler An ErrorHandler pointer **/
        static void addInputsArrayNotWellFormedError(FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when an input node is missing
            @param refNodeName The input node name
            @param inputName The input attribute name
            @param errorHandler An ErrorHandler pointer */
        static void addInputNodeNotFoundError(const QString &refNodeName,
                                              const QString &inputName,
                                              FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when an output node is missing
            @param refNodeName The output node name
            @param outputName The output attribute name
            @param errorHandler An ErrorHandler pointer */
        static void addOutputNotFoundError(const QString &refNodeName,
                                           const QString &outputName,
                                           FileLoadingErrorHandler *errorHandler);

        /* First node */

        /** @brief Used to add an error when the first node found cannot be read
            @param errorHandler An ErrorHandler pointer */
        static void addUnavailableFirstNodeError(FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when the first node has not the compulsory type
            @param firstNodeType Type of the first node
            @param errorHandler An ErrorHandler pointer */
        static void addFirstNodeWrongTypeError(const QString &firstNodeType,
                                               FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when an attribute of the first node cannot be found
            @param attributeName The attribute name
            @param errorHandler An ErrorHandler pointer */
        static void addFirstNodeMissingAttributeError(const QString &attributeName,
                                                      FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when the first node type is unknown
            @param errorHandler An ErrorHandler pointer */
        static void addUnknownFirstNodeTypeError(FileLoadingErrorHandler *errorHandler);

        /* Group node */

        /** @brief Used to add an error when the output of a group node is not a boolean
            @param groupNodeName The groupe node name
            @param outputName The output name
            @param errorHandler An ErrorHandler pointer */
        static void addGroupNodeOutputNotBoolError(const QString &groupNodeName,
                                                   const QString &outputName,
                                                   FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when the compulsary output of a group node is missing
            @param groupNodeName The groupe node name
            @param outputName The output name
            @param attributeName Name of the attribute which imposes the output
            @param errorHandler An ErrorHandler pointer */
        static void addCompulsaryOutputGroupNodeError(const QString &groupNodeName,
                                                      const QString &outputName,
                                                      const QString &attributeName,
                                                      FileLoadingErrorHandler *errorHandler);

        /* Decisional node */

        /** @brief Used to add an error when the decisional operator entered is unknown
            @param deciNodeName The name of the decisional node
            @param decisionalOperator The decisional operator entered
            @param errorHandler An ErrorHandler pointer */
        static void addUnknownDecisionalOperatorError(const QString &deciNodeName,
                                                      const QString &decisionalOperator,
                                                      FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when the attribute 'elements' cannot be converted
            @param errorHandler An ErrorHandler pointer */
        static void addElementsNotObjectsError(FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when the attribute 'elements' cannot be read
            @param errorHandler An ErrorHandler pointer */
        static void addUnavailableElementsError(FileLoadingErrorHandler *errorHandler);

        /* Targeted node */
        /** @brief Used to add an error when A targeted node cannot be read
            @param nodeName The name of the targeted node
            @param errorHandler An ErrorHandler pointer */
        static void addUnavailableTargetedNodeError(const QString &nodeName,
                                                    FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when A node target itself
            @param errorHandler An ErrorHandler pointer */
        static void addNodeCannotTargetItselfError(FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when A targeted node cannot be found
            @param nodeName The name of the targeted node
            @param errorHandler An ErrorHandler pointer */
        static void addTargetedNodeNotFound(const QString &nodeName,
                                            FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when A targeted node cannot be targeted
            @param errorHandler An ErrorHandler pointer */
        static void addNodeCannotBeTargetedError(FileLoadingErrorHandler *errorHandler);

        /* Fork and Join node */

        /** @brief Used to add an error when referenced fork node cannot be found
            @param forkNodeName The fork node name
            @param errorHandler An ErrorHandler pointer */
        static void addForkNodeNotFoundError(const QString &forkNodeName,
                                             FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when A join node is trying to connect
                   with a fork node already linked with a join node
            @param joinNodeName Name of the join node trying to connect with the fork node
            @param joinNodeAlreadyLinkedName Name of the join node alreadyLinked with the fork node
            @param forkNodeName The fork node name
            @param errorHandler An ErrorHandler pointer */
        static void addForkNodeAlreadyLinkedToJoinNodeError(const QString &joinNodeName,
                                                            const QString &joinNodeAlreadyLinkedName,
                                                            const QString &forkNodeName,
                                                            FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when A join node already linked with a fork node
                   is trying to connect with an other fork node
            @param joinNodeName The join node name
            @param forkNodeName The fork node name
            @param errorHandler An ErrorHandler pointer */
        static void addJoinNodeAlreadyLinkedToForkNodeError(const QString &joinNodeName,
                                                            const QString &forkNodeName,
                                                            FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when A node port and a node reference are incompatible
            @param nodePortName The node port name
            @param nodePortTypeManaged The type managed by the node port
            @param nodePortUnit The node port unit
            @param referenceNodeName The reference node name
            @param referenceNodeTypeManaged The type managed by the reference node
            @param referenceNodeUnit The reference node unit
            @param errorHandler An ErrorHandler pointer */
        static void addNodePortAndRefNodeIncompatibleError(
                                                        const QString &nodePortName,
                                                        TypeManaged::Enum nodePortTypeManaged,
                                                        const QString &nodePortUnit,
                                                        const QString &referenceNodeName,
                                                        TypeManaged::Enum referenceNodeTypeManaged,
                                                        const QString &referenceNodeUnit,
                                                        FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when a join node cannot be found
            @param joinNodeName The join node name
            @param errorHandler An Errorhandler pointer */
        static void addJoinNodeNotFoundError(const QString &joinNodeName,
                                             FileLoadingErrorHandler *errorHandler);

        /* Module node */

        /** @brief Used to add an error when the bank cannot be found
            @param bankName The bank name
            @param sequenceModuleName The sequence module name
            @param errorHandler An Errorhandler pointer */
        static void addBankNotFoundError(const QString &bankName,
                                         const QString &sequenceModuleName,
                                         FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when the bank cannot be found
            @param sequenceModuleName The sequence module name
            @param bankName The bank name
            @param errorHandler An Errorhandler pointer */
        static void addCannotCreateSequenceModuleInBank(const QString &sequenceModuleName,
                                                        const QString &bankName,
                                                        FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when a module name cannot be found
            @param nodeName The sequence module node name
            @param errorHandler An Errorhandler pointer */
        static void addUnavailableModuleNameError(const QString &nodeName,
                                                  FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when a module name cannot be found in a sequence module node
            @param nodeName The sequence module node name
            @param errorHandler An Errorhandler pointer */
        static void addUnavailableBankNameError(const QString &nodeName,
                                                FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when a bank name cannot be found in a sequence module node
            @param nodeName The sequence module node name
            @param errorHandler An Errorhandler pointer */
        static void addUnavailableBankInterfaceError(const QString &nodeName,
                                                     FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when a bank name cannot be found in a sequence module node
            @param errorHandler An Errorhandler pointer */
        static void addUnavailableModuleParamsFormatError(FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when module parameters cannot be read
                   in a sequence module node
            @param errorHandler An Errorhandler pointer */
        static void addUnavailableModuleParamsError(FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when module parameters cannot be set
                   in a sequence module node
            @param errors A vector containing error information collected in the banks
            @param errorHandler An Errorhandler pointer */
        static void addCannotSetModuleParamsError(const QVector<QVariant> &errors,
                                                  FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when a module attribute cannot be read
                   in a sequence module node
            @param attributeName The attribute name
            @param errorHandler An Errorhandler pointer**/
        static void addUnavailableModuleAttributeError(const QString &attributeName,
                                                       FileLoadingErrorHandler *errorHandler);

        /* Transition */

        /** @brief Used to add an error when A transition cannot be read
            @param errorHandler An ErrorHandler pointer **/
        static void addUnavailableTransitionError(FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when A transition is invalid
            @param errorHandler An ErrorHandler pointer **/
        static void addInvalidTransitionError(FileLoadingErrorHandler *errorHandler);

        /* File */
        /** @brief Used to add an error when a unit suffix is unknown
            @param errorHandler An ErrorHandler pointer **/
        static void addSuffixError(FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when sequenceFile can't be found
            @param errorHandler An ErrorHandler pointer **/
        static void addMissingSequenceFileError(const QString &filePath,
                                                FileLoadingErrorHandler *errorHandler);
};
