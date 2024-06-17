// SPDX-FileCopyrightText: 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "detectjlinkshareddata.hpp"


DetectJLinkSharedData::DetectJLinkSharedData(const QString &jLinkPgmPath, QObject *parent)
    : DetectionSharedData{parent},
    _jLinkPgmPath{jLinkPgmPath}
{
}

DetectJLinkSharedData::~DetectJLinkSharedData()
{
}
