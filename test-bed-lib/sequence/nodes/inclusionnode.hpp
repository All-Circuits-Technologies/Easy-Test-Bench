// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "sequence/nodes/asequencecontainernode.hpp"

class NodesSequenceManager;
class SequenceFile;
class SequenceLoadingErrorHandler;


/** @brief The inclusion node is a link toward an another sequence file
    @note When the node is entered for the first time, the sequence is loaded */
class InclusionNode : public ASequenceContainerNode
{
    Q_OBJECT

    public:
        /** @brief Class constuctor
            @param sequenceParent The NodesSequence parent of this node
            @param errorHandler An optional sequence loading error handler pointer
            @param parent The class parent */
        explicit InclusionNode(const QSharedPointer<NodesSequence> &sequenceParent,
                               QObject *parent = nullptr);

        /** @brief Set the sequence file linked to the inclusion node
            @param filePath The file path to the sequence file
            @param errorHandler An optional sequence loading error handler pointer
            @return True If no problem occurs */
        bool setAndLoadSequenceFile(const QString &filePath,
                                    SequenceLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Get the sequence file name */
        const QString &getSequenceFileName() const { return _sequenceFileName; }

    public:
        /** @see ASequenceContainerNode::getChildNodesSequence
            @note If the child sequenceFile is nullptr, the method will return a null object */
        virtual QSharedPointer<NodesSequence> getChildNodesSequence() const override;

    protected:
        /** @see ASequenceContainerNode::preparingSequenceBeforeStart */
        virtual bool preparingSequenceBeforeStart(QString &error) override;

        /** @see ASequenceContainerNode::forgetAllAboutMe */
        virtual void forgetAllAboutMe() override;

    private:
        /** @brief Load the sequence file
            @param errorHandler An optional sequence loading error handler pointer
            @return True if no problem occurs */
        bool loadSequenceFile(SequenceLoadingErrorHandler *errorHandler = nullptr);

    private:
        SequenceFile *_sequenceFile{nullptr};
        NodesSequenceManager *_nodesSequenceManager{nullptr};
        QString _sequenceFileName;
};
