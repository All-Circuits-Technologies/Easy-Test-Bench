// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QVector>


/** @brief This enum is used to describe the sequence log format to use */
class SequenceLogFormatType : public QObject
{
    Q_OBJECT

    public:
        enum Enum
        {
            DisplayAll,         //!< @brief Display all the elements in logs
            OnlyDisplayMsg,     //!< @brief Only display the log message
            Unknown
        };
        Q_ENUM(Enum)

    public:
        /** @brief Parse the enum from string value
            @param value The value to parse
            @return The enum parse or Unknown if the parsing failed */
        static Enum parseFromString(const QString &value);

        /** @brief Convert the enum to a string value
            @param value The parse to stringify
            @return The value stringified*/
        static QString toString(Enum value);

        /** @brief Get all the available enums */
        static const QVector<Enum> &getAllEnums();

    private:
        static QVector<Enum> enumList;
};
