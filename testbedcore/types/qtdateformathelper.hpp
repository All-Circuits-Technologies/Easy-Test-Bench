// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

/** @brief Useful class to manage the Qt enum: @ref Qt::DateFormat */
class QtDateFormatHelper
{
    private:
        /** @brief Private constructor  */
        QtDateFormatHelper() = default;

    public:
        /** @brief Convert the Qt::DateFormat to string
            @param dateFormat The dateFormat to stringify
            @return String representation of the Qt::DateFormat given */
        static QString toString(Qt::DateFormat dateFormat);

        /** @brief Parse the enum from string value
            @param dateFormat The Qt::DateFormat to parse
            @param ok If the pointer is not null, set the value to true if no problem occurs
            @return The enum parse or Qt::ISODateWithMs if the parsing failed */
        static Qt::DateFormat parseFromString(const QString &dateFormat, bool *ok = nullptr);

        /** @brief Get all the available enums */
        static const QVector<Qt::DateFormat> &getAllEnums();

    private:
        static QVector<Qt::DateFormat> enumList;
};
