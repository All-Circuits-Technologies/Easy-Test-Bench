// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "services/sizemanager.hpp"


SizeManager::SizeManager(int appHeight,
                         int appWidth,
                         int mockupHeight,
                         int mockupWidth,
                         QObject *parent) :
    QObject(parent),
    _appHeight(appHeight),
    _appWidth(appWidth),
    _mockupHeight(mockupHeight),
    _mockupWidth(mockupWidth)
{
}

void SizeManager::setAppHeight(int appHeight)
{
    if(appHeight != _appHeight)
    {
        _appHeight = appHeight;
        emit appHeightUpdated(appHeight);
    }
}

void SizeManager::setAppWidth(int appWidth)
{
    if(appWidth != _appWidth)
    {
        _appWidth = appWidth;
        emit appWidthUpdated(appWidth);
    }
}
