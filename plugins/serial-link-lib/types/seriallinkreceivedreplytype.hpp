// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>


/** @brief The current status of a received answer */
class SerialLinkReceivedReplyType : public QObject
{
    Q_OBJECT

    public:
    /** @brief Give the current status of the received answer (if it has been received or not,
               and if it has been received, is it a success or a failure */
        enum Enum
        {
            ExpectedAnswer,
            FailureAnswer,
            NoAnswer
        };
        Q_ENUM(Enum)
};
