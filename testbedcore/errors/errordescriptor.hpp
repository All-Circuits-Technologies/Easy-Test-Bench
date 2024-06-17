// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QVariant>

#include "errors/errorlayer.hpp"


/** @brief Structure used as storage format for errors **/
struct ErrorDescriptor
{
        ErrorLayer::Enum layer;
        int error;
        QVector<QVariant> parameters;
};
