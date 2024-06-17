// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>


/** @brief List of the errors related to sequence nodes **/
class SequenceError : QObject
{
    Q_OBJECT

    public:
        enum Enum
        {
            FailedNodesSequence,
            FailedUnknownNodesSequence ,
            FailedChildNode,
            FailedUnknownChildNode
        };

    public:
        /** @brief Return a translation key associated to a sequence error
            @param sequenceError The Sequence error enum value
            @return A translation key **/
        static const char *getTrKey(SequenceError::Enum sequenceError);

    private :
        static const constexpr char *unknownErrorKey = QT_TR_NOOP("(TR) Unknown error type");
        static const constexpr char* failedNodesSequenceKey = QT_TR_NOOP("(TR) Can’t load the "
                                                                         "nodes sequence : '%1'");
        static const constexpr char* failedUnknownNodesSequenceKey = QT_TR_NOOP("(TR) Can’t load "
                                                                                "the nodes "
                                                                                "sequence");
        static const constexpr char* failedChildNodeKey = QT_TR_NOOP("(TR) A problem occurred when "
                                                                     "reading the '%1', "
                                                                     "named: '%2'");
        static const constexpr char* failedUnknownChildNodeKey = QT_TR_NOOP("(TR) A problem "
                                                                            "occurred when reading "
                                                                            "a '%1'");
};
