// SPDX-FileCopyrightText: 2023 Julien Lemasson <julien.lemasson@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QCanBusFrame>

#include "qtpeakcanlib/src/candevice/candeviceintf.hpp"

class CanDeviceIntf;
class CompareCanSequenceActionInfo;
class CompareCanSequenceInfo;
class SequenceLogData;


/** @brief Contains methods to process the sendind/receiving of CAN message(s).
           This is the interface to the PEAK Can lib for the process of CAN messages */
class CompareMsgProcess : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The parent instance class */
        explicit CompareMsgProcess(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~CompareMsgProcess() override;

    public:
        /** @brief Process the sequence action info given: @ref seqActionInfo, to act with the
                   PEAK CAN lib.
            @param pCanBusIntf The can bus interface to work with the lib
            @param seqActionInfo Contains all the needed info to act with the PEAK Can lib
            @param exitIfVerifFails If true, when a verification returns an error, the method will
                                    return false.
                                    If false, even if a verification returns an error, the method
                                    will return true
            @param receivedFrames All the frames received in the process
            @return True if no problem occurred */
        bool process(PCanBusItf::Enum pCanBusIntf,
                     const CompareCanSequenceInfo &seqActionInfo,
                     bool exitIfVerifFails,
                     QVector<QCanBusFrame> &receivedFrames);

    private:
        /** @brief Process the sequence action to send a message and wait for its answer
            @param deviceIntf The PEAK CAN lib device interface used to communicate on the CAN bus
            @param toSend The message to send
            @param seqActionInfo Contains all the needed info to act with the PEAK Can lib
            @param exitIfVerifFails If true, when a verification returns an error, the method will
                                    return false.
                                    If false, even if a verification returns an error, the method
                                    will return true
            @param timeoutInMs This is the timeout (in milliseconds) used when waiting for the cmd
                               answer
            @param receivedFrame This is filled with the received frame
            @return True if no problem occurred */
        bool processWriteAndWaitAnswer(const QSharedPointer<CanDeviceIntf> &deviceIntf,
                                       const QCanBusFrame &toSend,
                                       const CompareCanSequenceActionInfo& seqActionInfo,
                                       bool exitIfVerifFails,
                                       int timeoutInMs,
                                       QCanBusFrame &receivedFrame);

        /** @brief Process the sequence action and wait for an expected message
            @param deviceIntf The PEAK CAN lib device interface used to communicate on the CAN bus
            @param seqActionInfo Contains all the needed info to act with the PEAK Can lib
            @param exitIfVerifFails If true, when a verification returns an error, the method will
                                    return false.
                                    If false, even if a verification returns an error, the method
                                    will return true
            @param timeoutInMs This is the timeout (in milliseconds) used when waiting for the cmd
                               answer
            @param receivedFrame This is filled with the received frame
            @return True if no problem occurred */
        bool processWaitExpectedMsg(const QSharedPointer<CanDeviceIntf> &deviceIntf,
                                    const CompareCanSequenceActionInfo& seqActionInfo,
                                    bool exitIfVerifFails,
                                    int timeoutInMs,
                                    QCanBusFrame &receivedFrame);

        /** @brief Parse the received messages for PEAK Can lib and do the verifications, if needed
            @param expectedMsgs The messages received from the PEAN CAN lib
            @param seqActionInfo Contains all the needed info to act with the PEAK Can lib
            @param exitIfVerifFails If true, when a verification returns an error, the method will
                                    return false.
                                    If false, even if a verification returns an error, the method
                                    will return true
            @param receivedFrame This is filled with the received frame */
        bool parseReceivedMsg(const QVector<QCanBusFrame> &expectedMsgs,
                              const CompareCanSequenceActionInfo& seqActionInfo,
                              bool exitIfVerifFails,
                              QCanBusFrame &receivedFrame);

    signals:
        /** @brief Emitted when we need to send a new log to the test bed core lib
            @param log The log to send to the test bed lib */
        void newLog(const SequenceLogData &log);
};
