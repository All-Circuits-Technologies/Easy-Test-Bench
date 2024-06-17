// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "jsonutility/jsontypedefs.hpp"
#include "testbedcore/testbedglobal.hpp"

class DisplayUiToken;


/** @brief This class manages the creation and displaying of the wanted UI
    @note This class has to be stored in the main thread (or the GUI thread)
    @note The abstract class has to be derived by a class defined in the application which call the
          library
    @note This class can be called by an another thread than the main one (or GUI thread);
          therefore, we have to manage the communication between threads */
class TESTBEDLIB_DLLSPEC ADisplayUserInterface : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The parent object */
        explicit ADisplayUserInterface(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~ADisplayUserInterface() override;

    public:
        /** @brief Ask for the displaying of the widget wanted
            @note The method is thread safe
            @note The result is returns via the DisplayUiToken promise returned
            @param elemToDisplay The details of the element to display
            @return A display UI token, this token has to be used like a promise */
        QSharedPointer<DisplayUiToken> displayWidget(const JsonObject &elemToDisplay);

        /** @brief Ask to update the content of a displaying widget
            @note The method is thread sage
            @param uiToken The token of the view to update
            @param elemToUpdate The element to update */
        void updateWidget(const QSharedPointer<DisplayUiToken> &uiToken,
                          const JsonObject &elemToUpdate);

    signals:
        /** @brief Emitted in order to display the widget in the right thread
            @param uiToken The promise UI token to return the result with
            @param elemToDisplay The details of the element to display */
        void _displayWidget(const QSharedPointer<DisplayUiToken> &uiToken,
                            const JsonObject &elemToDisplay);

        /** @brief Emitted in order to update the widget in the right thread
            @param uiToken The UI token linked to the view to update
            @param elemToUpdate The update details of the element to display */
        void _updateWidget(const QSharedPointer<DisplayUiToken> &uiToken,
                           const JsonObject &elemToUpdate);

    protected slots:
        /** @brief Manage the displaying of the widget wanted
            @note The method is called in the object thread (so in the right thread for displaying
                  objects)
            @param uiToken The promise UI token to return the result with
            @param elemToDisplay The details of the element to display */
        virtual void displayWidgetImpl(const QSharedPointer<DisplayUiToken> &uiToken,
                                       const JsonObject &elemToDisplay) = 0;

        /** @brief Manage the updating of the widget wanted
            @note The method is called in the object thread (so in the right thread for displaying
                  objects)
            @param uiToken The UI token linked to the view to update
            @param elemToUpdate The update details of the element to display */
        virtual void updateWidgetImpl(const QSharedPointer<DisplayUiToken> &uiToken,
                                      const JsonObject &elemToUpdate) = 0;

    private:
        bool _alreadyConnected{false};
};
