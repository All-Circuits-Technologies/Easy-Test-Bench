// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "errors/sequenceloadingerrorhandler.hpp"

#include "errors/sequenceerror.hpp"


SequenceLoadingErrorHandler::SequenceLoadingErrorHandler(QObject *parent) :
FileLoadingErrorHandler(parent)
{
}

QString SequenceLoadingErrorHandler::getDescriptorMessage(const ErrorDescriptor &descriptor)
{
    QString message = FileLoadingErrorHandler::getDescriptorMessage(descriptor);

    if(!message.isEmpty())
    {
        return message;
    }

    switch(descriptor.layer)
    {
        case ErrorLayer::Enum::FailedChildNode :
            message = SequenceError::getTrKey(static_cast<SequenceError::Enum>(descriptor.error));
            putMessageParameters(descriptor.parameters, message);
            break;

        case ErrorLayer::Enum::FailedNodesSequence :
            message = SequenceError::getTrKey(static_cast<SequenceError::Enum>(descriptor.error));
            putMessageParameters(descriptor.parameters, message);
            break;

        default:
            break;
    }

    return message;
}
