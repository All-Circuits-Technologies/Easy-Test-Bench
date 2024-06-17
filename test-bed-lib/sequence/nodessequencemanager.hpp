// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QJsonArray>
#include <QSharedPointer>

class NodesSequence;
class SequenceLoadingErrorHandler;

#include "testbedcore/types/stopaskedtype.hpp"


/** @brief Manage a NodeSequence inside a Sequencethread */
class NodesSequenceManager : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param errorHandler An optional sequence loading error handler pointer
            @param parent The parent class */
        explicit NodesSequenceManager(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~NodesSequenceManager();

        /** @brief Get the nodes sequence linked to the sequence file */
        const QSharedPointer<NodesSequence> &getNodesSequence() const { return _nodesSequence; }

        /** @brief Set the NodesSequence that will be used in the thread
            @param nodesSeq The nodesSequence to be set*/
        void setNodesSeq(const QSharedPointer<NodesSequence> &nodesSeq)
        { _nodesSequence = nodesSeq; }

    public slots :
        /** @brief Set the jsonArray used to initialize the nodesSequence
            @param jsonArray The jsonArray to set for loading the sequence
            @return True if no problem occurs */
        bool setSequenceData(const QJsonArray &jsonArray,
                             SequenceLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Pause the sequence in the nodesSequence
            @param setToPause True to pause the sequence, if it's running
                              False to resume the sequence, if it's in pause
            @return True if no problem occurs */
        bool manageSequencePause(bool setToPause);

        /** @brief Start the sequence in the nodesSequence
            @return True if no problem occurs */
        bool startSequence();

        /** @brief Stop the sequence in the nodesSequence
            @param stopAskedType Give a contact to the stop asking
            @return True if no problem occurs */
        bool stopSequence(StopAskedType::Enum stopAskedType);

        /** @brief Unload and clean NodesSequence */
        void cleanNodesSequence();

        /** @brief Recreate a NodesSequence from the jsonArray
            @return True if no problem occurs */
        bool cleanAndLoadNodesSequence(SequenceLoadingErrorHandler *errorHandler = nullptr);

    signals :
        /** @brief Send when the nodesSequence is loaded with succes or not
            @param success True if NodesSequence is loaded
            @return True if no problem occurs */
        void nodesSequenceLoaded(bool success);

    private :
        QSharedPointer<NodesSequence> _nodesSequence;
        QJsonArray _nodesSequenceInitializer;
};
