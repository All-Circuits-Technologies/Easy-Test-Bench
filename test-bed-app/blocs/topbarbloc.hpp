// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "abstractbloc.hpp"


/** @brief Backend class linked with the top bar **/
class TopBarBloc : public AbstractBloc
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The optional QObject parent */
        TopBarBloc(QObject *parent = nullptr);

        /** @brief Retrieve the The first and last name of the current user **/
        Q_INVOKABLE void retrieveCurrentUserFullName();

        /** @brief Called when logout item is clicked **/
        Q_INVOKABLE void onLogoutClicked();

        /** @brief Log out the current user **/
        void logout();

        /** @brief Display the login page **/
        void goToLoginPage();

    protected slots:
        /** @see AbstractBloc::initBloc */
        virtual void initBloc() override;

    signals:
        /** @brief Emit when the current user full name is retrieved
            @param currentUserFullName The first and last name of the current user */
        void currentUserFullNameRetrieved(const QString &currentUserFullName);

    public:
        /** @brief Register QML type linked to this bloc */
        static void registerQmlTypes();

    private:
        static const constexpr char *guestUserFullName = QT_TR_NOOP("(TR) Guest session");
};
