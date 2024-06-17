// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

class BackResponseCategory : public QObject
{
    Q_OBJECT

    public:
        enum Enum: quint16
        {
            InfoResponse = 0,   // The Test procedure was stopped correctly
            ErrorResponse = 1,   // One single pulse was triggered
            Unknown
        };

        Q_ENUM(Enum)

    public:
        /** @brief Allows to register the meta type of the BackResponseCategory::Enum */
        static void registerMetaType();
};
