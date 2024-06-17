// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>
#include "threadutility/basethreadhandler.hpp"

#include "process/autoansprocessthread.hpp"

class AutoAnsProcessThread;


/** @brief This class creates a @ref AutoAnsThreadHandler instance in its constructor and stop and
           delete later the thread in its constructor.
    @note This class is useful to keep a link to a thread and manage its destruction with Qt auto
          destruction as QSharedPointer */
class AutoAnsThreadHandler : public QObject, public BaseThreadHandler<AutoAnsProcessThread>
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @note Create an @ref AutoAnsProcessThread
            @param parent The parent instance */
        explicit AutoAnsThreadHandler(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~AutoAnsThreadHandler() override;
};
