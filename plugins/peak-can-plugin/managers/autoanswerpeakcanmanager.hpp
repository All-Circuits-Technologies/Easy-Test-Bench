// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "handlerutility/handlerclassmembersmixin.hpp"

#include "qtpeakcanlib/src/pcanapi/pcanbusitf.hpp"

class AutoAnsThreadHandler;
class CanMsgSeqParams;
class ISequenceInstanceModule;
class QCanBusFrame;


/** @brief This manager contains all the auto answer process managed by the plugin */
class AutoAnswerPeakCanManager : public QObject,
                                 HandlerClassMembersMixin<PCanBusItf::Enum, AutoAnsThreadHandler>
{
    Q_OBJECT

    public:
        /** @brief Class contructor
            @param parent The parent instance class */
        explicit AutoAnswerPeakCanManager(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~AutoAnswerPeakCanManager() override;

    public:
        /** @brief Create or get echo process
            @note If the handler is already created, the method doesn't use the given parameter and
                  returns the existing handler
            @param peakCanIntf The can interface used to communicate with can, this also the key of
                               of the auto answer process (there is only one process by interface)
            @param msgSeqParams The parameters of the can messages sequence
            @param caller The sequence instance module which creates the echo process
            @param manageStats If true, the class displays statistics at the end of the process
            @return The process created if eveything has gone right or nullptr if a problem
                    occurred */
        QSharedPointer<AutoAnsThreadHandler> createOrGetEchoProcess(
            PCanBusItf::Enum peakCanIntf,
            const CanMsgSeqParams &msgSeqParams,
            ISequenceInstanceModule& caller,
            bool manageStats = true);

        /** @brief Get the echo process
            @param peakCanIntf The can interface used to communicate with can, this also the key of
                               of the auto answer process (there is only one process by interface)
            @return The process found if eveything has gone right or nullptr if a problem
                    occurred or if the handler hasn't been found */
        QSharedPointer<AutoAnsThreadHandler> getEchoProcess(PCanBusItf::Enum peakCanIntf);
};
