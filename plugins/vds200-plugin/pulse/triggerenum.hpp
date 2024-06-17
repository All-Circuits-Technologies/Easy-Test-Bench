// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>


/** @brief List of the trigger value */
class TriggerEnum : public QObject
{
    Q_OBJECT

    public:
        enum Enum:quint16
        {
            Auto = 0,
            Man = 1,
            Unknown
        };

        Q_ENUM(Enum)

    public:
        /** @brief Parse the enum from string
            @param strValue The string to parse
            @return The enum parsed */
        static Enum parseFromString(const QString &strValue);

        /** @brief Return a string representation of the Flow Control
            @param flowControl The type to get the string representation from
            @return The string representation */
        static QString toString(Enum trigger);

        /** @brief Get all the flowcontrol enum values
            @return The list of memory types */
        static const QVector<Enum> &getAllEnums();

    private:
        static QVector<Enum> enumList;
};
