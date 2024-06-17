// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "backresponsecategory.hpp"


void BackResponseCategory::registerMetaType()
{
    qRegisterMetaType<BackResponseCategory::Enum>("BackResponseCategory::Enum");
}
