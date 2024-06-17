// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>
#include "threadutility/basethreadhandler.hpp"

#include "autoanswer/processes/autoanswerseriallinkthread.hpp"


/** @brief This is a useful @ref BaseThreadHandler to manage the @ref AutoAnswerSerialLinkThread
           thread deletion automatically with class destroying */
class AutoAnsSerialHandler : public QObject, public BaseThreadHandler<AutoAnswerSerialLinkThread>
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The parent instance class */
        explicit AutoAnsSerialHandler(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~AutoAnsSerialHandler() override;
};
