// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <qobject.h>


/** @brief List of file loading error causes **/
class ErrorCause : QObject
{
        Q_OBJECT

    public:
        enum Enum
        {
            /* Values and types */
            InvalidValue ,
            UnavailableAttribute,
            UnknownTypeManaged,
            UnavailableNodeType,
            UnavailableParentInstanceName,
            UnavailableValue,
            UnknownValueType,

            /* Units */
            UnitPowerOfTenNotInteger,
            UnitPowerOfTenOverflow,

            /* Inputs and outputs */
            InputsArrayNotWellFormed,
            InputNodeNotFound,
            OutputNodeNotFound,
            TwoOutputsSameNameDifferentType,
            TwoOutputsSameNameDifferentUnit,
            TwoOutputsSameNameDifferentVariable,
            TwoOutputsSameNameDifferentReference,

            /* Nodes sequence */
            NodeFoundIsNotNodeSequence,
            SetConstantValueWhileSequenceRunningError ,
            UnavailableInternalNodesSequence,
            UnavailableExternalNodesSequence,
            UnavailableExternalNode,

            /* First node */
            FirstNodeMissingAttribute,
            FirstNodeWrongType,
            UnavailableFirstNode,
            UnknownFirstNodetype,

            /* Group node */
            CompulsaryOutputGroupNode,
            GroupNodeOutputNotBool,

            /* Decisional node */
            ElementsNotObjects,
            UnavailableElements,
            UnknownDecisionalOperator,

            /* Targeted node */
            NodeCannotBeTargeted,
            NodeCannotTargetItself,
            TargetedNodeNotFound,
            UnavailableTargetedNode,

            /* Fork and Join node */
            ForkNodeAlreadyLinkedToJoinNode,
            ForkNodeNotFound,
            JoinNodeAlreadyLinkedToForkNode,
            JoinNodeNotFound,
            NodePortAndRefNodeIncompatible,

            /* Module node */
            BankNotFound,
            CannotCreateSequenceModuleInBank,
            CannotSetModuleParamsError,
            UnavailableBankInterface,
            UnavailableBankName,
            UnavailableModuleName,
            UnavailableModuleParamsFormat,
            UnavailableModuleParams,
            UnavailableModuleAttribute,

            /* Transition */
            InvalidTransition,
            UnavailableTransition,

            /* File */
            SuffixError,
            MissingSequenceFileError,

            /* Unknown */
            UnknownNodeType
        };

    public:
        /** @brief Return a translation key associated to an error cause
            @param errorCause An enum value related to the error cause
            @return A translation key **/
        static const char* getTrKey(ErrorCause::Enum errorCause);

