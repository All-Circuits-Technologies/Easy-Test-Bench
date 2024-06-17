// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphaël Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "blocs/loginbloc.hpp"

#include <QQmlEngine>

#include "core/hmimanager.hpp"
#include "services/navigation/navigationmanager.hpp"
#include "test-bed-lib/core/globalmanager.hpp"
#include "test-bed-lib/permission/authenticationmanager.hpp"
#include "test-bed-lib/database/dbmanager.hpp"
#include "test-bed-lib/models/liboptions.hpp"


LoginBloc::LoginBloc(QObject *parent) :
    ABasePageBloc(parent),
    _authenticationManager(GlobalManager::instance().accessAuthenticationMngr()),
    _buttonText(tr(NormalButtonText))
{
}

LoginBloc::~LoginBloc()
{
    // Overwrite the memory
    _password.fill(0);
}

void LoginBloc::initBloc()
{
    _isGuestModeAllowed = GlobalManager::instance().accessDbMngr()
                                                            .getLibOptions()->isGuestModeAllowed();
    updateLogInButton();
    ABasePageBloc::initBloc();
}

void LoginBloc::onConnexionButtonClicked(const QString &username, const QString &password)
{
    if(username.isEmpty() && password.isEmpty())
    {
        loginDefaultUser();
    }
    else
    {
        login(username, password);
    }
}

void LoginBloc::login(const QString &username,const QString &password)
{
    setLoading(true);
    bool idsAreCorrect = false;

    if(!_authenticationManager.login(username, password, idsAreCorrect))
    {
        emit loginFailed(DatabaseError);
        setLoading(false);
        return;
    }

    if(!idsAreCorrect)
    {
        emit loginFailed(WrongIdsError);
        setLoading(false);
        return;
    }

    goToProjectSelectionPage();
}

void LoginBloc::loginDefaultUser()
{
    setLoading(true);
    if(!_isGuestModeAllowed)
    {
        // Can't try to login as Default user if the mode isn't allowed
        emit loginFailed(WrongIdsError);
        setLoading(false);
        return;
    }

    if(!_authenticationManager.loginDefaultUser())
    {
        emit loginFailed(DatabaseError);
        setLoading(false);
        return;
    }

    goToProjectSelectionPage();
}

void LoginBloc::setButtonEnabled(bool enabled)
{
    if(_buttonEnabled != enabled)
    {
        _buttonEnabled = enabled;
        emit buttonEnabledUpdated(_buttonEnabled);
    }
}

void LoginBloc::setLoading(bool loading)
{
    if(_loading != loading)
    {
        _loading = loading;
        updateLogInButton();
        emit loadingUpdated(_loading);
    }
}

void LoginBloc::setButtonText(const QString &buttonText)
{
    if(_buttonText != buttonText)
    {
        _buttonText = buttonText;
        emit buttonTextUpdated(_buttonText);
    }
}

void LoginBloc::updateLogInButton()
{
    if(_loading)
    {
        setButtonEnabled(false);
        return;
    }

    if(_username.isEmpty() && _password.isEmpty())
    {
        if(_isGuestModeAllowed)
        {
            setButtonText(tr(GuestButtonText));
            setButtonEnabled(true);
        }
        else
        {
            setButtonText(tr(NormalButtonText));
            setButtonEnabled(false);
        }
    }
    else
    {
        setButtonText(tr(NormalButtonText));
        setButtonEnabled(true);
    }
}

void LoginBloc::goToProjectSelectionPage()
{
    HmiManager::instance().accessManager<NavigationManager>().goToProjectSelectionPage();
}

void LoginBloc::onUsernameUpdated(const QString &username)
{
    if(_username != username)
    {
        _username = username;
        updateLogInButton();
    }
}

void LoginBloc::onPasswordUpdated(const QString &password)
{
    if(_password != password)
    {
        _password = password;
        updateLogInButton();
    }
}

void LoginBloc::registerQmlTypes()
{
    qmlRegisterType<LoginBloc>("app.blocs", 1, 0, "LoginBloc");
}
