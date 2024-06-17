// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QString>


/** @brief Useful class to manage the Qt enum: @ref Qt::TimeSpec */
class QtTimeSpecHelper
{
    private:
        /** @brief Private constructor  */
        QtTimeSpecHelper() = default;

    public:
        /** @brief Convert the Qt::TimeSpec to string
            @param timeSpec The TimeSpec to stringify
            @return String representation of the Qt::TimeSpec given */
        static QString toString(Qt::TimeSpec timeSpec);

        /** @brief Parse the enum from string value
            @param dateFormat The Qt::TimeSpec to parse
            @param ok If the pointer is not null, set the value to true if no problem occurs
            @return The enum parse or Qt::UTC if the parsing failed */
        static Qt::TimeSpec parseFromString(const QString &timeSpec, bool *ok = nullptr);

        /** @brief Get all the available enums */
        static const QVector<Qt::TimeSpec> &getAllEnums();

    private:
        static QVector<Qt::TimeSpec> enumList;
};
