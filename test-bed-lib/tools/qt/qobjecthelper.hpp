// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QList>
#include <QString>
#include <QVariant>

class QObject;
struct QMetaObject;


/** @brief This namespace provide some useful methods around QObject */
namespace QObjectHelper
{
    /** A simple property agregates its name (key) and its value */
    typedef QPair<QString, QVariant> Property;
    /** Set of properties Key->Value.
     *  We do not want a simple QVariantMap because key ordering may be important. */
    typedef QList<Property> Properties;

    /** @brief Easily get all properties of a QObject
        @param[out] result A Map property name -> property value.
                            This map is ordered by key lexical ascending order,
                            not by property declaration in object.
                            May we want to return a QList<QPair<QString, QVariant>> in the future?
        @param obj QObject to extract properties of
        @param exceptQObjectBase Should QObject properties (objectName) be returned or not
        @return True upon success, false upon issue */
    bool getAllProperties(Properties &result, const QObject *obj, bool exceptQObjectBase);

    /** @brief Easily get all properties of a Q_OBJECT or a Q_GADGET
        @param[out] result A Map property name -> property value.
                            This map is ordered by key lexical ascending order,
                            not by property declaration in object.
                            May we want to return a QList<QPair<QString, QVariant>> in the future?
        @param obj Object to extract properties of
        @param objMeta Meta object of obj, as returned by T::staticMetaObject
        @param exceptBaseMeta Optional base to exclude properties up to
                (ex QObject::staticMetaObject, must be a super-class).
                Silently ignored if objMeta is not a subclass of exceptBaseMeta.
        @return True upon success, false upon issue */
    bool getAllProperties(Properties &result,
                          const void *obj,
                          const QMetaObject *objMeta,
                          const QMetaObject *exceptBaseMeta = nullptr);
};
