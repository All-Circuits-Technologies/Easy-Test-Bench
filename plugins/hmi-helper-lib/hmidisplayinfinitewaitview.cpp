// SPDX-FileCopyrightText: 2020 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "hmidisplayinfinitewaitview.hpp"

#include "bankcoreshared/usebyplugin/icommoninterface.hpp"


HmiDisplayInfiniteWaitView::HmiDisplayInfiniteWaitView(ICommonInterface &commonItf,
                                                       QObject *parent) :
    HmiDisplaySimpleTestView(commonItf, parent)
{
}

HmiDisplayInfiniteWaitView::~HmiDisplayInfiniteWaitView()
{
}

bool HmiDisplayInfiniteWaitView::displayInfiniteHmi(const QString &name,
                                                    const QString &description,
                                                    bool displayCancelButton)
{
    return displayHmi(name, description, displayCancelButton ? CancelButton : 0, defaultImg);
}
