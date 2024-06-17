// SPDX-FileCopyrightText: 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "detectatmelshareddata.hpp"


DetectAtmelSharedData::DetectAtmelSharedData(const QString &atPgmPath, QObject *parent)
    : DetectionSharedData{parent},
    _atPgmPath{atPgmPath}
{
}

DetectAtmelSharedData::~DetectAtmelSharedData()
{
}
