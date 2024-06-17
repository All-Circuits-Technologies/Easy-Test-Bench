// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "customevents.hpp"

#include <QDebug>
#include <QMetaEnum>


QVector<CustomEvents::Enum> CustomEvents::getAvailableElements()
{
    QVector<Enum> enumList;
    const QMetaEnum metaEnum = QMetaEnum::fromType<Enum>();

    for(int idx = 0; idx < metaEnum.keyCount(); idx++)
    {
        enumList.append(static_cast<Enum>(metaEnum.value(idx)));
    }

    return enumList;
}

void CustomEvents::registerEventTypes()
{
    QVector<CustomEvents::Enum> elements = getAvailableElements();
    for(auto citer = elements.cbegin(); citer != elements.cend(); ++citer)
    {
        int realEventType = QEvent::registerEventType(*citer);
        if(realEventType != *citer)
        {
            qWarning() << "Tryied to register the event type to: " << *citer << ", but the "
                       << "value is already used and the registering method returns: "
                       << realEventType;
        }
    }
}
