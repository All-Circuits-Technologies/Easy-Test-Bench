// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>


/** @brief Commands used with the atprogram.exe */
class AtProgramCmds : public QObject
{
    Q_OBJECT

    public:
        enum Enum {
            Info,
            Read,
            Verify,
            Program,
            Unknown
        };
        Q_ENUM(Enum)

    public:
        /** @brief Return the command stringified
            @note The string can be used as it is in the program arguments
            @param cmd The cmd to stringify
            @return The cmd stringified */
        static QString toString(Enum cmd);
};
