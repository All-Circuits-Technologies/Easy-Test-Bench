// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>


/** @brief The widget type */
class PluginWidgetType : public QObject
{
    Q_OBJECT

    public:
        /** @brief The type of available widgets plugins */
        enum Enum
        {
            String,
            RadioListButtons,
            SelectFile,
            OkButton,
            Unknown
        };
        Q_ENUM(Enum)

    public:
        /** @brief Parse the plugin widget from a string
            @param objectType The object type  to parse */
        static Enum parseFromString(const QString &objectType);

        /** @brief Register meta type linked to this enum */
        static void registerMetaTypes();

        /** @brief Register QML type linked to this enum  */
        static void registerQmlTypes();
};
