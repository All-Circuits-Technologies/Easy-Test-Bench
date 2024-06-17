// SPDX-FileCopyrightText: 2021 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "blocs/afileselectionbloc.hpp"

#include "blocs/companions/errorhandlercompanion.hpp"
#include "blocs/companions/menucompanion.hpp"
#include "errors/fileloadingerrorhandler.hpp"
#include "models/menuitemmodel.hpp"
#include "test-bed-lib/core/globalmanager.hpp"
#include "test-bed-lib/core/settingsmanager.hpp"
#include "test-bed-lib/models/project.hpp"
#include "test-bed-lib/project/projectmanager.hpp"

#include "yamlutility/yamltojsonmanager.hpp"

#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <QQmlEngine>


AFileSelectionBloc::AFileSelectionBloc(FileLoadingErrorHandler *fileLoadingErrorHandler,
                                       bool manageProjectMissingFile,
                                       QObject *parent) :
    ABasePageBloc(parent),
    _fileListModel(new GenericListModel<FileListItemModel>(this)),
    _errorHandlerCompanion(new ErrorHandlerCompanion(this)),
    _projectManager(GlobalManager::instance().accessProjectMngr()),
    _fileLoadingErrorHandler(fileLoadingErrorHandler),
    _folderPath(QUrl::fromLocalFile(QStandardPaths::writableLocation(
                                        QStandardPaths::HomeLocation))),
    _manageProjectMissingFile(manageProjectMissingFile)
{
}

AFileSelectionBloc::AFileSelectionBloc(QObject *parent) :
    ABasePageBloc(parent),
    _projectManager(GlobalManager::instance().accessProjectMngr())
{
}

AFileSelectionBloc::~AFileSelectionBloc()
{
    delete _fileLoadingErrorHandler;
}

ScrollablePopUpModel *AFileSelectionBloc::getPopUpModel() const
{
    return _errorHandlerCompanion->getPopUpModel();
}

void AFileSelectionBloc::setCurrentlySelected(int currentlySelected)
{
    if(_currentlySelected != currentlySelected)
    {
        _currentlySelected = currentlySelected;
        emit currentlySelectedUpdated(currentlySelected);
    }
}

bool AFileSelectionBloc::isYamlParsingEnabled() const
{
    return GlobalManager::instance().accessYamlMngr().isValid();
}

void AFileSelectionBloc::onFileSelectedFromFileExplorer(const QUrl &projectFileUrl)
{
    QFileInfo qFileInfo(projectFileUrl.toLocalFile());

    QSharedPointer<Project> project = QSharedPointer<Project>::create();
    project->setPath(qFileInfo.absoluteFilePath());

    loadProject(project);
}

void AFileSelectionBloc::onOpenButtonClicked()
{
    Q_UNIMPLEMENTED();
}

void AFileSelectionBloc::onCancelButtonClicked()
{
    Q_UNIMPLEMENTED();
}

void AFileSelectionBloc::initBloc()
{
    QString projectFolderPath;
    if(GlobalManager::instance().getSettingsMngr().read(
           KnownSettings::ProjectFileDefaultPath,
           projectFolderPath) &&
       !projectFolderPath.isEmpty())
    {
        // There is a default project folder, we try to use it
        QDir dirPath(projectFolderPath);
        if(dirPath.exists())
        {
            // The dir path exists; therefore we can use it
            setFolderPath(QUrl::fromLocalFile(dirPath.absolutePath()));
        }
    }

    ABasePageBloc::initBloc();
}

void AFileSelectionBloc::fillFileMenu()
{
    MenuItemModel *openItemModel = MenuCompanion::getMenuItem(MenuItemName::open, this);
    connect(openItemModel,  &MenuItemModel::clicked,
            this,           &AFileSelectionBloc::openFileExplorer);

    getFileMenuModel()->append(openItemModel);
    getFileMenuModel()->append(MenuCompanion::getMenuItem(MenuItemName::history, this));

    ABasePageBloc::fillFileMenu();
}

void AFileSelectionBloc::setFormButtonsEnabled(bool enabled)
{
    if(_formButtonsEnabled != enabled)
    {
        _formButtonsEnabled = enabled;
        emit formButtonsEnabledUpdated(_formButtonsEnabled);
    }
}

void AFileSelectionBloc::setFolderPath(const QUrl &folderPath)
{
    if(folderPath != _folderPath)
    {
        _folderPath = folderPath;
        emit folderPathUpdated(_folderPath);
    }
}

bool AFileSelectionBloc::loadProject(QSharedPointer<Project> &project)
{
    if(!getProjectManager().loadProject(project, _fileLoadingErrorHandler))
    {
        _errorHandlerCompanion->popUpBtnsFormat(_manageProjectMissingFile &&
                                            _fileLoadingErrorHandler->isMissingProjectFileError());

        _errorHandlerCompanion->getPopUpModel()->fireDisplayPopUp(
                                                    _errorHandlerCompanion->formatErrorMessages(
                                                        _fileLoadingErrorHandler->getMessages()));
        _fileLoadingErrorHandler->clearErrors();
        return false;
    }

    return true;
}

void AFileSelectionBloc::registerQmlTypes()
{
    qmlRegisterType<AFileSelectionBloc>("app.blocs", 1, 0, "AFileSelectionBloc");
}
