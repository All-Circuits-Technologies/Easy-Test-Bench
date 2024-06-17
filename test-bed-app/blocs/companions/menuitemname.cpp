// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "blocs/companions/menuitemname.hpp"

#include <QMetaEnum>


QString MenuItemName::getItemTrName(MenuItemName::Enum key)
{
    switch(key)
    {
        case Enum::exit :
            return tr(exitKey);

        case Enum::preferences :
            return tr(preferencesKey);

        case Enum::history :
            return tr(historyKey);

        case Enum::open :
            return tr(openKey);

        case Enum::exportLogs :
            return tr(exportLogsKey);

        case Enum::close :
            return tr(closeKey);
    }

    return {""};
}
