// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "handlerutility/handlerclassmembersmixin.hpp"

class AutoAnsSerialHandler;
class AutoAnswerParams;
class AutoAnswerSerialLinkThread;
class ISequenceInstanceModule;


/** @brief This manager contains the auto answering process linked to specific serial port */
class AutoAnswerSerialManager : public QObject,
                                HandlerClassMembersMixin<QString, AutoAnsSerialHandler>
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The parent instance */
        explicit AutoAnswerSerialManager(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~AutoAnswerSerialManager() override;

    public:
        /** @brief Create or get the echo process
            @note If the handler is already created, the method doesn't use the given parameter and
                  returns the existing handler
            @param portName The port name linked with the auto answering process
            @param caller The sequence instance module which creates the echo process
            @param params The module parameter to create the echo process
            @return The process created if eveything has gone right or nullptr if a problem
                    occurred */
        QSharedPointer<AutoAnsSerialHandler> createOrGetEchoProcess(
            const QString &portName,
            ISequenceInstanceModule &caller,
            const AutoAnswerParams *params);

        /** @brief Get the echo process
            @param portName The port name linked with the auto answering process
            @return The process found if eveything has gone right or nullptr if a problem
                    occurred or if the handler hasn't been found */
        QSharedPointer<AutoAnsSerialHandler> getEchoProcess(const QString &portName);
};
