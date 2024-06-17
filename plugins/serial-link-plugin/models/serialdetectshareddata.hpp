// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "detection-helper-lib/models/detectionshareddata.hpp"

#include "serial-link-lib/types/seriallinkreceivedreplytype.hpp"
#include "qtseriallinklib/seriallinkintf.hpp"

#include "jsonutility/jsontypedefs.hpp"


/** @brief This contains the shared data for the serial detection module */
class SerialDetectSharedData : public DetectionSharedData
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param jsonObjectInputValues The module input values
            @param serialIntf This is the interface of the serial link to communicate with.
                              This has already been tested and the link is valid to use.
            @param receivedReply The method implementation has to return the received reply state
            @param capturedElements Those are the captured elements by the expected reply regex
                                    Attention: the first element is the result of whole match;
                                    therefore you have to add one to the captured element index you
                                    expect.
            @param parent The parent instance */
        explicit SerialDetectSharedData(const JsonArray &jsonObjectInputValues,
                                        const QSharedPointer<SerialLinkIntf> &serialIntf,
                                        SerialLinkReceivedReplyType::Enum &receivedReply,
                                        QVector<QString> &capturedElements,
                                        QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~SerialDetectSharedData() override;

    public:
        /** @brief Get the module input values */
        const JsonArray &getJsonObjectInputValues() const { return _jsonObjectInputValues; }

        /** @brief Get the interface of the serial link to communicate with */
        const QSharedPointer<SerialLinkIntf> &getSerialIntf() const { return _serialIntf; }

        /** @brief Get the received reply state */
        SerialLinkReceivedReplyType::Enum &getReceivedReply() const { return _receivedReply; }

        /** @brief Get the captured elements got by the expected reply regex */
        QVector<QString> &getCapturedElements() const { return _capturedElements; }

    private:
        const JsonArray &_jsonObjectInputValues;
        const QSharedPointer<SerialLinkIntf> &_serialIntf;
        SerialLinkReceivedReplyType::Enum &_receivedReply;
        QVector<QString> &_capturedElements;
};
