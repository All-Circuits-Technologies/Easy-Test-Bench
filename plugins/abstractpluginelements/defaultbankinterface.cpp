// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "defaultbankinterface.hpp"

#include <QDebug>

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"
#include "abstractpluginelements/registering/registertopologymoduleutility.hpp"
#include "bankcoreshared/usebylib/isequenceinstancemodule.hpp"
#include "bankcoreshared/usebylib/itopologyinstancemodule.hpp"
#include "collectionutility/qhashhelper.hpp"


DefaultBankInterface::DefaultBankInterface(ICommonInterface &commonInterface, QObject *parent) :
    IBankInterface(parent),
    _commonInterface(commonInterface)
{
}

bool DefaultBankInterface::getUniqueName(QString &name) const
{
    name = PLUGIN_NAME;
    return true;
}

bool DefaultBankInterface::getVersion(QString &version) const
{
    version = VERSION_STRING;
    return true;
}

bool DefaultBankInterface::getTopologyModulesName(QVector<QString> &modulesName) const
{
    return getModuleNames(ModuleType::Topology, modulesName);
}

bool DefaultBankInterface::getSequenceModulesName(QVector<QString> &modulesName) const
{
    return getModuleNames(ModuleType::Sequence, modulesName);
}

bool DefaultBankInterface::getModuleNames(ModuleType moduleType, QVector<QString> &modulesName) const
{
    modulesName = QHashHelper::getKeys(getRegisteredModules(moduleType));
    return true;
}

QObject *DefaultBankInterface::createNewInstance(const QString &moduleName, ModuleType moduleType)
{
    const QHash<QString, QMetaObject> &modules = getRegisteredModules(moduleType);

    if(!modules.contains(moduleName))
    {
        return nullptr;
    }

    return modules[moduleName].newInstance();
}

ITopologyInstanceModule *DefaultBankInterface::createTopologyInstanceModule(
                                                                        const QString &moduleName)
{
    QObject *element = createNewInstance(moduleName, ModuleType::Topology);

    return qobject_cast<ITopologyInstanceModule*>(element);
}

ISequenceInstanceModule *DefaultBankInterface::createSequenceInstanceModule(
                                                                    const QString &moduleName,
                                                                    const QString &sequenceToken)
{
    // We lower the module name to be independent in sequence files of the class letter case
    QObject *element = createNewInstance(moduleName.toLower(), ModuleType::Sequence);

    ISequenceInstanceModule *seqModule = qobject_cast<ISequenceInstanceModule*>(element);

    if(seqModule && !seqModule->setSequenceToken(sequenceToken))
    {
        delete seqModule;
        seqModule = nullptr;
    }

    return seqModule;
}

QHash<QString, QMetaObject> DefaultBankInterface::getRegisteredModules(ModuleType moduleType)
{
    switch (moduleType)
    {
        case ModuleType::Sequence:
            return RegisterSequenceModuleUtility::Instance().getRegisteredModules();

        case ModuleType::Topology:
            return RegisterTopologyModuleUtility::Instance().getRegisteredModules();
    }

    return {};
}
