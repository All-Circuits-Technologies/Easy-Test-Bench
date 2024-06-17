// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "services/navigation/navigationbehavior.hpp"
#include "services/navigation/navigationpagetype.hpp"


/** @brief Useful manager, used as a singleton in the QML pages, to manage the navigation between
           QML pages */
class NavigationManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(NavigationPageType::Enum currentPage
               READ getCurrentPage
               WRITE setCurrentPage
               NOTIFY currentPageChanged)

    public:
        /** @brief Class constructor
            @param parent The parent instance class */
        explicit NavigationManager(QObject *parent = nullptr);

    public:
        /** @brief Get the current displayed page */
        NavigationPageType::Enum getCurrentPage() const { return _currentPage; }

        /** @brief Set the page currently displayed in the QML app */
        void setCurrentPage(NavigationPageType::Enum currentPage);

        /** @brief Get the name of the page
            @param page The page to get its name */
        Q_INVOKABLE QString getPageName(NavigationPageType::Enum page);

    public slots:
        /** @brief Go to the login page */
        Q_INVOKABLE void goToLoginPage();

        /** @brief Got to the projection selection page
            @param behavior The behavior to apply when modifying the page */
        Q_INVOKABLE void goToProjectSelectionPage(NavigationBehavior::Enum behavior =
                                                                    NavigationBehavior::Replace);

        /** @brief Go to the sequence selection page */
        Q_INVOKABLE void goToSequenceSelectionPage();

        /** @brief Go to the sequence run page */
        Q_INVOKABLE void goToSequenceRunPage();

    public:
        /** @brief Call all the known meta and event types to register */
        static void registerMetaTypes();

        /** @brief Register all the QML types linked to this manager */
        static void registerQmlTypes();

    signals:
        /** @brief Emitted when the current page has been changed */
        void currentPageChanged(NavigationPageType::Enum currentPage);

        /** @brief Emitted when we want to go to a specific page
            @param newPage The page where we want to go
            @param behavior The way we want to go to this page */
        void changePageTo(NavigationPageType::Enum newPage, NavigationBehavior::Enum behavior);

    private:
        /** @brief Call to prevent the QML that we want to change the current page
            @param newPage The page where we want to go
            @param behavior The way we want to go to this page */
        void goToPage(NavigationPageType::Enum page, NavigationBehavior::Enum behavior);

    private:
        NavigationPageType::Enum _currentPage{NavigationPageType::LoginPage};
};
