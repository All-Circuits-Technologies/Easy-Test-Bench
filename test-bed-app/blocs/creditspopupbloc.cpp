// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "blocs/creditspopupbloc.hpp"

#include <QCoreApplication>
#include <QQmlEngine>
#include <QSettings>

#include "core/configurationkeys.hpp"
#include "core/configurationmanager.hpp"
#include "core/hmimanager.hpp"


CreditsPopUpBloc::CreditsPopUpBloc(QObject *parent):
    AbstractBloc(parent)
{
}

void CreditsPopUpBloc::initBloc()
{
    ConfigurationManager *configurationManager
            = &HmiManager::instance().accessManager<ConfigurationManager>();

    if(!configurationManager->read(ConfigurationKeys::Enum::YearsOfProduction,
                                   _yearsOfProduction))
    {
        qWarning() << "Can't get years of production text from configuration file";
    }

    if(!configurationManager->read(ConfigurationKeys::Enum::Author,
                                   _author))
    {
        qWarning() << "Can't get author text from configuration file";
    }

    if(!configurationManager->read(ConfigurationKeys::Address,
                                   _address))
    {
        qWarning() << "Can't get address text from configuration file";
    }

    if(!configurationManager->read(ConfigurationKeys::Country,
                                   _country))
    {
        qWarning() << "Can't get country text from configuration file";
    }

    if(!configurationManager->read(ConfigurationKeys::Website,
                                   _website))
    {
        qWarning() << "Can't get website text from configuration file";
    }

    _appVersion = QString(APP_VERSION);

    emit blocInitialized();
}

void CreditsPopUpBloc::registerQmlTypes()
{
    qmlRegisterType<CreditsPopUpBloc>("app.blocs", 1, 0, "CreditsPopUpBloc");
}
