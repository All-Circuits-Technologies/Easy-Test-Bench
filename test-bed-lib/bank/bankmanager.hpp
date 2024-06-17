// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QHash>
#include <QObject>
#include <QSharedPointer>

#include "bank/bankcommoninterface.hpp"
#include "bankcoreshared/usebylib/ibankinterface.hpp"

class QPluginLoader;
class SettingsManager;


/** @brief Manager to help the loading of banks external plugins */
class BankManager : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param settingsMngr The settings manager used to get the folder where to find banks
            @param displayUserInterface The user interface object created in the application in
                                        order to display widgets in it, if null, the
                                        @ref displayParameterInput method will always return false
            @param dbManager The database manager used in the BankCommonInterface
            @param parent The parent class */
        explicit BankManager(const SettingsManager &settingsMngr,
                             ADisplayUserInterface *displayUserInterface,
                             DbManager &dbManager,
                             QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~BankManager() override;

    public:
        /** @brief Get the name list of loaded banks
            @return The loaded banks names list */
        QVector<QString> getBankNames() const;

        /** @brief Access to a loaded bank name thanks to its name
            @param name The name of the bank to access
            @return The Bank interface */
        QSharedPointer<IBankInterface> getBankInterface(const QString &name) const;

        /** @brief Access to the bank common interface
            @return The bank common interface */
        BankCommonInterface *accessCommonInterface() const { return _commonInterface; }

        /** @brief Reload the banks, and try to get all the loadable banks in the banks folder
            @return True If no problem occurs */
        bool reloadBanks();

    public:
        /** @brief Register all required stuff to use correctly the banks */
        static void RegisterMetaTypes();

    private:
        /** @brief Clean the banks list
            @return True If no problem occurs */
        bool cleanBanksList();

    private:
        /** @brief The structure contains the bank interface and the plugin loaded linked */
        struct BankData
        {
            QSharedPointer<IBankInterface> interface;
            QPluginLoader *pluginLoader{nullptr};
        };

    private:
        QHash<QString, BankData> _bankInterfaces;
        BankCommonInterface *_commonInterface{nullptr};
        const SettingsManager &_settingsMngr;
};
