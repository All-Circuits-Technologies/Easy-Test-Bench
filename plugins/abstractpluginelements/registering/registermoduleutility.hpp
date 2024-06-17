// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QHash>
#include <QMetaObject>


/** @brief This class helps to save registered modules. It's used with the classes:
 *         @ref RegisterSequenceModuleUtility and @ref RegisterTopologyModuleUtility */
class RegisterModuleUtility
{
    protected:
        /** @brief Class constructor */
        RegisterModuleUtility() {}

    public:
        /** @brief Get the registered modules */
        const QHash<QString, QMetaObject> &getRegisteredModules() const
        { return _registeredModules; }

    protected:
        /** @brief This method is used to insert registered module to the list
            @note The name (which is used as key) is got from the QMetaObject className
            @param metaObject The QMetaObject registered */
        void insertInList(const QMetaObject &metaObject);

    private:
        QHash<QString, QMetaObject> _registeredModules;
};
