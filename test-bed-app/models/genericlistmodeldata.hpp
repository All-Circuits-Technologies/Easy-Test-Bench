// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QAbstractListModel>
#include <QHash>
#include <QDebug>


/** @brief Generic implementation of QAbstractListModel */
class GenericListModelData : public QAbstractListModel
{
    Q_OBJECT

    private:
        /** @brief Useful information linked to a specific property */
        struct ObjectMetaMapping
        {
            QByteArray propertyName;    //!< @brief Name of the property
            int notifyIdx{-1};          //!< @brief Idx of the notify signal linked to the property
            QByteArray signalName;      //!< @brief Name of the notify signal linked to the property
        };

    public:
        /** @brief Class constructor
            @param parent The optional QObject parent */
        explicit GenericListModelData(QObject *parent);

        /** @brief Class constructor
            @param parent The optional QObject parent */
        explicit GenericListModelData(const QString &targettedClassName, QObject *parent);

        /** @brief Return one element value of a given item
            @param index The index of the item to return
            @param role The data role id of the value to get (specific to the item selected by
                        index)
            @return item at chosen index*/
        virtual QVariant data(const QModelIndex &index, int role) const override;

        /** @brief Set one element value of a given item
            @param index The index of the item to return
            @param value The value to set
            @param role The data role id of the value to set (specific to the item selected by
                        index)
            @return item at chosen index*/
        virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;

        /** @brief Returns the number of items attached to the list.
            @param parent Optional QModelIndex
            @return number of items attached to the list*/
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;

        /** @brief Generates a hash out of QMetaObject property index and property name */
        virtual QHash<int, QByteArray> roleNames() const override;

        /** @brief Useful to remove rows from table
            @note The method also deletes the linked qobject elements, stores in @ref _itemList
            @param row The index of the first row to remove
            @param count The number of rows to delete from the row index given
            @param parent Optional QModelIndex
            @return True if no problem occurs */
        virtual bool removeRows(int row,
                                int count,
                                const QModelIndex &parent = QModelIndex()) override;

        /** @brief Get item list */
        const QVector<QObject *> &getItemListData() const { return _itemList; }

        /** @brief Remove an element in the list
            @param element The element to remove
            @return True if no problem occurred */
        bool removeElement(QObject *element);

        /** @brief Clear all row from table
            @note The method also deletes the linked qobject elements, stores in @ref _itemList
            @note The method is a shorcut for the @ref removeRows method
            @return True if no problem occurs */
        bool clearAll();

    signals:
        /** @brief Emitted when the number of elements in the list changed */
        void countChanged();

    protected:
        /** @brief Add an item in the list model
            @param item The item to add */
        void appendItem(QObject *item);

    private slots:
        /** @brief Helper-Slot that emit's the dataChanged() signal of QAbstractListModel */
        void onDataChanged();

    private:
        /** @brief Get the index of a property, thanks to the signal index given
            @param signalIndex The signal index to find the property
            @return The property index linked to the signal */
        int getPropertyIndex(int signalIndex);

    private:
        /** @brief The method will extract all the needed properties infos from the QObject
            @param elementToAnalyze The element to analyze and get its infos
            @param targettedClassName The targetted class name
            @param propertyInfos The information extracted from the element */
        static void fillPropertyInfos(QObject &elementToAnalyze,
                                      const QString &targettedClassName,
                                      QHash<int, ObjectMetaMapping> &propertyInfo);

        /** @brief Return an usefull hash container containing the name of property with its index
            @param propertiesInfo The information linked to the properties
            @return The information useful for managing roles */
        static QHash<int, QByteArray> getRoles(const QHash<int, ObjectMetaMapping> &propertiesInfo);

    private:
        /** @brief Usefull constant for getting the Qt4 signal pattern
            @note The constant contains an "%1" argument, usefull to add the method name in the
                  pattern */
        static const char* Qt4SignalPattern;

    private:
        QString _targettedClassName;
        QVector<QObject *> _itemList;
        mutable QHash<int, ObjectMetaMapping> _propertiesInfos;
};
