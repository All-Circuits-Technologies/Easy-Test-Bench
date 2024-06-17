// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

class DbManager;
class Permission;
class User;


/** @brief This asker allows to formulate queries related to the Permission class in the database */
class DbPermissionAsker : public QObject
{
   Q_OBJECT

    public:
        /** @brief Class constructor
            @param dbmanager A DbManager reference
            @param parent Optional Qt parentship */
        explicit DbPermissionAsker(DbManager &dbManager, QObject* parent = nullptr);

    public:
        /** @brief Retrieve all permissions of a user registered in the database
            @param user The user
            @param permissions Vector which contains permissions found
            @return True if success, False if failure */
        bool getAllUserPermissions(const QSharedPointer<User> &user,
                                   QVector<QSharedPointer<Permission>> &permissions) const;
    private:
        static const constexpr char *selectUserPermissions =
                "SELECT * FROM permissions p WHERE EXISTS ("
                "SELECT * FROM  permissionRoles "
                "WHERE p.name = permissionName AND roleName = '%1')";

    private :
        DbManager &_dbManager;
};
