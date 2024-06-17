// SPDX-FileCopyrightText: 2020 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>


/** @brief The HMI display status
    @note The value of enum can be tested, if the Enum value is above the value of Enum::Displaying
          it means that the HMI ended (in error or successfully) */
class HmiDisplayStatus : public QObject
{
    Q_OBJECT

    public:
        enum Enum : quint8
        {
            Init        = 0x00,
            Displaying  = 0x01,
            Success     = 0x02,     /**< @brief This implies that the displaying is successfully
                                                finished */
            Error       = 0x03      /**< @brief This implies that the displaying has finished on
                                                error */
        };
        Q_ENUM(Enum)
};
