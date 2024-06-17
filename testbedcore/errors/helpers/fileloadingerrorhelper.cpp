// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "errors/helpers/fileloadingerrorhelper.hpp"

#include "errors/errordescriptor.hpp"
#include "threadutility/concurrent/threadconcurrentrun.hpp"


void FileLoadingErrorHelper::addErrorDescriptor(ErrorLayer::Enum layer,
                                                int error,
                                                FileLoadingErrorHandler &errorHandler,
                                                const QVector<QVariant> &errorParameters)
{
    ErrorDescriptor descriptor;
    descriptor.layer = layer;
    descriptor.error = error;
    descriptor.parameters = errorParameters;

    // The error handler is in an another thread
//    errorHandler.addErrorDescriptor(descriptor);
    ThreadConcurrentRun::run(errorHandler,
                             &FileLoadingErrorHandler::addErrorDescriptor,
                             descriptor);
}
