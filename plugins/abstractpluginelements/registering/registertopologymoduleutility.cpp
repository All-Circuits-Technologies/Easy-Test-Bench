// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "registertopologymoduleutility.hpp"


RegisterTopologyModuleUtility::RegisterTopologyModuleUtility() :
    RegisterModuleUtility()
{
}

RegisterTopologyModuleUtility &RegisterTopologyModuleUtility::Instance()
{
    if(_instance == nullptr)
    {
        _instance = new RegisterTopologyModuleUtility();
    }

    return *_instance;
}
