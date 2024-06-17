// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "autoanswerserialmanager.hpp"

#include "bankcoreshared/usebylib/isequenceinstancemodule.hpp"

#include "autoanswer/processes/autoansserialhandler.hpp"
#include "autoanswer/processes/autoanswerseriallinkthread.hpp"


AutoAnswerSerialManager::AutoAnswerSerialManager(QObject *parent)
    : QObject{parent}
{
}

AutoAnswerSerialManager::~AutoAnswerSerialManager()
{
}

QSharedPointer<AutoAnsSerialHandler> AutoAnswerSerialManager::createOrGetEchoProcess(
    const QString &portName,
    ISequenceInstanceModule &caller,
    const AutoAnswerParams *params)
{
    auto createAutoAnswerProcess = [this, &params, &caller](const QString &key)
    {
        AutoAnsSerialHandler *autoAnswerHandler = new AutoAnsSerialHandler();
        AutoAnswerSerialLinkThread *autoAnswerProcess = autoAnswerHandler->accessThread();

        if(!autoAnswerProcess->initProcess(key, params))
        {
            qWarning() << "A problem occurred when tried to initialize the auto answer process: "
                       << key;
            delete autoAnswerHandler;

            return QSharedPointer<AutoAnsSerialHandler>();
        }

        this->connect(autoAnswerProcess, &AutoAnswerSerialLinkThread::newLog,
                      &caller,           &ISequenceInstanceModule::newLog);

        return QSharedPointer<AutoAnsSerialHandler>(autoAnswerHandler);
    };

    return createOrGetHandler(portName, createAutoAnswerProcess);
}

QSharedPointer<AutoAnsSerialHandler> AutoAnswerSerialManager::getEchoProcess(
    const QString &portName)
{
    return getHandler(portName);
}
