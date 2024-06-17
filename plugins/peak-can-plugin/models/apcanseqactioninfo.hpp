// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

class CanDeviceConfig;
class Number;
class QCanBusFrame;


/** @brief This is the abstract class of the CAN message to send */
class APCanSeqActionInfo : public QObject
{
    Q_OBJECT

    protected:
        /** @brief Class constructor
            @param parent The class parent instance */
        explicit APCanSeqActionInfo(QObject *parent = nullptr);

    public:
        /** @brief Class destructor */
        virtual ~APCanSeqActionInfo() override;

    public:
        /** @brief Get the id of the expected message */
        quint32 getExpectedMsgId() const { return _expectedMsgId; }

        /** @brief Returns the frame to send, filled with the channel config
            @param config The PEAK lib Can device config
            @param frame The frame to filled with the info to send */
        void getCmdBus(const CanDeviceConfig &config, QCanBusFrame &frame) const;

        /** @brief Test if the class has a commande to send */
        bool hasCmdToSend() const;

        /** @brief Test if the class has expected answers */
        bool hasExpectedMessage() const;

    protected:
        /** @brief Fill the class parameters from the information given
            @param cmd The stringified CAN command to parse
            @param hasBitrateSwitch The bitrate switch to apply to the command
            @param isExtendedFrameFormat The extended frame format to apply to the command
            @param msgId The message id to apply to the command
            @return True if no problem occurred */
        bool fillFromParams(const QString &cmd,
                            bool hasBitrateSwitch,
                            bool isExtendedFrameFormat,
                            const Number &msgId);

    private:
        const QCanBusFrame *_cmdBus{nullptr};
        quint32 _expectedMsgId{std::numeric_limits<quint32>::max()};
};
