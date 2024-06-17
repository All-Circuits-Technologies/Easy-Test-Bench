// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "gpib4881visasetup.hpp"

#include "qtvisacomlib/src/visacommanager.hpp"
#include "qtvisacomlib/src/visacomgpib4881.hpp"


Gpib4881VisaSetup::Gpib4881VisaSetup(QObject *parent) :
    GpibVisaSetup(parent)
{
}

QSharedPointer<AVisacom> Gpib4881VisaSetup::createAndSetupItf()
{
    return VisacomManager::getInstance().createAndOpenGpib4881(getGpibNumber(), getGpibAddress());
}
