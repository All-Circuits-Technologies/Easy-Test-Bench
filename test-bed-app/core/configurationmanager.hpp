// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "core/configurationkeys.hpp"

class QSettings;


/** @brief This Manager allows access to the values stored in the configuration file  */
class ConfigurationManager : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The optional QObject parent */
        ConfigurationManager(QObject *parent = nullptr);

    public:
        /** @brief Read a value in the configuration file
            @param key The key to access to the value
            @param value The value to be retrieved from the configuration file
            @return True if success, false if failure */
        bool read(ConfigurationKeys::Enum key, QString &value) const;

    private:
        static const constexpr char * configurationFilePath = ":/app.conf";

    private:
        QSettings *_configurationFile{nullptr};
};
