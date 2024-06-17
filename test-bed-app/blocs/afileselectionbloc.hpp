// SPDX-FileCopyrightText: 2021 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "blocs/abasepagebloc.hpp"

#include "models/filelistitemmodel.hpp"
#include "models/genericlistmodel.hpp"
#include "models/scrollablepopupmodel.hpp"

#include <QUrl>

class ErrorHandlerCompanion;
class FileListItemModel;
class FileLoadingErrorHandler;
class Project;
class ProjectManager;


/** @brief Backend class linked with file selection pages */
class AFileSelectionBloc : public ABasePageBloc
{
    Q_OBJECT
    Q_PROPERTY(GenericListModel<FileListItemModel> *fileListModel
               READ getFileListModel
               NOTIFY blocInitialized)
    Q_PROPERTY(ScrollablePopUpModel *popUpModel READ getPopUpModel CONSTANT)
    Q_PROPERTY(QUrl folderPath READ getFolderPath NOTIFY folderPathUpdated)
    Q_PROPERTY(int currentlySelected READ getCurrentlySelected
                                     WRITE setCurrentlySelected
                                     NOTIFY currentlySelectedUpdated)
    Q_PROPERTY(bool formButtonsEnabled READ isFormButtonsEnabled NOTIFY formButtonsEnabledUpdated)
    Q_PROPERTY(bool yamlParsingEnabled READ isFormButtonsEnabled CONSTANT)

    public:
        /** @brief Class constructor
            @param fileLoadingErrorHandler The file loading error handler
            @param manageProjectMissingFile Useful to indicate if we manage the case where the
                                            project file is missing (if yes, a Pop Up is displayed
                                            to offer the removing of the file with different
                                            buttons)
            @param parent The parent object */
        explicit AFileSelectionBloc(FileLoadingErrorHandler *fileLoadingErrorHandler,
                                    bool manageProjectMissingFile,
                                    QObject *parent = nullptr);

        /** @brief Default class constructor
            @note This constructor is required to use the class in QML, but in reality, objects
                  created with this constructor are never used (and cannot be used)
            @param parent The parent object */
        explicit AFileSelectionBloc(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~AFileSelectionBloc() override;

    public:
        /** @brief Get the project list model */
        GenericListModel<FileListItemModel> *getFileListModel() const { return _fileListModel; }

        /** @brief Get the model linked to the pop up which can be displayed */
        ScrollablePopUpModel *getPopUpModel() const;

        /** @brief Get the folder path */
        const QUrl &getFolderPath() const { return _folderPath; }

        /** @brief Get the index of the currently selected file in the list view */
        int getCurrentlySelected() const { return _currentlySelected; }

        /** @brief Set the index of the currently selected file in the list view */
        void setCurrentlySelected(int currentlySelected);

        /** @brief Say if the submit button has to be enabled */
        bool isFormButtonsEnabled() const { return _formButtonsEnabled; }

        /** @brief Returns true if the YAML parsing is usable */
        bool isYamlParsingEnabled() const;

    public slots:
        /** @brief Called when a project is selected from the file explorer
            @param projectFileUrl Url of the project file selected */
        void onFileSelectedFromFileExplorer(const QUrl &projectFileUrl);

        /** @brief Called when open button is clicked
            @param index The index of the sequence selected */
        virtual void onOpenButtonClicked();

        /** @brief Called when open button is clicked */
        virtual void onCancelButtonClicked();

    protected slots:
        /** @see AbstractBloc::initBloc */
        virtual void initBloc() override;

    protected:
        /** @brief Get a ProjectManager Reference */
        ProjectManager &getProjectManager() const { return _projectManager; }

        /** @brief Access the error handler */
        FileLoadingErrorHandler &accessErrorHandler() { return *_fileLoadingErrorHandler; }

        /** @brief Access to the error companion */
        ErrorHandlerCompanion &getErrorCompanion() const { return *_errorHandlerCompanion; }

        /** @brief Load a project
            @param project The project Loaded */
        virtual bool loadProject(QSharedPointer<Project> &project);

        /** @brief Fill the file menu with the proper items */
        virtual void fillFileMenu() override;

        /** @brief Enable/disable the form buttons
            @param enabled The new enable value of the form buttons */
        void setFormButtonsEnabled(bool enabled);

    private:
        /** @brief Set the folder path */
        void setFolderPath(const QUrl &folderPath);

    signals :
        /** @brief Emited when user click on open menu item */
        void openFileExplorer();

        /** @brief Emitted when the form buttons enabled value has been updated
            @param formButtonsEnabled The enable value of the form buttons */
        void formButtonsEnabledUpdated(bool formButtonsEnabled);

        /** @brief Emitted when the selection has been updated
            @param currentlySelected The currently selected index */
        void currentlySelectedUpdated(int currentlySelected);

        /** @brief Emitted when the folder path has changed
            @param folderPath The updated folder path */
        void folderPathUpdated(const QUrl &folderPath);

    public:
        /** @brief Register QML type linked to this bloc */
        static void registerQmlTypes();

    private:
        GenericListModel<FileListItemModel> *_fileListModel{nullptr};
        ErrorHandlerCompanion *_errorHandlerCompanion{nullptr};
        ProjectManager &_projectManager;
        FileLoadingErrorHandler *_fileLoadingErrorHandler{nullptr};
        QUrl _folderPath;

        bool _manageProjectMissingFile{false};
        int _currentlySelected{-1};
        bool _formButtonsEnabled{true};
};
