// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "topbarbloc.hpp"

#include <core/hmimanager.hpp>
#include <QQmlEngine>

#include "test-bed-lib/core/globalmanager.hpp"
#include "test-bed-lib/models/user.hpp"
#include "test-bed-lib/permission/authenticationmanager.hpp"
#include "services/navigation/navigationmanager.hpp"


TopBarBloc::TopBarBloc(QObject *parent):
    AbstractBloc(parent)
{
}

void TopBarBloc::initBloc()
{
}

void TopBarBloc::retrieveCurrentUserFullName()
{
    const AuthenticationManager *authenticationManager =
            &GlobalManager::instance().accessAuthenticationMngr();

    QSharedPointer<User> currentUser = authenticationManager->getCurrentUser();

    if(currentUser != nullptr)
    {
        QString userFullName;

        if(currentUser->getId() == 0)
        {
           userFullName = tr(guestUserFullName);
        }
        else
        {
           userFullName = currentUser->getFirstName() + " " + currentUser->getLastName();
        }

        emit currentUserFullNameRetrieved(userFullName);
    }
}

void TopBarBloc::onLogoutClicked()
{
    logout();
    goToLoginPage();
}

void TopBarBloc::logout()
{
    GlobalManager::instance().accessAuthenticationMngr().logout();
}

void TopBarBloc::goToLoginPage()
{
    HmiManager::instance().accessManager<NavigationManager>().goToLoginPage();
}

void TopBarBloc::registerQmlTypes()
{
    qmlRegisterType<TopBarBloc>("app.blocs", 1, 0, "TopBarBloc");
}
