// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "errors/errorcause.hpp"

#include <QDebug>


const char *ErrorCause::getTrKey(ErrorCause::Enum errorCause)
{
    switch(errorCause)
    {
        case UnavailableAttribute:
            return UnavailableAttributeKey;

        case UnavailableParentInstanceName:
            return UnavailableParentInstanceNameKey;

        case UnknownTypeManaged:
            return UnknownTypeManagedKey;

        case UnknownValueType:
            return UnknownValueTypeKey;

        case UnavailableValue:
            return UnavailableValueKey;

        case UnavailableExternalNode:
            return UnavailableExternalNodeKey;

        case InvalidValue:
            return InvalidValueKey;

        case CannotSetModuleParamsError:
            return CannotSetModuleParamsErrorKey;

        case SetConstantValueWhileSequenceRunningError:
            return SetConstantValueWhileSequenceRunningErrorKey;

        case UnitPowerOfTenNotInteger:
            return UnitPowerOfTenNotIntegerDesc;

        case UnitPowerOfTenOverflow :
            return UnitPowerOfTenOverflowKey;

        case InputsArrayNotWellFormed:
            return InputsArrayNotWellFormedKey;

        case NodePortAndRefNodeIncompatible:
            return NodePortAndRefNodeIncompatibleKey;

        case UnavailableInternalNodesSequence:
            return UnavailableInternalNodesSequenceKey;

        case UnavailableExternalNodesSequence:
            return UnavailableExternalNodesSequenceKey;

        case GroupNodeOutputNotBool:
            return GroupNodeOutputNotBoolKey;

        case CompulsaryOutputGroupNode:
            return CompulsaryOutputGroupNodeKey;

        case UnavailableNodeType:
            return UnavailableNodeTypeKey;

        case NodeCannotTargetItself:
            return NodeCannotTargetItselfkey;

        case ElementsNotObjects:
            return ElementsNotObjectsKey;

        case ForkNodeNotFound:
            return ForkNodeNotFoundKey;

        case ForkNodeAlreadyLinkedToJoinNode:
            return ForkNodeAlreadyLinkedToJoinNodeKey;

        case BankNotFound:
            return BankNotFoundKey;

        case TargetedNodeNotFound:
            return TargetedNodeNotFoundKey;

        case CannotCreateSequenceModuleInBank:
            return CannotCreateSequenceModuleInBankKey;

        case UnavailableBankInterface:
            return UnavailableBankInterfaceKey;

        case UnavailableModuleName:
            return UnavailableModuleNameKey;

        case UnavailableBankName:
            return UnavailableBankNameKey;

        case UnavailableModuleParamsFormat:
            return UnavailableModuleParamsFormatKey;

        case UnavailableModuleParams:
            return UnavailableModuleParamsKey;

        case UnavailableModuleAttribute:
            return UnavailableModuleAttributeKey;

        case UnavailableTransition:
            return UnavailableTransitionKey;

        case UnknownDecisionalOperator:
            return UnknownDecisionalOperatorKey;

        case UnavailableElements:
            return UnavailableElementsKey;

        case UnavailableTargetedNode:
            return UnavailableTargetedNodeKey;

        case NodeCannotBeTargeted:
            return NodeCannotBeTargetedKey;

        case InvalidTransition:
            return InvalidTransitionKey;

        case UnavailableFirstNode:
            return UnvailableFirstNodeKey;

        case FirstNodeMissingAttribute:
            return FirstNodeMissingAttributeKey;

        case UnknownFirstNodetype:
            return UnknownFirstNodetypeKey;

        case FirstNodeWrongType:
            return FirstNodeWrongTypeKey;

        case JoinNodeAlreadyLinkedToForkNode:
            return JoinNodeAlreadyLinkedToForkNodeKey;

        case JoinNodeNotFound:
            return JoinNodeNotFoundKey;

        case NodeFoundIsNotNodeSequence:
            return NodeFoundIsNotNodeSequenceKey;

        case InputNodeNotFound:
            return InputNodeNotFoundKey;

        case OutputNodeNotFound:
            return OutputNodeNotFoundKey;

        case TwoOutputsSameNameDifferentUnit:
            return TwOutputsSameNameDifferentUnitKey;

        case TwoOutputsSameNameDifferentType:
            return TwOutputsSameNameDifferentTypeKey;

        case TwoOutputsSameNameDifferentVariable:
            return TwOutputsSameNameDifferentVariableKey;

        case TwoOutputsSameNameDifferentReference:
            return TwOutputsSameNameDifferentReferenceKey;

        case SuffixError:
            return SuffixErrorKey;

        case MissingSequenceFileError:
            return MissingSequenceFileErrorKey;

        case UnknownNodeType:
            break;
    }

    qWarning() << "This kind of error is not managed";
    return UnknownErrorCauseKey;
}
