// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "services/navigation/navigationpagetype.hpp"

#include <QMetaEnum>
#include <QQmlEngine>

#include "definesutility/definesutility.hpp"


QString NavigationPageType::toString(NavigationPageType::Enum pageType)
{
    return QString::fromLatin1(QMetaEnum::fromType<Enum>().valueToKey(pageType));
}

void NavigationPageType::registerMetaTypes()
{
    qRegisterMetaType<NavigationPageType::Enum>("NavigationPageType::Enum");
}

void NavigationPageType::registerQmlTypes()
{
    qmlRegisterType<NavigationPageType>("app.managers.navigation",
                                        1,
                                        0,
                                        "NavigationPageType");
}
