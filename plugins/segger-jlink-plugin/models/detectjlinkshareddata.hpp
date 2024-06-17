// SPDX-FileCopyrightText: 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "detection-helper-lib/models/detectionshareddata.hpp"


class DetectJLinkSharedData : public DetectionSharedData
{
    Q_OBJECT

    public:
        explicit DetectJLinkSharedData(const QString &jLinkPgmPath, QObject *parent = nullptr);

        virtual ~DetectJLinkSharedData() override;

    public:
        const QString &getJLinkPgmPath() const { return _jLinkPgmPath; }

    private:
        QString _jLinkPgmPath;
};
