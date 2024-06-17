// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "types/popupbtntype.hpp"


/** @brief This model is used with the scrollable pop up item */
class ScrollablePopUpModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector<PopUpBtnType::Enum> neededBtns
               READ getNeededBtns
               NOTIFY btnsUpdated)
    Q_PROPERTY(PopUpBtnType::Enum primaryBtn READ getPrimaryBtn NOTIFY btnsUpdated)

    public:
        /** @brief Class constructor
            @param neededBtns The buttons to display in the Pop Up
            @param primaryBtn The button to display as primary
            @param parent The parent object */
        explicit ScrollablePopUpModel(PopUpBtnType::Enums neededBtns = {},
                                      PopUpBtnType::Enum primaryBtn = PopUpBtnType::Unknown,
                                      QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~ScrollablePopUpModel() override;

    public:
        /** @brief Get the needed buttons linked to the pop up */
        QVector<PopUpBtnType::Enum> getNeededBtns() const;

        /** @brief Get the button which is primary */
        PopUpBtnType::Enum getPrimaryBtn() const { return _primaryBtn; }

        /** @brief Set the buttons configuration for the pop up
            @param neededBtns The buttons to set in the view
            @param primaryBtn The button which is primary */
        void setBtns(PopUpBtnType::Enums neededBtns, PopUpBtnType::Enum primaryBtn);

        /** @brief Test if the button given is primary
            @param btnType The button to test if it's primary
            @return True if the button is primary */
        Q_INVOKABLE bool isPrimary(PopUpBtnType::Enum btnType) const;

        /** @brief Get the button label
            @param btnType The button to get its label
            @return The label linked to the btton */
        Q_INVOKABLE QString getBtnLabel(PopUpBtnType::Enum btnType) const;

        /** @brief Fire the button click
            @note Emits the clicked signal depending of the btn type given
            @param btnType The button to fire its click */
        Q_INVOKABLE void fireBtnClick(PopUpBtnType::Enum btnType);

        /** @brief Fire the display of Pop Up view
            @param message The message to display */
        Q_INVOKABLE void fireDisplayPopUp(const QString &message);

    signals:
        /** @brief Emitted when the buttons configuration is updated */
        void btnsUpdated();

        /** @brief Emitted when the OK button has been clicked */
        void okBtnClicked();

        /** @brief Emitted when the NO button has been clicked */
        void noBtnClicked();

        /** @brief Emitted when the YES button has been clicked */
        void yesBtnClicked();

        /** @brief Display the Pop Up with the given message
            @param message The message to display */
        void displayPopUp(const QString &message);

    public:
        /** @brief Register QML type linked to this class */
        static void registerQmlType();

    private:
        /** @brief Useful method to set the buttons configuration without emitting the
                   @ref btnsUpdated signal
            @param neededBtns The buttons to set in the view
            @param primaryBtn The button which is primary
            @param atLeastOneModified Equal to true if the configuration has been modified */
        void setBtnsWithoutSignal(PopUpBtnType::Enums neededBtns,
                                  PopUpBtnType::Enum primaryBtn,
                                  bool &atLeastOneModified);

    private:
        PopUpBtnType::Enums _neededBtns;

        PopUpBtnType::Enum _primaryBtn{PopUpBtnType::Unknown};
};
