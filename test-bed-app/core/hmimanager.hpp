// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "interfaces/appinterfacemanager.hpp"

#include "test-bed-lib/core/settingsmanager.hpp"

class ConfigurationManager;
class NavigationManager;
class SizeManager;


/** @brief The Hmi manager manages all the managers (singleton) related to the hmi
           and it's the entry point to get them all. */
class HmiManager : public QObject
{
        Q_OBJECT

        private:
            /** @brief Class constructor
                @param parent The optional QObject parent */
            explicit HmiManager(QObject *parent = nullptr);

        public:
            /** @brief Generic constant getter for accessing all the(known) managers
                @note If the manager isn't known an error will raise at building time */
            template<class T>
            const T &getManager() const;

            /** @brief Generic accesser for accessing all the(known) managers
                @note If the manager isn't known an error will raise at building time */
            template<class T>
            T &accessManager();

            /** @brief Get DisplayAppInterface */
            AppInterfaceManager &getDisplayAppInterface() { return *_displayAppInterface; }

        public:
            /*! @brief Create the instance.
                @note Must be called once, before Instance gets called */
            static void create();

            /*! @brief Get singleton instance.
                @note If the instance hasn't already been created, creates it
             *  @see HmiManager::Create */
            static HmiManager &instance();

        private:
            /** @brief Call all the known C++ and QML modules (which needs to be registered in C++) to
                       register them */
            void registerQmlTypes();

            /** @brief Call all the known meta and event types to register */
            void registerMetaTypes();

            /*! @brief Instantiate all members.
                @warning When the method is called the main event loop isn't started yet, if you need it
                         see @ref HmiManager::onEventLoopReady
                @note See comment inside contructor to understand why.,*/
            void init();

            /** @brief Init the translation layer for the hmi
                @note This method is called by the @ref init method*/
            void initTranslations();

        private:
            static HmiManager * _instance;    //!< @brief Instance singleton

        private:
            SizeManager *_sizeManager{nullptr};
            SettingsManager *_settingsMngr{nullptr};
            NavigationManager *_navigationManager{nullptr};
            ConfigurationManager *_configurationManager{nullptr};
            AppInterfaceManager *_displayAppInterface{nullptr};
    };

    template<class T>
    const T &HmiManager::getManager() const
    {
        // Create an invalid assert to return an error from builder when we are here
        static_assert (std::is_same<T, QObject>::value && !std::is_same<T, QObject>::value,
                "The manager isn't managed");
        return T();
    }

    template<class T>
    T &HmiManager::accessManager()
    {
        // Create an invalid assert to return an error from builder when we are here
        static_assert (std::is_same<T, QObject>::value && !std::is_same<T, QObject>::value,
                "The manager isn't managed");
        return T();
    }

template<>
inline const SizeManager &HmiManager::getManager() const { return *_sizeManager; }

template<>
inline SizeManager &HmiManager::accessManager() { return *_sizeManager; }

template<>
inline const ConfigurationManager &HmiManager::getManager() const { return *_configurationManager; }

template<>
inline ConfigurationManager &HmiManager::accessManager() { return *_configurationManager; }

template<>
inline const NavigationManager &HmiManager::getManager() const { return *_navigationManager; }

template<>
inline NavigationManager &HmiManager::accessManager() { return *_navigationManager; }
