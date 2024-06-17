// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "models/scrollablepopupmodel.hpp"


/** @brief The error handler companion to manage the errors of sequence loading to display in
           pop up view */
class ErrorHandlerCompanion : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor */
        explicit ErrorHandlerCompanion(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~ErrorHandlerCompanion() override;

    public:
        /** @brief Get the model linked to the pop up which can be displayed */
        ScrollablePopUpModel *getPopUpModel() const { return _popUpModel; }

        /** @brief Convert error messages into a single message in html format
            @param messages List of error messages
            @return Formated error message */
        QString formatErrorMessages(const QStringList &messages);

        /** @brief Helpful method to modify the buttons to display in the Pop Up (when it's
                   displayed)
            @param withClosedQuestion If yes, the Pop Up will display two buttons (Yes and No),
                                      which allows to answer to a closed question
                                      If no, the Pop Up will display an "Ok" button (one button to
                                      acknowledge the information) */
        void popUpBtnsFormat(bool withClosedQuestion);

    private:
        static const constexpr char* ulTag = "<ul>%1</ul>";
        static const constexpr char* liTag = "<li style=\"margin: %1px 0 0 0\">%2</li>";
        static const constexpr int errorMsgsMarginSize = 10;

    private:
        ScrollablePopUpModel *_popUpModel{nullptr};
};
