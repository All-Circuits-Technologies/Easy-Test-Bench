// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>


/** @brief Class that manages the content of a file list view item */
class FileListItemModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString uniquePath READ getUniquePath WRITE setUniquePath NOTIFY uniquePathChanged)
    Q_PROPERTY(bool missingFile READ isMissingFile WRITE setMissingFile NOTIFY missingFileChanged)

    public:
        /** @brief Class constructor
            @param parent The optional QObject parent */
        FileListItemModel(QObject *parent = nullptr);

    public:
        /** @brief Get the name displayed on the file item */
        const QString &getName() const { return _name; }

        /** @brief Set the name displayed on the file item */
        void setName(const QString &name);

        /** @brief Get the unqiue path displayed on the item */
        const QString &getUniquePath() const { return _uniquePath; }

        /** @brief Set the unqiue path displayed on the item */
        void setUniquePath(const QString &uniquePath);

        /** @brief Get boolean that informs if the file related to the item is missing */
        bool isMissingFile() const { return _missingFile; }

         /** @brief Set boolean that informs if the file related to the item is missing */
        void setMissingFile(bool missingFile);

    signals:
        /** @brief Emitted when the name has changed
            @param name The new name */
        void nameChanged(const QString &name);

        /** @brief Emitted when the unique path has changed
            @param pathChanged The new path */
        void uniquePathChanged(const QString &pathChanged);

        /** @brief Emitted when the missing file option has changed
            @param missingFile The new value of the option */
        void missingFileChanged(bool missingFile);

    private:
        QString _name;
        QString _uniquePath;
        bool _missingFile{false};
};
