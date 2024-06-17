// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "servicesregistrator.hpp"

#include <QQmlEngine>

#include "definesutility/definesutility.hpp"
#include "core/hmimanager.hpp"
#include "services/navigation/navigationmanager.hpp"
#include "services/sizemanager.hpp"


void ServicesRegistrator::registerMetaTypes()
{
    NavigationManager::registerMetaTypes();
}

void ServicesRegistrator::registerQmlTypes()
{
    NavigationManager::registerQmlTypes();

    qmlRegisterSingletonType(QUrl("qrc:/managers/ColorsManager.qml"),
                             "app.managers",
                             1,
                             0,
                             "ColorsManager");

    qmlRegisterSingletonType(QUrl("qrc:/managers/FontsManager.qml"),
                             "app.managers",
                             1,
                             0,
                             "FontsManager");

    qmlRegisterSingletonType<SizeManager>("app.managers", 1, 0, "SizeManager",
                [](QQmlEngine *qmlEngine, QJSEngine *jsEngine) -> QObject* {
                    Q_UNUSED(qmlEngine)
                    Q_UNUSED(jsEngine)
                    return &HmiManager::instance().accessManager<SizeManager>();
                });


    qmlRegisterSingletonType<NavigationManager>("app.managers.navigation",
                                                1,
                                                0,
                                                "NavigationManager",
                [](QQmlEngine *qmlEngine, QJSEngine *jsEngine) -> QObject* {
                    Q_UNUSED(qmlEngine)
                    Q_UNUSED(jsEngine)
                    return &HmiManager::instance().accessManager<NavigationManager>();
                });
}
