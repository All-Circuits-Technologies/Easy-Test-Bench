// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "blocs/abstractbloc.hpp"

#include <QQuickView>

#include "models/genericlistmodel.hpp"
#include "models/plugins/aplugininputmodel.hpp"
#include "testbedcore/types/sequencestatetype.hpp"

class DisplayUiToken;
class AppInterfaceManager;


/** @brief Backend class linked with sequence plugins view */
class SequencePluginsBloc : public AbstractBloc
{
    Q_OBJECT

    Q_PROPERTY(GenericListModel<APluginInputModel> *inputGenModels
               READ getPluginInputGenModels
               NOTIFY inputModelsListChanged)
    Q_PROPERTY(QVector<APluginInputModel*> inputModels
               READ getPluginInputModels
               NOTIFY inputModelsListChanged)
    Q_PROPERTY(SequenceStateType::Enum sequenceState
               READ getSequenceState
               WRITE setSequenceState
               NOTIFY sequenceStateChanged)

    public:
        /** @brief Class constructor
            @param parent An optional parent QObject */
        SequencePluginsBloc(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~SequencePluginsBloc() override;

    public:
        /** @brief Get the displayable list of plugin input models */
        GenericListModel<APluginInputModel> *getPluginInputGenModels() const
        { return _pluginInputModels; }

        /** @brief Get the list of plugin input models */
        QVector<APluginInputModel*> getPluginInputModels() const
        { return _pluginInputModels->getItemList(); }

        /** @brief Get the sequence state */
        SequenceStateType::Enum getSequenceState() const { return _sequenceState; }

        /** @brief Update the sequence state */
        void setSequenceState(SequenceStateType::Enum sequenceState);

    public:
         /** @brief Register QML type linked to this bloc */
        static void registerQmlTypes();

    protected slots:
        /** @see AbstractBloc::initBloc */
        virtual void initBloc() override;

    private slots:
        /** @brief Called by the DisplayAppInterface to display a plugin input
            @param uiToken The promise UI token to return the result with
            @param elemToDisplay The details of the element to display */
        void onDisplayPluginView(APluginInputModel *inputModel);

        /** @brief Called by a plugin input model when it's removed from the plugin view
            @note We expected that the sender of this method is a @ref APluginInputModel */
        void onRemovePluginView();

        /** @brief Called when the visibility threshold timer raise to ask to hide the current
                   view */
        void onThresholdTimerRaise();

    signals:
        /** @brief Emitted when the input models list is updated
            @param previousNb The number of elements before the modification
            @param newNb The number of elements after the modification */
        void inputModelsListChanged(int previousNb, int newNb);

        /** @brief Emitted to ask the display (or not) of the his sequence plugins view
            @note To avoid to have flashing display, we add a threshold to the hide asking */
        void askVisibilityWithThreshold(bool needToBeVisible);

        /** @brief emitted when the sequence state changed
            @param sequenceState The new sequence state */
        void sequenceStateChanged(SequenceStateType::Enum sequenceState);

    private:
        /** @brief Append plugin models to input models
            @param inputModels The input models to append */
        void appendPluginModels(const QVector<APluginInputModel *> &inputModels);

        /** @brief Called to manage the emitting of the models list changed signals but also the
                   visibility signal
            @param previousNb The previous number of elements in the models list
            @param newNb The new number of elements in the models list */
        void manageModelsListChanged(int previousNb, int newNb);

    private:
        /** @brief This timeout is for the threshold hidding of the plugins view in milliseconds */
        static const constexpr int VisibilityTimeoutThresholdInMs = 2000;

    private:
        AppInterfaceManager &_displayAppInterface;
        SequenceStateType::Enum _sequenceState { SequenceStateType::Unknown };
        GenericListModel<APluginInputModel> *_pluginInputModels{nullptr};

        QTimer* _visibilityTimer{nullptr};
};

