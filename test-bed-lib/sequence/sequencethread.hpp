// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "threadutility/basethread.hpp"

#include "testbedcore/testbedglobal.hpp"

class NodesSequence;
class NodesSequenceManager;
class SequenceLoadingErrorHandler;
class SequenceLogManager;
class SequenceLogData;

#include "testbedcore/types/stopaskedtype.hpp"


/** @brief Represents a sequence thread running an instance of the NodesSequence  */
class TESTBEDLIB_DLLSPEC SequenceThread : public BaseThread
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param errorHandler An optional sequence loading error handler pointer
            @param parent The parent class */
        explicit SequenceThread(QObject * parent = nullptr);

        /** @brief Class destructor */
        virtual ~SequenceThread() override;

    public:
        /** @brief Get the NodeSequence managed by the thread */
        QSharedPointer<const NodesSequence> getLinkedNodesSequence() const;

    public slots:
        /** @brief Relay the jsonArray to the NodesSequenceManager
            @note The method is thread-safe
            @param jsonArray QJsonArray to initialize the nodesSequence
            @return True if no problem occurs */
        bool setSequenceData(const QJsonArray &jsonArray,
                             SequenceLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Start the sequence
            @note The method is thread-safe
            @return True if no problem occurs */
        bool startSequence();

        /** @brief Stop the sequence
            @note The method is thread-safe
            @param stopAskedType Give a contact to the stop asking
            @return True if no problem occurs */
        bool stopSequence(StopAskedType::Enum stopAskedType);

        /** @brief Manage the sequence pause
            @param setToPause If true the sequence is set to pause
                              If false, resume the sequence if it was in pause
            @note The method is thread-safe
            @return True if no problem occurs */
        bool manageSequencePause(bool setToPause);


        /** @brief Unload and clean NodesSequence
            @note The method is thread-safe */
        void cleanNodesSequence();

        /** @brief If a NodesSequence already exists, unload it and load a new one
            @note The method is thread-safe
            @return True if no problem occurs */
        bool cleanAndLoadNodesSequence(SequenceLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Ask to anload all the elements contains in the QThread and quit the thread */
        void prepareAndQuit();

    signals:
        /** @brief Emitted when the sequence has created a new log
            @param newLog The new sequence log */
        void newLog(const SequenceLogData &newLog);

        /** @brief Emitted when the nodesSequence is loaded
            @param success True if the NodesSequence is loaded */
        void nodesSequenceLoaded(bool success);

        /** @brief Emitted when the nodesSequence process is finished
            @param succes Return true if normal / false if error */
        void nodeSeqFinished(bool success);

    protected:
        /** @brief Start the thread */
        virtual void run() override;

    private slots:
        /** @brief Called when a NodesSequence has been loaded
            @param loadSuccess True if the sequence has been successfuly loaded */
        void onNodesSequenceLoaded(bool loadSuccess);

        /** @brief Called when the nodes sequences is finished
            @param succes True if normal / false if error */
        void onNodesSeqFinished(bool success);

    private:
        /** @brief Get the sequence log manager linked to the current nodes sequence
            @return The log manager of the sequence */
        const SequenceLogManager *getSeqLogManager() const;

    private:
        NodesSequenceManager* _nodesSeqManager {nullptr};
};
