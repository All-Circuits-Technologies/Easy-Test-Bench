// SPDX-FileCopyrightText: 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "detection-helper-lib/models/detectionshareddata.hpp"


class DetectAtmelSharedData : public DetectionSharedData
{
    Q_OBJECT

    public:
        explicit DetectAtmelSharedData(const QString &atPgmPath,
                                       QObject *parent = nullptr);

        virtual ~DetectAtmelSharedData() override;

    public:
        const QString &getAtPgmPath() const { return _atPgmPath; }

    private:
        QString _atPgmPath;
};
