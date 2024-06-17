// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "serialdetectshareddata.hpp"


SerialDetectSharedData::SerialDetectSharedData(const JsonArray &jsonObjectInputValues,
                                               const QSharedPointer<SerialLinkIntf> &serialIntf,
                                               SerialLinkReceivedReplyType::Enum &receivedReply,
                                               QVector<QString> &capturedElements,
                                               QObject *parent) :
    DetectionSharedData{parent},
    _jsonObjectInputValues{jsonObjectInputValues},
    _serialIntf{serialIntf},
    _receivedReply{receivedReply},
    _capturedElements{capturedElements}
{
}

SerialDetectSharedData::~SerialDetectSharedData()
{
}
