// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "sequencerunbloc.hpp"

#include <qqmlengine.h>
#include <QSharedPointer>

#include "blocs/companions/errorhandlercompanion.hpp"
#include "blocs/companions/menucompanion.hpp"
#include "errors/sequenceloadingerrorhandler.hpp"
#include "models/menuitemmodel.hpp"
#include "models/logviewitemmodel.hpp"
#include "services/navigation/navigationmanager.hpp"
#include "test-bed-lib/core/globalmanager.hpp"
#include "test-bed-lib/project/projectmanager.hpp"
#include "test-bed-lib/project/projectfile.hpp"
#include "test-bed-lib/sequence/sequencefile.hpp"
#include "testbedcore/logs/sequencelogdata.hpp"


SequenceRunBloc::SequenceRunBloc(QObject *parent) :
    ABasePageBloc(parent),
    _errorHandlerCompanion(new ErrorHandlerCompanion(this)),
    _logViewModel(new GenericListModel<LogViewItemModel>()),
    _seqLoadingErrorHandler(new SequenceLoadingErrorHandler())
{
}

SequenceRunBloc::~SequenceRunBloc()
{
    delete _seqLoadingErrorHandler;
}

ScrollablePopUpModel *SequenceRunBloc::getPopUpModel() const
{
    return _errorHandlerCompanion->getPopUpModel();
}

void SequenceRunBloc::onPlayButtonClicked()
{
    _seqLoadingErrorHandler->clearErrors();

    if(_sequenceState == SequenceStateType::Unknown || _sequenceState == SequenceStateType::Stopped)
    {
        QVector<SequenceThread*> threads = _loadedSequence->accessSequenceThreads();
        for(auto citer = threads.cbegin(); citer != threads.cend(); ++citer)
        {
            (*citer)->cleanAndLoadNodesSequence(_seqLoadingErrorHandler);
        }

        setSequenceState(SequenceStateType::Running);
    }
    else if(_sequenceState == SequenceStateType::InPause)
    {
        emit manageSequencesPause(false);
        setSequenceState(SequenceStateType::Running);
    }
    else if(_sequenceState == SequenceStateType::Running)
    {
        emit manageSequencesPause(true);
        setSequenceState(SequenceStateType::InPause);
    }
}

void SequenceRunBloc::onStopButtonClicked()
{
    emit stopSequences(StopAskedType::AskedByUser);
    setSequenceState(SequenceStateType::Stopped);
}

void SequenceRunBloc::setSequenceState(SequenceStateType::Enum sequenceState)
{
    if(sequenceState != _sequenceState)
    {
        _sequenceState = sequenceState;
        emit sequenceStateChanged(_sequenceState);
    }
}

void SequenceRunBloc::initBloc()
{
    initLoadedSequence();
    ABasePageBloc::initBloc();
}

void SequenceRunBloc::initLoadedSequence()
{
    ProjectManager &projectManager = GlobalManager::instance().accessProjectMngr();

    QVector<QSharedPointer<const ProjectFile>> projects = projectManager.getProjects();
    if(projects.isEmpty())
    {
        qWarning() << "Can't load sequence, there is no opened project";
        return;
    }

    QSharedPointer<const ProjectFile> projectFile = projects.first();
    _projectName = projectFile->getName();

    // Because this particular app only loads one particular sequence in the previous page
    // we search in all the arborescence the sequence loaded.
    const QMap<QString, QSharedPointer<SequenceFile>> &sequences = projectFile->getSequenceFiles();
    QMap<QString, QSharedPointer<SequenceFile>>::const_iterator citer = sequences.cbegin();

    for(; citer != sequences.cend(); ++citer)
    {
        if(citer.value()->areAllNodesSequenceLoaded())
        {
            _loadedSequence = citer.value();
            break;
        }
    }

    if(!_loadedSequence)
    {
        qWarning() << "Currently there is no loaded sequence, we can't proceed";
        return;
    }

    // Because _loadedSequence isn't null, there is at least one sequence thread linked to the
    // sequence file
    SequenceThread* sequenceThread = _loadedSequence->accessSequenceThreads().first();

    connect(this,           &SequenceRunBloc::stopSequences,
            sequenceThread, &SequenceThread::stopSequence);
    connect(this,           &SequenceRunBloc::manageSequencesPause,
            sequenceThread, &SequenceThread::manageSequencePause);

    connect(sequenceThread, &SequenceThread::newLog,
            this,           &SequenceRunBloc::receiveLogs);
    connect(sequenceThread, &SequenceThread::nodesSequenceLoaded,
            this,           &SequenceRunBloc::onNodesSequenceLoaded);
    connect(sequenceThread, &SequenceThread::nodeSeqFinished,
            this,           &SequenceRunBloc::onSequenceFinished);

    setSequenceState(SequenceStateType::Stopped);
}

