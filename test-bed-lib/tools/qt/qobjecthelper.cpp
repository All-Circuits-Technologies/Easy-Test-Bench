// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "qobjecthelper.hpp"

#include <QDebug>
#include <QObject>
#include <QMetaObject>
#include <QMetaProperty>
#include <QVariantMap>


bool QObjectHelper::getAllProperties(Properties &result,
                                     const QObject *obj,
                                     bool exceptQObjectBase)
{
    if(!obj)
    {
        qWarning() << "Unexpected null object";
        return false;
    }

    return getAllProperties(result,
                            static_cast<const void*>(obj),
                            obj->metaObject(),
                            exceptQObjectBase ? &QObject::staticMetaObject : nullptr);
}

bool QObjectHelper::getAllProperties(Properties &result,
                                     const void *obj,
                                     const QMetaObject *objMeta,
                                     const QMetaObject *exceptBaseMeta)
{
    result.clear();

    if(!obj || !objMeta)
    {
        qWarning() << "Unexpected null parameter";
        return false;
    }

    const bool isQObject = objMeta->inherits(&QObject::staticMetaObject);
    const bool ignoreBase = (exceptBaseMeta && objMeta->inherits(exceptBaseMeta));
    const int firstPropertyToReturn = ignoreBase ? exceptBaseMeta->propertyCount() : 0;
    for(int i = firstPropertyToReturn; i < objMeta->propertyCount(); ++i)
    {
        const QMetaProperty property = objMeta->property(i);

        if(isQObject)
        {
            result.append(qMakePair(QString(property.name()),
                                    property.read(reinterpret_cast<const QObject*>(obj))));
        }
        else
        {
            result.append(qMakePair(QString(property.name()),
                                    property.readOnGadget(obj)));
        }
    }

    return true;
}
