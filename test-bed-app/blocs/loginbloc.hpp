// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphaël Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "blocs/abasepagebloc.hpp"

#include "permission/authenticationmanager.hpp"


/** @brief Backend class linked with the login page **/
class LoginBloc : public ABasePageBloc
{
    Q_OBJECT
    Q_PROPERTY(bool buttonEnabled READ isButtonEnabled NOTIFY buttonEnabledUpdated)
    Q_PROPERTY(bool loading READ isLoading NOTIFY loadingUpdated)
    Q_PROPERTY(QString buttonText READ getButtonText NOTIFY buttonTextUpdated)


    public:
        /** @brief Class constructor
            @param parent The optional QObject parent */
        explicit LoginBloc(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~LoginBloc() override;

    public:
        /** @brief Called when connexion Button is clicked
            @note This method is used in QML file to say that a click has occurred
            @param username The username entered by the user
            @param password The password entered by the user */
        Q_INVOKABLE void onConnexionButtonClicked(const QString &username, const QString &password);

        /** Redirect to the project selection page **/
        void goToProjectSelectionPage();

        /** @brief Say if the submit button has to be enabled */
        bool isButtonEnabled() const { return _buttonEnabled; }

        /** @brief Say if the page is loading
            @note When the page is loading the submit button has to be disabled */
        bool isLoading() const { return _loading; }

        /** @brief Return the button text */
        const QString &getButtonText() const { return _buttonText; }

    public slots:
        /** @brief Called when the user modifies the username in the text field
            @param username The current username value */
        void onUsernameUpdated(const QString &username);

        /** @brief Called when the user modifies the password in the text field
            @param password The current password value */
        void onPasswordUpdated(const QString &password);

    protected slots:
        /** @see AbstractBloc::initBloc */
        virtual void initBloc() override;

    signals :
        /** @brief Emited when, login fail
            @param loginErrorMessage The error message displayed on the hmi */
        void loginFailed(const QString &loginErrorMessage);

        /** @brief Emitted when the button enabled value has been updated
            @param buttonEnabled The enable value of the button */
        void buttonEnabledUpdated(bool buttonEnabled);

        /** @brief Emitted when the loading value is updated
            @param loading The new loading value */
        void loadingUpdated(bool loading);

        /** @brief Emitted when the button text is updated
            @param buttonText The updated button text */
        void buttonTextUpdated(const QString &buttonText);

    private:
        /** @brief Attempts to log the user in based on the identifiers he have entered
            @param username The username entered by the user
            @param password The password entered by the user */
        void login(const QString &username, const QString &password);

        /** @brief logs in the default user */
        void loginDefaultUser();

        /** @brief Enable/disable the button
            @param enabled The new enable value of the button */
        void setButtonEnabled(bool enabled);

        /** @brief Set the loading value of the page
            @param loading The new loading value */
        void setLoading(bool loading);

        /** @brief The submit button text
            @param buttonText The text of the button */
        void setButtonText(const QString &buttonText);

        /** @brief To call in order to update the log in button */
        void updateLogInButton();

    public:
        /** @brief Register QML type linked to this bloc */
        static void registerQmlTypes();

    private:
        static const constexpr char *DatabaseError = QT_TR_NOOP("(TR) Connection failed, an error "
                                                                "occurred within the application");
        static const constexpr char *WrongIdsError = QT_TR_NOOP("(TR) Wrong username/password "
                                                                "combination");
        static const constexpr char *GuestButtonText =  QT_TR_NOOP("(TR) Continue without "
                                                                   "authentication");
        static const constexpr char *NormalButtonText =  QT_TR_NOOP("(TR) Log in");

    private:
        bool _isGuestModeAllowed{false};
        AuthenticationManager &_authenticationManager;

        QString _username;
        QString _password;
        bool _loading{false};
        bool _buttonEnabled{false};
        QString _buttonText;
};
