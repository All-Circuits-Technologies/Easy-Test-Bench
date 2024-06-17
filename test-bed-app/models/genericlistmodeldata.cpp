// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "models/genericlistmodeldata.hpp"

#include <QQmlEngine>
#include <QMetaProperty>

const char* GenericListModelData::Qt4SignalPattern = SIGNAL(%1);


GenericListModelData::GenericListModelData(QObject *parent) :
    QAbstractListModel(parent)
{
}

GenericListModelData::GenericListModelData(const QString &targettedClassName, QObject *parent) :
    QAbstractListModel(parent),
    _targettedClassName(targettedClassName)
{
}

QVariant GenericListModelData::data(const QModelIndex &index, int role) const
{
    QObject *item = _itemList.at(index.row());
    return item->property(_propertiesInfos[role].propertyName);
}

bool GenericListModelData::setData(const QModelIndex &index, const QVariant &value, int role)
{
    QObject *item = _itemList.at(index.row());
    item->setProperty(_propertiesInfos[role].propertyName, value);

    emit dataChanged(index, index, { role });

    return true;
}

int GenericListModelData::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return _itemList.size();
}

QHash<int, QByteArray> GenericListModelData::roleNames() const
{
    if(_itemList.isEmpty())
    {
        // If the item list is empty, we don't know the roles
        return {};
    }

    if(_propertiesInfos.isEmpty())
    {
        fillPropertyInfos(*_itemList.first(), _targettedClassName, _propertiesInfos);
    }

    return getRoles(_propertiesInfos);
}

bool GenericListModelData::removeRows(int row, int count, const QModelIndex &parent)
{
    int length = _itemList.length();
    if(row >= length || row < 0)
    {
        qWarning() << "The row: " << row << ", overflows the item list length: " << length;
        return false;
    }

    int lastIdx = (row + count) - 1;

    if(count < 0 || lastIdx >= length)
    {
        qWarning() << "The last row idx: " << lastIdx << ", overflows the item list length: "
                   << length;
        return false;
    }

    beginRemoveRows(parent, row, lastIdx);

    for(int idx = row; idx <= lastIdx; ++idx)
    {
        _itemList.at(idx)->deleteLater();
    }

    _itemList.remove(row, count);

    endRemoveRows();
    return true;
}

bool GenericListModelData::removeElement(QObject *element)
{
    int idx = _itemList.indexOf(element);

    if(idx < 0)
    {
        // The element hasn't been found, it's like it's already removed
        return true;
    }

    return removeRow(idx);
}

void GenericListModelData::appendItem(QObject *item)
{
    if (_propertiesInfos.isEmpty())
    {
        fillPropertyInfos(*item, _targettedClassName, _propertiesInfos);
    }

    //Connect each notify() signals to the onDataChanged() slot which call the dataChanged() signal
    QHash<int, ObjectMetaMapping>::const_iterator citer = _propertiesInfos.cbegin();
    for(; citer != _propertiesInfos.cend(); ++citer)
    {
        const ObjectMetaMapping &objectMeta = citer.value();
        if(objectMeta.notifyIdx != -1)
        {
            connect(item, objectMeta.signalName.constData(), this, SLOT(onDataChanged()));
        }
    }

    //Finally append the item the list
    int listLength = _itemList.length();
    beginInsertRows(QModelIndex(), listLength, listLength);
    _itemList.append(item);
    endInsertRows();

    emit countChanged();
}

void GenericListModelData::onDataChanged()
{
    int listIdx = _itemList.indexOf(sender());
    QModelIndex index = createIndex(listIdx, 0);

    QVector<int> roles;
    int propertyIdx = getPropertyIndex(senderSignalIndex());

    if(propertyIdx >= 0)
    {
        roles.append(propertyIdx);
    }

    emit dataChanged(index, index, roles);
}

int GenericListModelData::getPropertyIndex(int signalIndex)
{
    QHash<int, ObjectMetaMapping>::ConstIterator citer = _propertiesInfos.cbegin();
    for(; citer != _propertiesInfos.cend(); ++citer)
    {
        if(citer.value().notifyIdx == signalIndex)
        {
            return citer.key();
        }
    }

    return -1;
}

void GenericListModelData::fillPropertyInfos(QObject &elementToAnalyze,
                                             const QString &targettedClassName,
                                             QHash<int, ObjectMetaMapping> &propertyInfo)
{
    const QMetaObject *metaObject = elementToAnalyze.metaObject();

    while(!targettedClassName.isEmpty() && metaObject->className() != targettedClassName)
    {
        metaObject = metaObject->superClass();
    }

    if(metaObject == nullptr)
    {
        metaObject = elementToAnalyze.metaObject();
        qWarning() << "We searched the targetted class name: " << targettedClassName
                   << ", but nothing has been found, we use the current MetaObject: "
                   << metaObject->className();
    }

    const int propertyCount = metaObject->propertyCount();
    for(int idx = 0; idx < propertyCount; idx++)
    {
        QMetaProperty property = metaObject->property(idx);
        ObjectMetaMapping meta{};
        meta.propertyName = property.name();

        if(property.hasNotifySignal())
        {
            meta.signalName = QString(Qt4SignalPattern)
                                            .arg(QString(property.notifySignal().methodSignature()))
                                            .toLatin1();
            meta.notifyIdx = property.notifySignalIndex();
        }

        propertyInfo.insert(idx, meta);
    }
}

QHash<int, QByteArray> GenericListModelData::getRoles(
                        const QHash<int, GenericListModelData::ObjectMetaMapping> &propertiesInfo)
{
    QHash<int, QByteArray> roles;
    roles.reserve(propertiesInfo.size());

    QHash<int, GenericListModelData::ObjectMetaMapping>::const_iterator citer =
                                                                            propertiesInfo.cbegin();

    for(; citer != propertiesInfo.cend(); ++citer)
    {
        roles.insert(citer.key(), citer.value().propertyName);
    }

    return roles;
}



bool GenericListModelData::clearAll()
{
    if(_itemList.isEmpty())
    {
        return true;
    }

    return removeRows(0, _itemList.length());
}
