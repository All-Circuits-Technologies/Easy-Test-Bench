// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "core/hmimanager.hpp"
#include "core/hmiconstants.hpp"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    QGuiApplication::setOrganizationName(HmiConstants::organizationName);
    QGuiApplication::setOrganizationDomain(HmiConstants::organizationDomain);
    QGuiApplication::setApplicationName(HmiConstants::applicationName);

    HmiManager::create();

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    auto exitSlot = [url](QObject *obj, const QUrl &objUrl)
    {
        if ((obj == nullptr) && (url == objUrl))
        {
            QCoreApplication::exit(-1);
        }
    };

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app,    exitSlot, Qt::QueuedConnection);

    engine.load(url);

    return QGuiApplication::exec();
}
