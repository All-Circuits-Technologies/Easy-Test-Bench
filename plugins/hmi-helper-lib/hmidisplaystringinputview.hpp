// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "hmidisplayhelper.hpp"


/** @brief Displau a views with a string input to fill by user */
class HmiDisplayStringInputView : public HmiDisplayHelper
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param commonItf The common interface to use in order to ask the display of a GUI
            @param parent The parent class */
        explicit HmiDisplayStringInputView(ICommonInterface& commonItf, QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~HmiDisplayStringInputView() override;

    public:
        /** @brief Set clear field if error value
            @attention To work, this method has to be called before the @ref displayHmi method
            @param value True to clear the field if an error occurred */
        void setClearFieldIfError(bool value) { _clearFieldIfError = value; }

        /** @brief Display an string input view
            @param name The name of the view
            @param description The description to display in the view
            @param imagePath The path of the image to display
            @param minLength The min length of the expected string to write by user
            @param maxLength The max length of the expected string to write by user
            @param patternRegex The pattern regex of the expected string to write by user
            @return True if no problem occurred */
        bool displayHmi(const QString &name,
                        const QString &description,
                        const QString &imagePath,
                        int minLength = DefaultMinLength,
                        int maxLength = DefaultMaxLength,
                        const QString &patternRegex = QString());

    protected:
        /** @brief Parse the display result and say if the HMI ended in succes or not
            @param valuesSet The values got from HMI
            @return True if the HMI ended in success */
        virtual bool parseDisplayResult(const JsonArray &valuesSet) override;

    public:
        /** @brief Default minimum length value */
        static const constexpr int DefaultMinLength = -1;

        /** @brief Default maximum length value */
        static const constexpr int DefaultMaxLength = -1;

        /** @brief Default clear field if error value */
        static const constexpr bool DefaultClearFieldIfError = false;

    private:
        int _minLength{DefaultMinLength};
        int _maxLength{DefaultMaxLength};
        QString _patternRegex;
        bool _clearFieldIfError{DefaultClearFieldIfError};
};
