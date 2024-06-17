// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "errors/errorlayer.hpp"

#include "errors/fileloadingerrorhandler.hpp"


/** @brief Base class for error handler helpers */
class FileLoadingErrorHelper
{
    protected:
        /** @brief Add an error descriptor in the error stack
            @param layer The error layer
            @param error The integer corresponding to the associated enum value
            @param errorHandler An Errorhandler pointer
            @param errorParameters A vector containing error parameters */
        static void addErrorDescriptor(ErrorLayer::Enum layer,
                                       int error,
                                       FileLoadingErrorHandler &errorHandler,
                                       const QVector<QVariant> &errorParameters =
                                                                            QVector<QVariant>());
};
