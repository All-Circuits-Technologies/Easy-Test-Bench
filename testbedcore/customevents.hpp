// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QEvent>


/** @brief This class allowes to manage Custom events create in the application */
class CustomEvents : public QObject
{
    Q_OBJECT

    public:
        /** @brief This enum extends the QEvent::Type */
        enum Enum : int
        {
            DecisionalEventValue = QEvent::User + 1
        };
        Q_ENUM(Enum)

    public:
        /** @brief Cast the custom event to QEvent::Type
            @param custEvent The custom event to cast
            @return The event casted or QEvent::None */
        static QEvent::Type castToEventType(Enum custEvent)
        {
            switch(custEvent)
            {
                case DecisionalEventValue:
                    return static_cast<QEvent::Type>(custEvent);
            }

            return QEvent::None;
        }

        /** @brief Get all the custom and available elements
            @return The custom elements */
        static QVector<Enum> getAvailableElements();

        /** @brief Register the custom event types
            @note This function has to be called at the application starts */
        static void registerEventTypes();
};
