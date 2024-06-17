// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "services/navigation/navigationbehavior.hpp"

#include <QQmlEngine>

#include "definesutility/definesutility.hpp"


void NavigationBehavior::registerMetaTypes()
{
    qRegisterMetaType<NavigationBehavior::Enum>("NavigationBehavior::Enum");
}

void NavigationBehavior::registerQmlTypes()
{
 qmlRegisterType<NavigationBehavior>("app.managers.navigation",
                                     1,
                                     0,
                                     "NavigationBehavior");
}
