// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "blocs/companions/menuitemname.hpp"

class MenuItemModel;


/** @brief Useful class to fill menu model */
class MenuCompanion
{
    public:
        /** @brief Class constructor */
        MenuCompanion();

    public:
        /** @brief Get a menu item model according to the name passed in parameter
            @param name Menu item Name
            @param parent An optional QObject parent
            @return A menu item model */
        static MenuItemModel *getMenuItem(MenuItemName::Enum name, QObject *parent = nullptr);
};
