// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "numberutility/number.hpp"

class CanMsgSeqParams;
class PCanIntfSetupHandle;
class PCanSequenceActionInfo;


/** @brief This represents the PEAK CAN sequence information for the read and write process
    @note This may contain multiple frames to send and the expected answers linked to each one of
          them
    @note @ref _cmdsInfo contains a list of elements to send (with their expected result) */
class PCanSequenceInfo : public QObject
{
    Q_OBJECT

    private:
        /** @brief Class constructor
            @param testGroupName The test group to display in business logs before sending all the
                                 linked commands
            @param timeoutInMs The timeout in milliseconds to use when waiting for a CAN message, if
                               equals to -1, nothing is done
            @param seqActionsInfo The commands to send and/or the expected message to verify
            @param parent The parent instance class */
        explicit PCanSequenceInfo(const QString &testGroupName,
                                  int timeoutInMs,
                                  const QVector<const PCanSequenceActionInfo*> &seqActionsInfo,
                                  QObject *parent = nullptr);

    public:
        /** @brief The class destructor */
        virtual ~PCanSequenceInfo() override;

    public:
        /** @brief Get the list of the commands to send */
        const QVector<const PCanSequenceActionInfo*> &getSeqActionsInfo() const
        { return _seqActionsInfo; }

        /** @brief Get the test group name to display in business logs
            @note Nothing has to be displayed if the value returned is empty */
        const QString &getTestGroupName() const { return _testGroupName; }

        /** @brief Get the timeout in milliseconds when waiting for the CAN messages
            @note If -1 it means that we can wait forever */
        int getTimeoutInMs() const { return _timeoutInMs; }

    public:
        /** @brief Parse the PEAK CAN sequence information from the parameters given
            @param msgSeqParams The message sequence parameters to parse to create the class
            @param manageMultipleCmd If true, the parent modules manages multiple cmds
                                     If false, it means that the parent module only send one
                                     command, and so the command idx is not written in the
                                     @ref verifs values (we have to add it manually)
            @return The created @ref PCanWritingInfo thanks to the given parameters or nullptr if
                    a problem occurred. */
        static const PCanSequenceInfo *parsePCanSequenceInfo(const CanMsgSeqParams &msgSeqParams,
                                                             bool manageMultipleCmd = false);

    private:
        QVector<const PCanSequenceActionInfo*> _seqActionsInfo;
        QString _testGroupName{};
        int _timeoutInMs{-1};
};
