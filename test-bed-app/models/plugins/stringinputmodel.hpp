// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "aplugininputmodel.hpp"

#include <QRegularExpression>


/** @brief Model class linked with string input views */
class StringInputModel : public APluginInputModel
{
        Q_OBJECT
        Q_PROPERTY(int maxLength READ getMaxLength NOTIFY maxLengthChanged)
        Q_PROPERTY(int minLength READ getMinLength NOTIFY minLengthChanged)
        Q_PROPERTY(QRegularExpression regex READ getRegex NOTIFY regExpChanged)
        Q_PROPERTY(bool clearInputIfError READ isClearInputIfError NOTIFY clearInputIfErrorChanged)

    public:
        /** @brief Class constructor
            @param parent The parent class */
        StringInputModel(QObject *parent = nullptr);

        /** @brief Class constructor
            @param uiToken The Display UI Token linked to this plugin input model
            @param parent The parent widget */
        StringInputModel(const QSharedPointer<DisplayUiToken> &uiToken, QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~StringInputModel() override;

    public:
        /** @brief Get minimum length of the input */
        int getMinLength() const { return _minLength; }

        /** @brief Get maximum length of the input */
        int getMaxLength() const { return _maxLength; }

        /** @brief Get the regEx the input */
        const QRegularExpression &getRegex() const { return _regex; }

        /** @brief Say if we have to clear input when a validated error occurred */
        bool isClearInputIfError() const { return _clearInputIfError; }

    signals:
        /** @brief Emitted when maximum length changed */
        void maxLengthChanged(int maxLength);

        /** @brief Emitted when the minimum length changed */
        void minLengthChanged(int minLength);

        /** @brief Emitted when the reg exp changed */
        void regExpChanged(const QRegularExpression &regExp);

        /** @brief Emitted when the clear input if error occurred information changed */
        void clearInputIfErrorChanged(bool clearInputIfError);

    protected:
        /** @brief Parse the JsonObject description, get the informations and set the values on
                   the object
            @see ISequenceInstanceModule::getModuleParametersFormat
            @param elemDescription The JsonObject description to parse in order to build the widget
            @param update True if the elemDescription given is for update the model or for creating
                          a new view
            @return True if no problem occurs */
        virtual bool parseAndSetAttributes(const JsonObject &elemDescription,
                                           bool update = false) override;

    private:
        /** @brief Set minimum length of the input */
        void setMinLength(int minLength);

        /** @brief Set maximum length of the input */
        void setMaxLength(int maxLength);

        /** @brief Set the regEx of the input */
        void setRegex(const QString &regex);

        /** @brief Set the regEx of the input */
        void setRegex(const QRegularExpression &regex);

        /** @brief Set clear input if error information */
        void setClearInputIfError(bool clearInputIfError);

    public:
        /** @brief Register QML type linked to this model */
        static void registerQmlType();

    private:
        QRegularExpression _regex;
        int _minLength = -1;
        int _maxLength = -1;
        bool _clearInputIfError = false;
};
