// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "loadingstatus.hpp"


void LoadingStatus::RegisterMetaType()
{
    qRegisterMetaType<LoadingStatus::Enum>("LoadingStatus::Enum");
}
