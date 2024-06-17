// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "core/configurationmanager.hpp"

#include <QCoreApplication>
#include <QtDebug>
#include <QSettings>


ConfigurationManager::ConfigurationManager(QObject *parent):
    QObject(parent)
{
    _configurationFile = new QSettings(configurationFilePath, QSettings::IniFormat);
}

bool ConfigurationManager::read(ConfigurationKeys::Enum key, QString &value) const
{
    if(_configurationFile == nullptr)
    {
        qWarning() << "Can't access to configuration file";
        return false;
    }

    value = _configurationFile->value(ConfigurationKeys::toString(key).toLower()).toString();

    return true;
}
