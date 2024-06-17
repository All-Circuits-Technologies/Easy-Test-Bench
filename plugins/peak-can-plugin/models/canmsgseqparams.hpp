// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QVector>

#include "numberutility/number.hpp"

#include "peakcanconstants.hpp"


/** @brief This model contains the parameters managed by the CAN message module sequence */
class CanMsgSeqParams : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The parent instance */
        explicit CanMsgSeqParams(QObject *parent = nullptr);

        /** @brief Copy constructor
            @param otherElement The other element to copy */
        explicit CanMsgSeqParams(const CanMsgSeqParams &otherElement);

        /** @brief Class destructor*/
        virtual ~CanMsgSeqParams() override;

    public:
        /** @brief Get the test group name */
        const QString &getTestGroupName() const { return _testGroupName; }

        /** @brief Set the test group name */
        void setTestGroupName(const QString &newTestGroupName)
        { _testGroupName = newTestGroupName; }

        /** @brief Get the wait CAN message timeout in milliseconds */
        int getTimeoutInMs() const { return _timeoutInMs; }

        /** @brief Set the wait CAN message timeout in milliseconds */
        void setTimeoutInMs(int newTimeoutInMs) { _timeoutInMs = newTimeoutInMs; }

        /** @brief Get the commands list */
        const QVector<QString> &getCmds() const { return _cmds; }

        /** @brief Set the commands list */
        void setCmds(const QVector<QString> &newCmds) { _cmds = newCmds; }

        /** @brief Say if the command to send has bitrate switch */
        bool hasBitrateSwitch() const { return _hasBitrateSwitch; }

        /** @brief Set the bitrate switch state for the command to send */
        void setBitrateSwitch(bool newBitrateSwitch)
        { _hasBitrateSwitch = newBitrateSwitch; }

        /** @brief Say if the message id has extended frame format */
        bool isExtendedFrameFormat() const { return _isExtendedFrameFormat; }

        /** @brief Set the extended frame format */
        void setExtendedFrameFormat(bool newExtendedFrameFormat)
        { _isExtendedFrameFormat = newExtendedFrameFormat; }

        /** @brief Get the ids list of expected messages */
        const QVector<Number> &getExpectedMsgsIds() const { return _expectedMsgsIds; }

        /** @brief Set the expected messages ids list */
        void setExpectedMsgsIds(const QVector<Number> &newExpectedMsgsIds)
        { _expectedMsgsIds = newExpectedMsgsIds; }

        /** @brief Get the verification to apply on the received messages */
        const QVector<QString> &getVerifs() const { return _verifs; }

        /** @brief Set the verification to apply on the received messages */
        void setVerifs(const QVector<QString> &newVerifs) { _verifs = newVerifs; }

    public:
        /** @brief Equals operator
            @param otherElement The element to get its values from
            @return A reference to this instance */
        CanMsgSeqParams &operator=(const CanMsgSeqParams &otherElement);

    private:
        QString _testGroupName;
        int _timeoutInMs{-1};
        QVector<QString> _cmds;
        bool _hasBitrateSwitch{
                    PeakCanConstants::ModulesEntries::Params::HasBitrateSwitchDefaultValue};
        bool _isExtendedFrameFormat{
                    PeakCanConstants::ModulesEntries::Params::IsExtendedFrameFormatDefaultValue};
        QVector<Number> _expectedMsgsIds;
        QVector<QString> _verifs;
};
