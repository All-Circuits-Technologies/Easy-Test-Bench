// SPDX-FileCopyrightText: 2019 - 2020 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "basicbankinterface.hpp"

#include <QDebug>

#include "testbedcore/testbedglobal.hpp"
#include "simplesequenceinstancemodule.hpp"

QSharedPointer<BasicBankInterface> BasicBankInterface::_instance =
                                                            QSharedPointer<BasicBankInterface>();


BasicBankInterface::BasicBankInterface(ICommonInterface &commonInterface, QObject *parent) :
    IBankInterface(parent),
    _commonInterface(commonInterface)
{
}

const QSharedPointer<BasicBankInterface> &BasicBankInterface::Create(
                                                                ICommonInterface &commonInterface)
{
    if(!_instance)
    {
        _instance = QSharedPointer<BasicBankInterface>(new BasicBankInterface(commonInterface));
    }
    else
    {
        qWarning() << "The object has already been created, return the current instance";
    }

    return _instance;
}

const QSharedPointer<BasicBankInterface> &BasicBankInterface::Instance()
{
    if(!_instance)
    {
        qWarning() << "The object hasn't been created, return null pointer";
    }

    return _instance;
}

bool BasicBankInterface::getUniqueName(QString &name) const
{
    name = PLUGIN_NAME;
    return true;
}

bool BasicBankInterface::getVersion(QString &version) const
{
    version = VERSION_STRING;
    return true;
}

bool BasicBankInterface::getTopologyModulesName(QVector<QString> &modulesName) const
{
    Q_UNUSED(modulesName)
    NOT_IMPLEMENTED();
    return true;
}

bool BasicBankInterface::getSequenceModulesName(QVector<QString> &modulesName) const
{
    modulesName << SimpleSequenceInstanceModule::moduleName;
    return true;
}

ITopologyInstanceModule *BasicBankInterface::createTopologyInstanceModule(
                                                                        const QString &moduleName)
{
    Q_UNUSED(moduleName)
    return nullptr;
}

ISequenceInstanceModule *BasicBankInterface::createSequenceInstanceModule(
                                                                    const QString &moduleName,
                                                                    const QString &sequenceToken)
{
    if(moduleName != SimpleSequenceInstanceModule::moduleName)
    {
        return nullptr;
    }

    ISequenceInstanceModule *sequence = new SimpleSequenceInstanceModule();

    if(!sequence->setSequenceToken(sequenceToken))
    {
        delete sequence;
    }

    return sequence;
}
