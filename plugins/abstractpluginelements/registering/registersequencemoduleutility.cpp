// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "registersequencemoduleutility.hpp"


RegisterSequenceModuleUtility::RegisterSequenceModuleUtility() :
    RegisterModuleUtility()
{
}

RegisterSequenceModuleUtility &RegisterSequenceModuleUtility::Instance()
{
    if(_instance == nullptr)
    {
        _instance = new RegisterSequenceModuleUtility();
    }

    return *_instance;
}
