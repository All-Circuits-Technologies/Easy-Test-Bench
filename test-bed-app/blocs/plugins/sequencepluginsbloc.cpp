// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "sequencepluginsbloc.hpp"

#include <QQmlEngine>
#include <QQuickItem>
#include <QQmlContext>
#include <QTimer>

#include "core/hmimanager.hpp"
#include "interfaces/appinterfacemanager.hpp"
#include "models/plugins/aplugininputmodel.hpp"


SequencePluginsBloc::SequencePluginsBloc(QObject *parent) :
    AbstractBloc(parent),
    _displayAppInterface(HmiManager::instance().getDisplayAppInterface()),
    _pluginInputModels(new GenericListModel<APluginInputModel>(this)),
    _visibilityTimer(new QTimer(this))
{
    _visibilityTimer->setSingleShot(true);
    _visibilityTimer->setInterval(VisibilityTimeoutThresholdInMs);
    connect(_visibilityTimer, &QTimer::timeout,
            this,             &SequencePluginsBloc::onThresholdTimerRaise);
}

SequencePluginsBloc::~SequencePluginsBloc()
{
}

void SequencePluginsBloc::setSequenceState(SequenceStateType::Enum sequenceState)
{
    if(_sequenceState == sequenceState)
    {
        // Nothing to do
        return;
    }

    _sequenceState = sequenceState;
    emit sequenceStateChanged(sequenceState);

    if(sequenceState == SequenceStateType::InPause || sequenceState == SequenceStateType::Running)
    {
        QVector<APluginInputModel *> itemsList = _pluginInputModels->getItemList();
        for(auto citer = itemsList.cbegin(); citer != itemsList.cend(); ++citer)
        {
            (*citer)->onPauseOrResume(sequenceState == SequenceStateType::Running);
        }
    }
    else if(_visibilityTimer->isActive())
    {
        // We stop the timer, because we know that no new plugin will pop up
        _visibilityTimer->stop();
        onThresholdTimerRaise();
    }
}

void SequencePluginsBloc::initBloc()
{
    connect(&_displayAppInterface,   &AppInterfaceManager::displayPluginView,
            this,                    &SequencePluginsBloc::onDisplayPluginView);

    appendPluginModels(_displayAppInterface.getInputModels());

    emit blocInitialized();
}

void SequencePluginsBloc::onDisplayPluginView(APluginInputModel *inputModel)
{
    appendPluginModels({ inputModel });
}

void SequencePluginsBloc::onRemovePluginView()
{
    APluginInputModel *inputModel = qobject_cast<APluginInputModel*>(sender());

    if(inputModel == nullptr)
    {
        qWarning() << "Only a plugin input model has to be connected or has to call the slot: "
                   << "onRemovePluginView ; can't proceed";
        return;
    }

    int prevCount = _pluginInputModels->getItemListData().length();

    if(!_pluginInputModels->removeElement(inputModel))
    {
        // The input model is deleted later in the remove element
        qWarning() << "A problem occurred when tried to remove the plugin input model from the "
                   << "grid view";
    }

    manageModelsListChanged(prevCount, _pluginInputModels->getItemListData().length());
}

void SequencePluginsBloc::onThresholdTimerRaise()
{
    emit askVisibilityWithThreshold(false);
}

void SequencePluginsBloc::appendPluginModels(const QVector<APluginInputModel *> &inputModels)
{
    int prevCount = _pluginInputModels->getItemListData().length();

    for(APluginInputModel *inputModel : inputModels)
    {
        connect(inputModel, &APluginInputModel::closeItem,
                this,       &SequencePluginsBloc::onRemovePluginView);
        _pluginInputModels->append(inputModel);
    }

    manageModelsListChanged(prevCount, _pluginInputModels->getItemListData().length());
}

void SequencePluginsBloc::manageModelsListChanged(int previousNb, int newNb)
{
    emit inputModelsListChanged(previousNb, newNb);

    if(previousNb == 0 && newNb > 0)
    {
        _visibilityTimer->stop();
        emit askVisibilityWithThreshold(true);
    }
    else if(previousNb > 0 && newNb == 0)
    {
        if(_sequenceState == SequenceStateType::Stopped ||
           _sequenceState == SequenceStateType::Unknown)
        {
            // No need to wait, we know that no new plugin will pop
            onThresholdTimerRaise();
        }
        else
        {
            _visibilityTimer->start();
        }
    }
}

void SequencePluginsBloc::registerQmlTypes()
{
    qmlRegisterType<SequencePluginsBloc>("app.blocs", 1, 0, "SequencePluginsBloc");
}
