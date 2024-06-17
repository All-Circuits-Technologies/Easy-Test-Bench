// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "blocs/companions/menucompanion.hpp"

#include <QCoreApplication>
#include <QMetaEnum>

#include "models/menuitemmodel.hpp"


MenuCompanion::MenuCompanion()
{
}

MenuItemModel *MenuCompanion::getMenuItem(MenuItemName::Enum name, QObject *parent)
{
    MenuItemModel *menuItemModel = new MenuItemModel(parent);
    menuItemModel->setName(MenuItemName::getItemTrName(name));

    switch(name)
    {
        case MenuItemName::exit:
            QObject::connect(menuItemModel, &MenuItemModel::clicked, &QCoreApplication::quit);
            break;

        case MenuItemName::preferences:
        case MenuItemName::history:
        case MenuItemName::exportLogs:
            menuItemModel->setEnabled(false);
            break;

        default:
            // Do nothing
            break;
    }

    return menuItemModel;
}
