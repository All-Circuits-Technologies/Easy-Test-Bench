// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "bmsgadgetjsonloader.hpp"

#include "collectionutility/qvariantlisthelper.hpp"
#include "jsonutility/jsonhelper.hpp"
#include "testbedcore/parsers/jsonparsersconstants.hpp"
#include "testbedcore/testbedglobal.hpp"
#include "tools/qt/bmsgadget.hpp"

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QMetaProperty>
#include <QMetaType>


BmsGadgetJsonLoader::BmsGadgetJsonLoader(QObject *parent) :
    AJsonParser (parent)
{ }

bool BmsGadgetJsonLoader::createObject(const QJsonObject &jsonObject,
                                       BmsGadget **dest,
                                       const QMetaObject *defaultDestMeta,
                                       const QMetaObject **effectiveMeta)
{
    // Note to developer: this method contains multiple returns
    if(!dest)
    {
        AJSONPARSER_LOG_ERR(QString("'dest' output argument is mandatory and can't be null"));
        return false;
    }
    *dest = nullptr;
    if(effectiveMeta)
    {
        *effectiveMeta = nullptr;
    }
    clearError();

    /* Find target meta-type */
    // Any meta type announced in JSON node overrides given destMeta.
    const QMetaObject *destMeta = nullptr;
    QString jsonRequestedTypeName;
    (void) JsonHelper::getString(jsonObject,
                                 JsonParsersConstants::Generic::MetaType,
                                 jsonRequestedTypeName,
                                 true);
    if(!jsonRequestedTypeName.isEmpty())
    {
        destMeta = QMetaType::metaObjectForType(
                              QMetaType::type(jsonRequestedTypeName.toLocal8Bit().constData()));
        if(!destMeta)
        {
            AJSONPARSER_LOG_ERR(QString("Failed to get %1 metaobject (JSON source)")
                                .arg(jsonRequestedTypeName));
            return false;
        }
    }
    else if(defaultDestMeta)
    {
        destMeta = defaultDestMeta;
    }
    else
    {
        AJSONPARSER_LOG_ERR(QString("No metaObject given and no hints in JSON object"));
        return false;
    }

    if(!destMeta->inherits(&BmsGadget::staticMetaObject))
    {
        AJSONPARSER_LOG_ERR(QString("For simplicity, only BmsGadget subclasses are supported"));
        return false;
    }

    /* Instanciate this type */
    // Yes, instanciation of gadgets returns a wrongly QObject* typed pointer but we cast it.
    *dest = (BmsGadget*)(destMeta->newInstance());

    if(!*dest)
    {
        AJSONPARSER_LOG_ERR(QString("Failed to instanciate %1").arg(destMeta->className()));
        return false;
    }

    /* Fill it */
    bool success = updateObject(jsonObject, **dest);

    /* Free it in case of any failure/incomplete work */
    if(!success)
    {
        /* Since dest is created from a meta, we can be confident meta exists
         * to find back dest type and destroy the allocation.
         */
        if(*dest && destMeta)
        {
            int typeId = QMetaType::type(destMeta->className());
            if(typeId != QMetaType::UnknownType)
            {
                QMetaType::destroy(typeId, *dest);
                *dest = nullptr;
            }
            else
            {
                qWarning() << "Failed to free mis-filled object. Caller will have to it itself";
            }
        }
    }

    if(!*dest)
    {
        AJSONPARSER_LOG_ERR(QString("Failed to create object type %1")
                            .arg(destMeta ? destMeta->className() : "unknown"));
    }

    if(effectiveMeta)
    {
        /* Update effective meta using the metatype used to instanciate dest object,
         * or null if we failed to create the object
         */
        *effectiveMeta = *dest ? destMeta : nullptr;
    }

    return success;
}

bool BmsGadgetJsonLoader::updateObject(const QJsonObject &jsonObject,
                                       BmsGadget &dest)
{
    bool success = true;

    /* Iterate over all JSON keys.
     * If destination object have a matching property, then affect it.
     */
    for(auto iter = jsonObject.constBegin(); iter != jsonObject.constEnd(); iter++)
    {
        const QString key = iter.key();
        const QJsonValue jValue = iter.value();

        success &= updateObjectProperty(dest, key, jValue);
    }

    return success;
}

bool BmsGadgetJsonLoader::updateObjectProperty(BmsGadget &dest,
                                               const QString &jsonKey,
                                               const QJsonValue &jsonValue)
{
    // Note to developers: this method contains multiple returns
    const QString &propName = jsonKey;
    QMetaProperty destProperty;

    if(findProperty(*dest.metaObject(), propName, destProperty))
    {
        QVariant qValue;
        if(jsonValueToProperty(jsonValue, dest, destProperty, qValue))
        {
            /* Write property using gadgets specific call */
            if(!destProperty.writeOnGadget(&dest, qValue))
            {
                AJSONPARSER_LOG_ERR(QString("Failed to write %1 gadget property %2 "
                                            "with value of type %3")
                                                                .arg(dest.metaObject()->className(),
                                                                     propName,
                                                                     QString(qValue.typeName())));
                return false;
            }
        }
        else
        {
            /* Failed to build a QVariant out of JSON value */
            AJSONPARSER_LOG_ERR(QString("Failed to update %1::%2 (conversion to type %3)")
                                                                .arg(dest.metaObject()->className(),
                                                                     propName,
                                                                     destProperty.typeName()));
            return false;
        }
    }
    else
    {
        /* No mathcing property in targetted object.
         * This is fine for now, we accept extra JSON keys in configuration files
         * Future: want we create a dynamic property (of which type?),
         *         want we enforce all properties to be statically known by destination type?
         */
        qDebug() << "Property" << propName << "not found in"
                 << dest.metaObject()->className() << "destination";
        // Not a failure
    }

    return true;
}