void SequenceRunBloc::receiveLogs(const SequenceLogData &log)
{
    LogViewItemModel *logViewItemModel = new LogViewItemModel(this);
    logViewItemModel->setDescription(log.getText());
    logViewItemModel->setCategory(log.getCategory());
    logViewItemModel->setLevel(log.getType());
    logViewItemModel->setTimeStampMSecsSinceEpoch(log.getTimestampMSecsSinceEpoch());

    _logViewModel->append(logViewItemModel);
}

void SequenceRunBloc::onNodesSequenceLoaded(bool success)
{
    SequenceThread *sequenceThread = qobject_cast<SequenceThread*>(sender());

    if(!sequenceThread)
    {
        qWarning() << "The sender is not a SequenceThread";
        return;
    }

    if(!success)
    {
        _errorHandlerCompanion->getPopUpModel()->fireDisplayPopUp(
                                                _errorHandlerCompanion->formatErrorMessages(
                                                        _seqLoadingErrorHandler->getMessages()));
        _seqLoadingErrorHandler->clearErrors();
        return;
    }

    sequenceThread->startSequence();
}

void SequenceRunBloc::onSequenceFinished(bool success)
{
    SequenceThread *sequenceThread = qobject_cast<SequenceThread*>(sender());

    if(!sequenceThread)
    {
        qWarning() << "The sender is not a SequenceThread";
        return;
    }

    if(!success)
    {
        setSequenceState(SequenceStateType::Stopped);
        sequenceThread->cleanNodesSequence();
        return;
    }

    incrementSequencePerformed();

    if(_infiniteLoop)
    {
        sequenceThread->cleanAndLoadNodesSequence(_seqLoadingErrorHandler);
    }
}

void SequenceRunBloc::closeProject()
{
    ProjectManager &projectManager = GlobalManager::instance().accessProjectMngr();
    projectManager.unloadAllProjects();
    getNavigationManager().goToProjectSelectionPage(NavigationBehavior::Replace);
}

void SequenceRunBloc::fillFileMenu()
{
    MenuItemModel *closeItemModel = MenuCompanion::getMenuItem(MenuItemName::close, this);
    QObject::connect(closeItemModel, &MenuItemModel::clicked,
                     this, &SequenceRunBloc::closeProject);

    getFileMenuModel()->append(closeItemModel);
    getFileMenuModel()->append(MenuCompanion::getMenuItem(MenuItemName::exportLogs, this));

    ABasePageBloc::fillFileMenu();
}

void SequenceRunBloc::setSequencesPerformed(int newSequencePerfomed)
{
    if(newSequencePerfomed != _sequencesPerformed)
    {
        _sequencesPerformed = newSequencePerfomed;
        emit sequencesPerformedChanged(_sequencesPerformed);
    }
}

void SequenceRunBloc::incrementSequencePerformed()
{
    setSequencesPerformed(_sequencesPerformed + 1);
}

void SequenceRunBloc::registerQmlTypes()
{
    qmlRegisterType<SequenceRunBloc>("app.blocs", 1, 0, "SequenceRunBloc");
}
