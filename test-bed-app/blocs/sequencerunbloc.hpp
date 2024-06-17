// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "abasepagebloc.hpp"

#include "models/scrollablepopupmodel.hpp"
#include "testbedcore/types/sequencestatetype.hpp"

class ErrorHandlerCompanion;
class LogViewItemModel;
class SequenceLoadingErrorHandler;
class SequenceLogData;
class SequenceFile;

#include "testbedcore/types/stopaskedtype.hpp"


/** @brief Backend class linked with sequence run page */
class SequenceRunBloc : public ABasePageBloc
{
    Q_OBJECT
    Q_PROPERTY(GenericListModel<LogViewItemModel> *logViewModel
               READ getLogViewModel
               NOTIFY blocInitialized)
    Q_PROPERTY(QString projectName READ getProjectName NOTIFY blocInitialized)
    Q_PROPERTY(ScrollablePopUpModel *popUpModel READ getPopUpModel CONSTANT)
    Q_PROPERTY(SequenceStateType::Enum sequenceState
               READ getSequenceState
               NOTIFY sequenceStateChanged)
    Q_PROPERTY(int sequencesPerformed READ getSequencesPerformed NOTIFY sequencesPerformedChanged)

    public:
        /** @brief Class constructor
            @param parent An optional parent QObject */
        SequenceRunBloc(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~SequenceRunBloc() override;

    public:
        /** @brief Get the model linked to the pop up which can be displayed */
        ScrollablePopUpModel *getPopUpModel() const;

        /** @brief Called when play button is clicked */
        Q_INVOKABLE void onPlayButtonClicked();

        /** @brief Called when stop button is clicked */
        Q_INVOKABLE void onStopButtonClicked();

        /** @brief Return the log view model */
        GenericListModel<LogViewItemModel> *getLogViewModel() const {return _logViewModel;}

        /** @brief Get the project name */
        const QString &getProjectName() const { return _projectName; }

        /** @brief Get the number of sequences performed */
        int getSequencesPerformed() { return _sequencesPerformed; }

        /** @brief Get the sequence state */
        SequenceStateType::Enum getSequenceState() const { return _sequenceState; }

    protected:
        /** @see ABasePageBloc::fillFileMenu */
        virtual void fillFileMenu() override;

    protected slots:
        /** @see AbstractBloc::initBloc */
        virtual void initBloc() override;

    private slots:
        /** @brief Close the project and returns to the project selection page */
        void closeProject();

        /** @brief Called when a log is sent by a sequence thread
            @param log The sequence log data sent */
        void receiveLogs(const SequenceLogData &log);

        /** @brief Called when a NodesSequence is finished
            @param succes True if the NodesSequence has finished with no errors */
        void onSequenceFinished(bool success);

        /** @brief Called when a NodesSequence has been loaded (or if a problem occurs when loading)
            @param success True if no problem occurs when loading, False if not */
        void onNodesSequenceLoaded(bool success);

    signals:
        /** @brief emitted when the sequence state changed
            @param sequenceState The new sequence state */
        void sequenceStateChanged(SequenceStateType::Enum sequenceState);

        /** @brief Emitted to stop all the sequences
            @param stopAskedType Give a contact to the stop asking */
        void stopSequences(StopAskedType::Enum stopAskedType);

        /** @brief Emitted to pause all the sequences nodes
            @param setToPause True to pause all the sequences, False to resume if they were
                              paused */
        void manageSequencesPause(bool setToPause);

        /** @brief Emitted when sequence performed number changed
            @param sequencesPerformed Number of sequences performed */
        void sequencesPerformedChanged(int sequencesPerformed);

    private:
        /** @brief Initialize the loaded sequence */
        void initLoadedSequence();

        /** @brief Change the state of the sequence state
            @param sequenceState The new sequence state */
        void setSequenceState(SequenceStateType::Enum sequenceState);

        /** @brief Increment the number of sequence performed */
        void incrementSequencePerformed();

        /** @brief Set the sequences number performed */
        void setSequencesPerformed(int newSequencePerfomed);

    public:
        /** @brief Register QML type linked to this bloc */
        static void registerQmlTypes();

    private:
        ErrorHandlerCompanion *_errorHandlerCompanion{nullptr};
        QSharedPointer<SequenceFile> _loadedSequence;
        GenericListModel<LogViewItemModel> *_logViewModel;
        bool _infiniteLoop{true};
        QString _projectName;
        SequenceStateType::Enum _sequenceState { SequenceStateType::Unknown };
        int _sequencesPerformed{0};
        SequenceLoadingErrorHandler *_seqLoadingErrorHandler{nullptr};
};
