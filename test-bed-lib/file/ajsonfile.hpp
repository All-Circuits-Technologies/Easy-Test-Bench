// SPDX-FileCopyrightText: 2019 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "testbedcore/testbedglobal.hpp"

class QFile;


/** @brief Abstract class for all the files managed by the library, contains the common elements and
           helpful methods */
class TESTBEDLIB_DLLSPEC AJsonFile : public QObject
{
    public:
        /** @brief Class constructor
            @param jsonFile The file to get informations from
            @param parent The class parent */
        explicit AJsonFile(QFile *jsonFile, QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~AJsonFile() override = default;

    public:
        /** @brief Get the file linked to this json file */
        const QFile *getFile() const { return _file; }

        /** @brief Get the absolute file path
            @note If the file linked is nullptr, the method will return a null string
            @note The method doesn't test if the file exists
            @return The absolute file path or null string if the file is null */
        QString getAbsoluteFilePath() const;

        /** @brief Get the absolute directory path where the file is
            @note If the file linked is nullptr, the method will return a null sring
            @note The method doesn't test if the file exists
            @return The absolute directory path or null string if the file is null */
        QString getAbsoluteDirPath() const;

    protected:
        /** @brief Set the file linked to this class */
        void setFile(QFile *file);

        /** @brief Access the file linked to this class */
        QFile &accessFile() { return *_file; }

        /** @brief Set the alias path, the path of the file used to generate this file */
        void setAliasPath(const QString &aliasPath) { _aliasPath = aliasPath; }

        /** @brief Get the alias path, the path of the file used to generate this file */
        const QString &getAliasPath() const { return _aliasPath; }

    private:
        QFile *_file{nullptr};
        QString _aliasPath{};
};