bool BmsGadgetJsonLoader::jsonValueToProperty(const QJsonValue &jValue,
                                              BmsGadget &dest,
                                              const QMetaProperty &destMeta,
                                              QVariant &qValue)
{
    (void) dest;  // see doxygen
    return jsonValueToVariant(jValue,
                              destMeta.userType(),
                              destMeta.typeName(),
                              qValue);
}

bool BmsGadgetJsonLoader::jsonValueToVariant(const QJsonValue &jValue,
                                             int userType,
                                             const char *typeName,
                                             QVariant &qValue)
{
    bool success = false;

    if(jValue.isNull() || jValue.isUndefined())
    {
        qValue.clear();
        success = true;
    }
    else if(jValue.isBool() || jValue.isDouble() || jValue.isString())
    {
        qValue = jValue.toVariant();
        success = true;
    }
    else if (jValue.isArray())
    {
        /* Load a Collection<Type> type.
         * Note: QStringList is currently not supported, please use a QVector<QString> instead.
         */

        /* Split container and contained types */
        QString typeNameStr(typeName);
        typeNameStr.replace('<', ' ');
        typeNameStr.replace(',', ' ');
        typeNameStr.replace('>', ' ');

        QStringList qtTypeTokens = typeNameStr.split(' ', QString::SkipEmptyParts);

        if(qtTypeTokens.count() != 2)
        {
            // Maybe a qpecial type such as QStringList
            AJSONPARSER_LOG_ERR(QString("Unsupported array type %1 "
                                        "(only simple XX<YY> is supported")
                                .arg(QString(typeName)));
        }

        /* Fill a Variant list */
        QList<QVariant> qVarCollection;
        const QString &cellsTypeName = qtTypeTokens.last();
        bool allItemsConverted = true;
        QJsonArray jArray = jValue.toArray();
        for(auto iter = jArray.constBegin(); iter != jArray.constEnd(); ++iter)
        {
            QVariant cellQValue;
            allItemsConverted &= jsonValueToVariant(
                                     *iter,
                                     QMetaType::type(cellsTypeName.toLatin1().constData()),
                                     cellsTypeName.toLatin1().constData(),
                                     cellQValue);
            qVarCollection.append(cellQValue);
        }

        /* Convert variant list to final collection type */
        qValue = QVariantListHelper::VariantListToVariant(qVarCollection, typeName);
        success = allItemsConverted;
    }
    else if(jValue.isObject())
    {
        const QJsonObject jsonObject = jValue.toObject();
        const QMetaObject *childMeta = nullptr;

        success &= getMetaObjectByTypeName(typeName, userType, &childMeta);

        if(!childMeta)
        {
            AJSONPARSER_LOG_ERR(QString("Failed to get %1 meta object").arg(typeName));
        }

        BmsGadget *childQObj = nullptr;
        const QMetaObject *effectiveChildMeta = nullptr;
        success = createObject(jsonObject,
                               &childQObj,
                               childMeta,
                               &effectiveChildMeta);
        if(success)
        {
            qValue = QVariant(QMetaType::type(effectiveChildMeta->className()), childQObj);
        }
        else
        {
            qWarning() << "create obj from" << jsonObject << "FAILED";
            qValue.clear();
        }
    }
    else
    {
        AJSONPARSER_LOG_ERR(QString("Unexpected JSON type %1").arg(jValue.type()));
        success = false;
    }

    return success;
}

bool BmsGadgetJsonLoader::findProperty(const QMetaObject &destMeta,
                                       const QString &name,
                                       QMetaProperty &prop)
{
    const QMetaObject *objMeta = &destMeta;
    const int propCount = objMeta->propertyCount();

    for(int i = 0; i < propCount; ++i)
    {
        if(QString::compare(QString::fromLocal8Bit(objMeta->property(i).name()),
                            name,
                            Qt::CaseInsensitive) == 0)
        {
            prop = objMeta->property(i);
            return true;
        }
    }

    /* No warning when not found here, it is up to the caller to know if this is an issue or not. */
    return false;
}

bool BmsGadgetJsonLoader::getMetaObjectByTypeName(const QString &typeName,
                                                  int userType,
                                                  const QMetaObject **meta)
{
    Q_UNUSED(userType)

    const int typeId = QMetaType::type(typeName.toLatin1().constData());
    const QMetaObject *metaObject = QMetaType::metaObjectForType(typeId);

    /* Fill output parameters */
    if(meta)
    {
        *meta = metaObject;
    }

    return !!metaObject;
}
