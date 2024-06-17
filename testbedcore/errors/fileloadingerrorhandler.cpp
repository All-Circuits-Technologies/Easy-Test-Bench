// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "fileloadingerrorhandler.hpp"

#include "errorcause.hpp"
#include "errortype.hpp"


FileLoadingErrorHandler::FileLoadingErrorHandler(QObject *parent) :
    QObject(parent)
{
}

QStringList FileLoadingErrorHandler::getMessages()
{
    QStringList messages;

    for(auto citer = _errorDescriptors.cbegin(); citer != _errorDescriptors.cend(); ++citer)
    {
        messages.append(getDescriptorMessage(*citer));
    }

    return messages;
}

void FileLoadingErrorHandler::addErrorDescriptor(const ErrorDescriptor &errorDescriptor)
{
    _errorDescriptors.append(errorDescriptor);
}

QString FileLoadingErrorHandler::getDescriptorMessage(const ErrorDescriptor& descriptor)
{
    QString message = QString("");

    switch(descriptor.layer)
    {
        case ErrorLayer::Enum::ErrorCause :
            message = tr(ErrorCause::getTrKey(static_cast<ErrorCause::Enum>(descriptor.error)));
            putMessageParameters(descriptor.parameters, message);
            break;

        case ErrorLayer::Enum::ErrorType :
            message = tr(ErrorType::getTrKey(static_cast<ErrorType::Enum>(descriptor.error)));
            putMessageParameters(descriptor.parameters, message);
            break;

        default:
            break;
    }

    return message;
}

void FileLoadingErrorHandler::putMessageParameters(const QVector<QVariant> &parameters,
                                                   QString &message)
{
    for(const QVariant &parameter : parameters)
    {
        message = message.arg(parameter.toString());
    }
}

void FileLoadingErrorHandler::clearErrors()
{
    _errorDescriptors.clear();
}