    private:
        static const constexpr char* UnknownErrorCauseKey = QT_TR_NOOP("(TR) Unknown error cause");
        static const constexpr char* TwOutputsSameNameDifferentTypeKey =
                                                            QT_TR_NOOP("(TR) There are two output "
                                                                       "node ports with the same "
                                                                       "name: '%1', but with a "
                                                                       "different managed type");
        static const constexpr char* TwOutputsSameNameDifferentUnitKey =
                                                            QT_TR_NOOP("(TR) There are two output "
                                                                       "node ports with the same "
                                                                       "name: '%1', but with "
                                                                       "different units");
        static const constexpr char* TwOutputsSameNameDifferentVariableKey =
                                                            QT_TR_NOOP("(TR) There are two output "
                                                                       "node ports with the same "
                                                                       "name: '%1', but with a "
                                                                       "different variable "
                                                                       "attached, one is: '%2' , "
                                                                       "and the other is: '%3'");
        static const constexpr char* TwOutputsSameNameDifferentReferenceKey =
                                                            QT_TR_NOOP("(TR) There are two input "
                                                                       "node ports with the same "
                                                                       "name: '%1, but with a "
                                                                       "different reference "
                                                                       "attached, one is: '%2, and "
                                                                       "the other is: '%3'");
        static const constexpr char* JoinNodeNotFoundKey = QT_TR_NOOP("(TR) The join node : '%1' "
                                                                      "does not exist or is not a "
                                                                      "join node");
        static const constexpr char* UnvailableFirstNodeKey =
                                                            QT_TR_NOOP("(TR) First node is "
                                                                       "unavailable because one "
                                                                       "element of the sequence "
                                                                       "file isn't well formed");
        static const constexpr char* FirstNodeMissingAttributeKey =
                                                            QT_TR_NOOP("(TR) The attribute : '%1' "
                                                                       "of the first node is "
                                                                       "missing");
        static const constexpr char* UnknownFirstNodetypeKey = QT_TR_NOOP("(TR) First node has a "
                                                                          "unknown type");
        static const constexpr char* InvalidTransitionKey = QT_TR_NOOP("(TR) Invalid transition");
        static const constexpr char* UnavailableTargetedNodeKey = QT_TR_NOOP("(TR) Targeted node "
                                                                             "unavailable");
        static const constexpr char* NodeCannotBeTargetedKey = QT_TR_NOOP("(TR) A referenced node "
                                                                          "can't be targeted");
        static const constexpr char* UnavailableElementsKey = QT_TR_NOOP("(TR) Elements "
                                                                         "unavailable");
        static const constexpr char* UnknownDecisionalOperatorKey =
                                                            QT_TR_NOOP("(TR) In the node: '%1', "
                                                                       "the decisional operator: "
                                                                       "'%2' is not known");
        static const constexpr char* UnavailableTransitionKey = QT_TR_NOOP("(TR) Transition "
                                                                           "unavailable");
        static const constexpr char* TargetedNodeNotFoundKey = QT_TR_NOOP("(TR) The node targeted: "
                                                                          "'%1', doesn't exist");
        static const constexpr char* CannotSetModuleParamsErrorKey = QT_TR_NOOP("(TR) Module "
                                                                                "Parameters "
                                                                                "error : %1");
        static const constexpr char* UnavailableModuleParamsFormatKey =
                                                            QT_TR_NOOP("(TR) Module parameters "
                                                                       "format unavailable");
        static const constexpr char* UnavailableModuleParamsKey = QT_TR_NOOP("(TR) Module "
                                                                             "parameters "
                                                                             "unavailable");
        static const constexpr char* UnavailableModuleAttributeKey =
                                                            QT_TR_NOOP("(TR) Module attribute : "
                                                                       "'%1' unavailable");
        static const constexpr char* UnavailableBankInterfaceKey =
                                                            QT_TR_NOOP("(TR) The bank interface "
                                                                       "linked to the node : '%1' "
                                                                       "is unavailable");
        static const constexpr char* UnavailableBankNameKey = QT_TR_NOOP("(TR) Can't get the name "
                                                                         "of the bank interface "
                                                                         "linked to the node: "
                                                                         "'%1'");
        static const constexpr char* UnavailableModuleNameKey = QT_TR_NOOP("(TR) Can't get the "
                                                                           "name of the module "
                                                                           "linked to the node : "
                                                                           "'%1'");
        static const constexpr char* CannotCreateSequenceModuleInBankKey =
                                                            QT_TR_NOOP("(TR) Can't create the "
                                                                       "sequence module instance "
                                                                       "named : '%1' , in '%2'");
        static const constexpr char* BankNotFoundKey = QT_TR_NOOP("(TR) The bank : '%1' , linked "
                                                                  "to the sequence module : '%2', "
                                                                  "doesn't exist");
        static const constexpr char* ForkNodeNotFoundKey = QT_TR_NOOP("(TR) The fork node: '%1' "
                                                                      "doesn't exist or it's not a "
                                                                      "fork node");
        static const constexpr char* ForkNodeAlreadyLinkedToJoinNodeKey =
                                                            QT_TR_NOOP("(TR) The JoinNode: '%1', "
                                                                       "is linked to the ForkNode: "
                                                                       "'%2', but the ForkNode has "
                                                                       "already been connected to "
                                                                       "the JoinNode: '%3'");
        static const constexpr char* JoinNodeAlreadyLinkedToForkNodeKey =
                                                            QT_TR_NOOP("(TR) The JoinNode: '%1', "
                                                                       "is already linked to the "
                                                                       "ForkNode: '%2'");
        static const constexpr char* ElementsNotObjectsKey = QT_TR_NOOP("(TR) Some node elements "
                                                                        "in the file are not "
                                                                        "objects");
        static const constexpr char* NodeFoundIsNotNodeSequenceKey =
                                                            QT_TR_NOOP("(TR) The node found is not "
                                                                       "a sequence node: '%1'");
        static const constexpr char* UnavailableAttributeKey = QT_TR_NOOP("(TR) Attribute : '%1' "
                                                                          "unavailable");
        static const constexpr char* NodeCannotTargetItselfkey = QT_TR_NOOP("(TR) A node can't "
                                                                            "target itself");
        static const constexpr char* UnavailableNodeTypeKey = QT_TR_NOOP("(TR) The type of a node "
                                                                         "is unavailable");
        static const constexpr char* GroupNodeOutputNotBoolKey = QT_TR_NOOP("(TR) Error with the "
                                                                            "output: '%1', of the "
                                                                            "group node:'%2', it's "
                                                                            "not a boolean");
        static const constexpr char* CompulsaryOutputGroupNodeKey =
                                                            QT_TR_NOOP("(TR) The output: %1, is "
                                                                       "compulsory in the group "
                                                                       "node: '%2', because the "
                                                                       "attribute: '%3', is equal "
                                                                       "to false");
        static const constexpr char* UnavailableInternalNodesSequenceKey =
                                                            QT_TR_NOOP("(TR) Internal nodes "
                                                                       "Sequence: '%1' "
                                                                       "unavailable");
        static const constexpr char* UnavailableExternalNodesSequenceKey =
                                                            QT_TR_NOOP("(TR) Internal nodes "
                                                                       "Sequence: '%1' "
                                                                       "unavailable");
        static const constexpr char* InternalNodesSequenceNotFoundKey =
                                                            QT_TR_NOOP("(TR) Internal nodes "
                                                                       "Sequence: '%1' not found "
                                                                       "at entry: '%2'");
        static const constexpr char* UnavailableInternalRefNodeNameKey =
                                                            QT_TR_NOOP("(TR) Internal reference "
                                                                       "node name unavailable");
        static const constexpr char* OutputNodeNotFoundKey = QT_TR_NOOP("(TR) Can't find the "
                                                                        "output named: '%1'for the "
                                                                        "entry: '%2'");
        static const constexpr char* InputNodeNotFoundKey = QT_TR_NOOP("(TR) Can't find the input "
                                                                       "variable named: '%1'for "
                                                                       "the entry: '%2'");
        static const constexpr char* UnavailableRefNodeNameKey =
                                                            QT_TR_NOOP("(TR) The node port: '%1' "
                                                                       "(type: '%2', unit: '%3' ), "
                                                                       "and the reference node: "
                                                                       "'%4 (type: '%5', unit: "
                                                                       "'%6'), are incompatible");
        static const constexpr char* NodePortAndRefNodeIncompatibleKey =
                                                            QT_TR_NOOP("(TR) Reference node name "
                                                                       "unavailable");
        static const constexpr char* UnavailableExternalNodeKey =
                                                            QT_TR_NOOP("(TR) External node: '%1' "
                                                                       "unavailable");
        static const constexpr char* ExternalNodeNotFoundKey =
                                                            QT_TR_NOOP("(TR) Can't find the node "
                                                                       "reference named: '%1' for "
                                                                       "the entry : '%2'");
        static const constexpr char* UnavailableParentInstanceNameKey =
                                                            QT_TR_NOOP("(TR) Parent instance name "
                                                                       "unavailable");
        static const constexpr char* UnknownValueTypeKey = QT_TR_NOOP("(TR) Attribute : '%2' has "
                                                                      "an unknown type '%1'");
        static const constexpr char* UnknownTypeManagedKey =
                                                            QT_TR_NOOP("(TR) Unknown type managed: "
                                                                       "'%1'");
        static const constexpr char* InvalidValueKey = QT_TR_NOOP("(TR) Value of type: '%1', "
                                                                  "unavailable at attribute: '%2'");
        static const constexpr char* UnavailableValueKey = QT_TR_NOOP("(TR) Value unavailable at "
                                                                      "attribute: '%1'");
        static const constexpr char* SetConstantValueWhileSequenceRunningErrorKey =
                                                            QT_TR_NOOP("(TR) Value : '%1' can't be "
                                                                       "set while the sequence is "
                                                                       "running");
        static const constexpr char* UnitPowerOfTenOverflowKey = QT_TR_NOOP("(TR) Unit power of "
                                                                            "ten overflow the "
                                                                            "limit");
        static const constexpr char* UnitPowerOfTenNotIntegerDesc =
                                                            QT_TR_NOOP("(TR) Unit power of ten is "
                                                                       "too big or is not an "
                                                                       "integer");
        static const constexpr char* UnknownNodeTypeKey = QT_TR_NOOP("(TR) Unknown node type");
        static const constexpr char* FirstNodeWrongTypeKey = QT_TR_NOOP("(TR) The sequence file "
                                                                        "doesn't contain the "
                                                                        "compulsory node: '%1'");
        static const constexpr char* InputsArrayNotWellFormedKey =
                                                            QT_TR_NOOP("(TR) Inputs array is not "
                                                                       "well formed");
        static const constexpr char* SuffixErrorKey = QT_TR_NOOP("(TR) The file suffix isn't "
                                                                 "known");
        static const constexpr char* MissingSequenceFileErrorKey =
                                                            QT_TR_NOOP("(TR) The sequence file "
                                                                       "can't be found at : %1");
};
