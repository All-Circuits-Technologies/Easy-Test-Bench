// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "aplugininputbloc.hpp"

#include "models/plugins/stringinputmodel.hpp"


/** @brief Backend class linked with string input view */
class StringInputBloc : public APluginInputBloc
{
        Q_OBJECT
        Q_PROPERTY(StringInputModel *castedModel
                   READ getStringInputModel
                   NOTIFY castedModelChanged)

    public:
        /** @brief Class constructor
            @param parent An optional parent QObject */
        StringInputBloc(QObject *parent = nullptr);

    public:
        /** @brief Get the input model */
        StringInputModel *getStringInputModel();

        /** @brief Set the input model */
        void setStringInputModel(StringInputModel *stringInputModel);

    public slots:
        /** @brief Called when the accept button is clicked
            @param inputText The input text written */
        void onAcceptClicked(const QString &inputText);

    public:
        /** @brief Register QML type linked to this bloc */
        static void registerQmlTypes();

    private slots:
        /** @brief Called when the model is updated */
        void onModelUpdated();

    signals:
        /** @brief emitted to display an error message when string entered is invalid
            @param errorMessage The message to display */
        void stringEnteredError(const QString &errorMessage);

        /** @brief Emitted when the casted model has been updated
            @param castedModel The updated model */
        void castedModelChanged(StringInputModel *castedModel);

    private:
        /** @brief Check if the string entered is valid
            @param value The string entered by the user */
//        bool validateString(const QString &value);

    private:
        static const constexpr char* stringTootShort = QT_TR_NOOP("(TR) String too short");
        static const constexpr char* stringTootLong = QT_TR_NOOP("(TR) String too long");
        static const constexpr char* stringUnexpected = QT_TR_NOOP("(TR) String do not match what "
                                                                   "is expected");
};
