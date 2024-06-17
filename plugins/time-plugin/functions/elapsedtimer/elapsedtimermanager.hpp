// SPDX-FileCopyrightText: 2020 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QHash>

class QElapsedTimer;


/** @brief This manager allowes to create and manage Elapsed timers */
class ElapsedTimerManager : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The parent class */
        explicit ElapsedTimerManager(QObject *parent = nullptr);

        /** @brief The class destructor */
        virtual ~ElapsedTimerManager() override;

        /** @brief Create an elapsed timer and return an unique token to point it
            @return An unique token linked to the elapsed timer newly created */
        const QString &createAnElapsedTimer();

        /** @brief Fire an elapsed timer pointed by the token
            @param elapsedToken The token which points the elapsed timer to fire
            @return True if no problem occurs */
        bool fireAnElapsedTimer(const QString &elapsedToken);

        /** @brief Get an elapsed timer thanks to its token
            @param elapsedToken The token of the elapsed timer to get
            @return The elapsed timer linke to the token given or nullptr if no elapsed timer has
                    been found with */
        const QElapsedTimer *getAnElapsedTimer(const QString &elapsedToken) const;

        /** @brief Remove an elapsed timer, find thanks to its elapsed timer
            @param elapsedToken The token linked to the elapsed timer */
        void removeElapsedTimer(const QString &elapsedToken);

    private:
        QHash<QString, QElapsedTimer*> _elapsedTimers;
};
