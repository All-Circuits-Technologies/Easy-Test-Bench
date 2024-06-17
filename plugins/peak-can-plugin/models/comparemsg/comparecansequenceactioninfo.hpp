// SPDX-FileCopyrightText: 2023 Julien Lemasson <julien.lemasson@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "models/apcanseqactioninfo.hpp"

#include "models/comparemsg/compareexpectedmsg.hpp"

class CanDeviceConfig;
class ExpectedCanFrameMask;
class Number;
class QCanBusFrame;


/** @brief This represents the PEAK CAN sequence action information for the read and write process
    @note This may contain one frame to send and/or the expected message id
    @note @ref _expectedAnswers contains a list of elements to check on answer received
    @note The method @ref parsePCanSequenceActionInfo makes the class data consistent; therefore,
          if @ref _expectedMsgVerifs contains elements, the @ref _expectedMsgId has a valid id */
class CompareCanSequenceActionInfo : public APCanSeqActionInfo
{
    Q_OBJECT

    private:
        /** @brief Class constructor
            @param expectedMsgsVerif The elements to check on the received message
            @param parent The parent instance class */
        explicit CompareCanSequenceActionInfo(
                                const QVector<const CompareExpectedMsg *> &expectedMsgsVerif,
                                const ExpectedCanFrameMask *expectedCanFrameMask,
                                QObject *parent = nullptr);

    public:
        /** @brief Class destructor */
        virtual ~CompareCanSequenceActionInfo() override;

    public:
        /** @brief Get the expected message verif data */
        const QVector<const CompareExpectedMsg *> &getExpectedMsgData() const
        { return _expectedMsgVerifs; }

        /** @brief Get the expected CAN frame mask
            @warning Value can be nullptr */
        const ExpectedCanFrameMask *getExpectedCanFrameMask() const
        { return _expectedCanFrameMask; }

    public:
        /** @brief Parse the PEAK CAN sequence action information from the parameters given
            @note The object created and returned by this method will take ownership on the
                  @ref expectedMsgVerifs parameter.
                  HOWEVER if something fails in this method process, the class caller will keep
                  ownership on those elements.
            @param cmd The CAN command to parse, may be empty if we only want to wait for a
                       message
            @param hasBitrateSwitch Say if the command to send has the bitrate switch state
            @param isExtendedFrameFormat Say if the command to send has the extended frame format
            @param expectedMsgVerifs The verification to do on the expected message, may be empty if
                                     we don't wait to verify the expected message
            @param msgId The ID of the expected message, may be invalid if we just want to send a
                         CAN message without waiting for a message
            @return The created @ref PCanSequenceActionInfo thanks to the given parameters or
                    nullptr if a problem occurred. */
        static const CompareCanSequenceActionInfo *parsePCanSequenceActionInfo(
            const QString &cmd,
            bool hasBitrateSwitch,
            bool isExtendedFrameFormat,
            const QVector<const CompareExpectedMsg *> &expectedMsgVerifs,
            const Number &msgId);

    private:
        QVector<const CompareExpectedMsg *> _expectedMsgVerifs;
        const ExpectedCanFrameMask *_expectedCanFrameMask;
};
