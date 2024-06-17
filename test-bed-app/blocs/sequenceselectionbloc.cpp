// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphaël Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "blocs/sequenceselectionbloc.hpp"

#include "blocs/companions/errorhandlercompanion.hpp"
#include "errors/sequenceloadingerrorhandler.hpp"
#include "definesutility/definesutility.hpp"
#include "fileutility/filehelper.hpp"
#include "models/project.hpp"
#include "models/filelistitemmodel.hpp"
#include "project/projectmanager.hpp"
#include "services/navigation/navigationmanager.hpp"
#include "test-bed-lib/project/projectfile.hpp"
#include "test-bed-lib/project/projectmanager.hpp"
#include "test-bed-lib/sequence/sequencefile.hpp"

#include <QFileInfo>
#include <QQmlEngine>


SequenceSelectionBloc::SequenceSelectionBloc(QObject *parent) :
    AFileSelectionBloc(new SequenceLoadingErrorHandler(), false, parent)
{
    // The Ok button is also clicked when loading sequence files, but in that case we do nothing in
    // the gotToProjectSelectionPage method
    connect(getPopUpModel(), &ScrollablePopUpModel::okBtnClicked,
            this,            &SequenceSelectionBloc::goToProjectSelectionPageIfNeeded);
}

void SequenceSelectionBloc::initBloc()
{
    updateSequenceList();

    AFileSelectionBloc::initBloc();
}

void SequenceSelectionBloc::updateSequenceList()
{
    QSharedPointer<const ProjectFile> projectFile = getProjectManager().getProjects().first();

    const QMap<QString, QSharedPointer<SequenceFile>> &sequences = projectFile->getSequenceFiles();
    QMap<QString, QSharedPointer<SequenceFile>>::const_iterator citer = sequences.cbegin();

    deleteAllAndClear();

    for(; citer != sequences.cend(); ++citer)
    {
        SequenceInfo seqInfo;
        seqInfo.name = citer.key();
        seqInfo.file = citer.value();
        bool fileAlreadyOnTheList = false;
        QString uniquePath;

        for(auto cInfoIter = _sequenceInfos.cbegin(); cInfoIter != _sequenceInfos.cend(); ++cInfoIter)
        {
            if(seqInfo.name == cInfoIter->name)
            {
                if(seqInfo.file->getAbsoluteFilePath() != cInfoIter->file->getAbsoluteFilePath())
                {
                    FileHelper::getUniquePartOfAPath(seqInfo.file->getAbsoluteFilePath(),
                                                     cInfoIter->file->getAbsoluteFilePath(),
                                                     uniquePath);
                }
                else
                {
                    fileAlreadyOnTheList = true;
                    break;
                }
            }
        }
        if(!fileAlreadyOnTheList)
        {
            FileListItemModel *itemModel = new FileListItemModel(this);
            itemModel->setName(seqInfo.name);
            itemModel->setUniquePath(uniquePath);

            _sequenceInfos.append(seqInfo);
            getFileListModel()->append(itemModel);
        }
    }
}

void SequenceSelectionBloc::deleteAllAndClear()
{
    _sequenceInfos.clear();
    getFileListModel()->clearAll();
}

void SequenceSelectionBloc::onOpenButtonClicked()
{
    setFormButtonsEnabled(false);
    _selectedSequenceInfo = _sequenceInfos.at(getCurrentlySelected());
    accessErrorHandler().clearErrors();

    if(!_selectedSequenceInfo.file->loadSequenceDataAndLaunchThreads(
                                1,
                                qobject_cast<SequenceLoadingErrorHandler*>(&accessErrorHandler())))
    {
        getErrorCompanion().popUpBtnsFormat(false);
        getPopUpModel()->fireDisplayPopUp(getErrorCompanion().formatErrorMessages(
                                              accessErrorHandler().getMessages()));
        setFormButtonsEnabled(true);
    }
    else
    {
        connect(_selectedSequenceInfo.file.data(), &SequenceFile::allThreadsRunning,
                this, &SequenceSelectionBloc::testIfAllSequenceThreadsAreRunning);
    }
}

void SequenceSelectionBloc::testIfAllSequenceThreadsAreRunning()
{
    if(_selectedSequenceInfo.file->areAllThreadsRunning())
    {
        QVector<SequenceThread*> sequenceThreadList =
                                                _selectedSequenceInfo.file->accessSequenceThreads();
        for(int index = 0; index < sequenceThreadList.count(); index++)
        {
            SequenceThread *sequenceThread = sequenceThreadList[index];

            connect(sequenceThread, &SequenceThread::nodesSequenceLoaded,
                    this,           &SequenceSelectionBloc::onNodesSequenceLoaded);

            sequenceThread->setSequenceData(
                                _selectedSequenceInfo.file->accessJsonArray(),
                                qobject_cast<SequenceLoadingErrorHandler*>(&accessErrorHandler()));
            sequenceThread->cleanAndLoadNodesSequence(
                                qobject_cast<SequenceLoadingErrorHandler*>(&accessErrorHandler()));
        }
    }
}

void SequenceSelectionBloc::onNodesSequenceLoaded(bool success)
{
    if(success)
    {
        getNavigationManager().goToSequenceRunPage();
    }
    else
    {
        _selectedSequenceInfo.file->stopAndClearThreads();
        QStringList errorMessages = accessErrorHandler().getMessages();

        getErrorCompanion().popUpBtnsFormat(false);
        getPopUpModel()->fireDisplayPopUp(getErrorCompanion().formatErrorMessages(errorMessages));
        setFormButtonsEnabled(true);
    }
}

void SequenceSelectionBloc::onCancelButtonClicked()
{
    setFormButtonsEnabled(false);
    getProjectManager().unloadFirstProject();
    goToProjectSelectionPageIfNeeded();
    setFormButtonsEnabled(true);
}

bool SequenceSelectionBloc::loadProject(QSharedPointer<Project> &project)
{
    RETURN_IF_FALSE(AFileSelectionBloc::loadProject(project));
    getProjectManager().unloadFirstProject();
    updateSequenceList();
    return true;
}

void SequenceSelectionBloc::goToProjectSelectionPageIfNeeded()
{
    if(getProjectManager().getProjects().isEmpty())
    {
        getNavigationManager().goToProjectSelectionPage(NavigationBehavior::PushOrPop);
    }
}

void SequenceSelectionBloc::registerQmlTypes()
{
    qmlRegisterType<SequenceSelectionBloc>("app.blocs", 1, 0, "SequenceSelectionBloc");
}
