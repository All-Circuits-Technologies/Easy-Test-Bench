// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphaël Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "genericlistmodeldata.hpp"

#include "collectionutility/qvectorhelper.hpp"


/** @brief A derivative of the GenericListModelData class that facilitates its use */
template <typename T>
class GenericListModel : public GenericListModelData
{
    public:
        /** @brief Class constructor
            @param parent The optional QObject parent */
        explicit GenericListModel(QObject *parent = nullptr);

        /** @brief Add an element in the list model
            @param item The item to append */
        void append(T *item);

        /** @brief Return an element of the list model
            @param index The index of the item to return
            @return item at chosen index*/
        T *at(int index);

        /** @brief Get the items list */
        QVector<T*> getItemList() const;

        /** @brief Register meta types linked to this model
            @param metaTypeString Meta type to register in string format */
        static void registerMetaTypes(const QString &metaTypeString);
};

template<typename T>
GenericListModel<T>::GenericListModel(QObject *parent) :
    GenericListModelData(T::staticMetaObject.className(), parent)
{
    static_assert(std::is_base_of<QObject, T>::value, "The class is not a QObject");
}

template<typename T>
void GenericListModel<T>::append(T *item)
{
    appendItem(item);
}

template<typename T>
T *GenericListModel<T>::at(int index) {
    return qobject_cast<T *>(getItemListData().at(index));
}

template<typename T>
QVector<T *> GenericListModel<T>::getItemList() const
{
    return QVectorHelper::castFromQObjectVector<T>(getItemListData());
}

template <typename T>
void GenericListModel<T>::registerMetaTypes(const QString &metaTypeString)
{
    QByteArray byteArray = metaTypeString.toLocal8Bit();
    const char *metaType = byteArray.data();
    qRegisterMetaType<GenericListModel<T>*>(metaType);
}
