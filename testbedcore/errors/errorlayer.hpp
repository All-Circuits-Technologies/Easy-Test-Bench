// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>


/** @brief List of file loading error layers */
class ErrorLayer : public QObject
{
    Q_OBJECT

    public:
        enum Enum
        {
            // File errors
            ErrorType,            //First layer : type of error
            ErrorCause,           //Last layer : cause of error interruption

            // Sequence file errors
            FailedNodesSequence,  //Optional intermediate : nodesSequence failed
            FailedChildNode,      //Optional intermediate : child node failed
        };
};
