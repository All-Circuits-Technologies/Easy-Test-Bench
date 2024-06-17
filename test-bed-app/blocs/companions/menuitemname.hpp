// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>


/** @brief List of the menu item names */
class MenuItemName : public QObject
{
    Q_OBJECT

    public:
        enum Enum
        {
            exit,
            preferences,
            history,
            open,
            exportLogs,
            close
        };

        Q_ENUM(Enum)

    public:
        /** @brief Obtain a menu item name by passing a key
            @param key The key to pass
            @return Name of a menu item */
        static QString getItemTrName(MenuItemName::Enum key);

    private:
        static const constexpr char *exitKey = QT_TR_NOOP("(TR) Exit");
        static const constexpr char *preferencesKey = QT_TR_NOOP("(TR) Preferences");
        static const constexpr char *historyKey = QT_TR_NOOP("(TR) History");
        static const constexpr char *openKey = QT_TR_NOOP("(TR) Open");
        static const constexpr char *exportLogsKey = QT_TR_NOOP("(TR) Exports logs");
        static const constexpr char *closeKey = QT_TR_NOOP("(TR) Close");
};
