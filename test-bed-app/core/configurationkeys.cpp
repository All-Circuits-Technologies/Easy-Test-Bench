// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "core/configurationkeys.hpp"

#include <QMetaEnum>


QString ConfigurationKeys::toString(ConfigurationKeys::Enum configurationKey)
{
    return QString::fromLatin1(QMetaEnum::fromType<Enum>().valueToKey(configurationKey));
}
