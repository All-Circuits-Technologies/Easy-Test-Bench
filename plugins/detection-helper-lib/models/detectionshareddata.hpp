// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>


/** @brief This represents the data to shared in the detection mixin
    @note This class can be used as it is when you have no data to pass */
class DetectionSharedData : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The parent instance constructor */
        explicit DetectionSharedData(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~DetectionSharedData() override;
};
