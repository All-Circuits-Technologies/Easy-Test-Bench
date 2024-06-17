// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "models/plugins/aplugininputmodel.hpp"


/** @brief Model class linked with select file input views */
class SelectFileInputModel : public APluginInputModel
{
    Q_OBJECT
    Q_PROPERTY(bool selectDir READ isSelectDir NOTIFY selectDirChanged)
    Q_PROPERTY(QStringList filesExtensions READ getFilesExtensions
                                           NOTIFY filesExtensionsChanged)
    Q_PROPERTY(QString currentDirectory READ getCurrentDirectory NOTIFY currentDirectoryChanged)
    Q_PROPERTY(QString selectedPath READ getSelectedPath NOTIFY selectedPathChanged);

    public:
        /** @brief Class constructor
            @param parent The parent class */
        explicit SelectFileInputModel(QObject *parent = nullptr);

        /** @brief Class constructor
            @param uiToken The display ui token
            @param parent The parent widget */
        explicit SelectFileInputModel(const QSharedPointer<DisplayUiToken> &uiToken,
                                      QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~SelectFileInputModel() override;

    public:
        /** @brief Get select dir state */
        bool isSelectDir() const { return _selectDir; }

        /** @brief Get files extensions */
        QStringList getFilesExtensions() const { return _filesExtensions.toList(); }

        /** @brief Get the current directory */
        const QString &getCurrentDirectory() const { return _currentDirectory; }

        /** @brief Get the selected path */
        QString getSelectedPath() const;

    signals:
        /** @brief Emitted when the type of element has changed
            @param True if the element to select is a Dir, false if it's a file */
        void selectDirChanged(bool selectDir);

        /** @brief Emitted when files Extensions changed */
        void filesExtensionsChanged();

        /** @brief Emitted when the current directory has changed
            @param currentDirectory The new current directory */
        void currentDirectoryChanged(const QString &currentDirectory);

        /** @brief Emitted when the selected path has changed
            @param selectedPath The new selected path */
        void selectedPathChanged(const QString &selectedPath);

    protected:
        /** @brief Parse the JsonObject description, get the informations and set the values on
                   the object
            @param elemDescription The JsonObject description to parse in order to build the widget
            @param update True if the elemDescription given is for update the model or for creating
                          a new view
            @return True if no problem occurs */
        virtual bool parseAndSetAttributes(const JsonObject &elemDescription,
                                           bool update = false) override;

    private slots:
        /** @brief Call when the value is updated
            @param value The new value */
        void onValueUpdated(const QVariant &value);

    private:
        /** @brief Set select dir state
            @param True if the element to select is a Dir, false if it's a file */
        void setSelectDir(bool selectDir);

        /** @brief Set files extensions
            @param filesExtensions The files extensions to select files */
        void setFilesExtensions(const QVector<QString> &filesExtensions);

        /** @brief Set he current directory
            @param currentDirectory The current directory */
        void setCurrentDirectory(const QString &currentDirectory);

    public:
        /** @brief Register QML type linked to this model */
        static void registerQmlType();

    private:
        static const constexpr char *fileExtensionsRegExp =
                                                    R"(.* \(((\s?(\*(\.[a-zA-Z0-9]+)+)\s?)+|\*)\))";

    private:
        bool _selectDir {false};
        QVector<QString> _filesExtensions;
        QString _currentDirectory;
};
