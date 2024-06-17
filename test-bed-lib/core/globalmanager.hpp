// SPDX-FileCopyrightText: 2019 - 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
// SPDX-FileCopyrightText: 2019 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "testbedcore/testbedglobal.hpp"

class ADisplayUserInterface;
class AuthenticationManager;
class BankManager;
class DbManager;
class LogsManager;
class PermissionManager;
class ProjectManager;
class SettingsManager;
class TicManager;
class YamlToJsonManager;


/*! @brief This manager is the entry point to access all infinite-life objects */
class TESTBEDLIB_DLLSPEC GlobalManager : public QObject
{
    Q_OBJECT

    public:
        /*! @brief Create the global manager
            @param parent Optional Qt parentship */
        explicit GlobalManager(QObject *parent = nullptr);

    public:
        /** @brief Access project manager
            @return The project manager */
        ProjectManager &accessProjectMngr() { return *_projectManager; }

        /** @brief Get project manager
            @return The project manager */
        const ProjectManager &getProjectMngr() const { return *_projectManager; }

        /** @brief Access bank manager
         *  @return The bank manager */
        BankManager &accessBankMngr() { return *_bankManager; }

        /** @brief Get bank manager
            @return The bank manager */
        const BankManager &getBankMngr() const { return *_bankManager; }

        /** @brief Access settings manager
            @return The settings manager */
        SettingsManager &accessSettingsMngr() { return *_settingsManager; }

        /** @brief Get settings manager
            @return The bank manager */
        const SettingsManager &getSettingsMngr() const { return *_settingsManager; }

        /** @brief Access database manager */
        DbManager &accessDbMngr() { return *_dbManager; }

        /** @brief Get database manager */
        const DbManager &getDbMngr() const { return *_dbManager; }

        /** @brief Get permission manager
            @return The permission manager */
        const PermissionManager &accessPermissionMngr() const { return *_permissionManager; }

        /** @brief Get authentication manager
            @return The authentication manager */
        AuthenticationManager &accessAuthenticationMngr() const { return *_authenticationManager; }

        /** @brief Access the tic manager
            @return The tic manager */
        TicManager &accessTicMngr() const { return *_ticManager; }

        /** @brief Access YAML manager */
        YamlToJsonManager &accessYamlMngr() const { return *_yamlManager; }

    public:
        /*! @brief Create the instance.
            @note Must be called once, before Instance gets called
            @param displayUserInterface The user interface object created in the application in
                                        order to display widgets in it, if null, the
                                        @ref displayParameterInput method will always return false*/
        static void create(ADisplayUserInterface *displayUserInterface = nullptr);

        /*! @brief Get singleton instance.
            @note If the instance hasn't already been created, creates it
         *  @see GlobalManager::Create */
        static GlobalManager &instance();

    private slots:
        /** @brief Called when the main event loop is entered and ready
            @note Called at library start
            @note If you have methods to call at start which needs to use the event loop, put yours
                  methods in it */
        void onEventLoopReady();

    private:
        /** @brief Call all the known meta and event types to register */
        void registerTypes();

        /** @brief Call to set all the default informations of the library */
        void setDefaultLibraryInformations();

        /*! @brief Instanciate all members.
            @warning When the method is called the main event loop isn't started yet, if you need it
                     see @ref GlobalManager::onEventLoopReady
            @note See comment inside contructor to understand why.
            @param displayUserInterface The user interface object created in the application in
                                        order to display widgets in it, if null, the
                                        @ref displayParameterInput method will always return false*/
        void init(ADisplayUserInterface *displayUserInterface);

        /** @brief Initialize the logs manager */
        void initLogsManager();

        /** @brief Create and init YAML manager
            @param settingsMngr Reference to the settings manager */
        void createAndInitYamlManager(SettingsManager &settingsMngr);

    private:
        static GlobalManager * _instance;    //!< @brief Instance singleton

    private:
        ProjectManager *_projectManager{nullptr};
        BankManager *_bankManager{nullptr};
        LogsManager *_logsManager{nullptr};
        SettingsManager *_settingsManager{nullptr};
        DbManager * _dbManager{nullptr};
        PermissionManager *_permissionManager{nullptr};
        AuthenticationManager *_authenticationManager{nullptr};
        TicManager *_ticManager{nullptr};
        YamlToJsonManager *_yamlManager{nullptr};
};
