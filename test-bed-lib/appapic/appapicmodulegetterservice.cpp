// SPDX-FileCopyrightText: 2024 Hugo Landre <hugo.landre@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "actlibs/qtutilities/jsonutility/jsonstringify.hpp"

#include "appapicmodulegetterservice.hpp"
#include "appapic/models/appapiccatalogmodel.hpp"
#include "bank/bankmanager.hpp"
#include "core/globalmanager.hpp"
#include "models/appapicpluginmodel.hpp"
#include "usebylib/isequenceinstancemodule.hpp"


AppApiCModuleGetterService::AppApiCModuleGetterService(QObject *parent):QObject(parent)
{
}

bool AppApiCModuleGetterService::getModule(const QString &bankName,
                                           const QString &moduleName,
                                           AppApiCModuleModel &module)
{
    BankManager const &bankManager = GlobalManager::instance().getBankMngr();

    ISequenceInstanceModule *instanceModule = bankManager.getBankInterface(bankName)
                                             ->createSequenceInstanceModule(moduleName, SequenceToken);

    JsonArray jsonParams, jsonInputs, jsonOutputs;
    QByteArray params, inputs, outputs;

    bool ok;
    ok = instanceModule->getModuleParametersFormat(jsonParams);
    if (!ok)
    {
        delete instanceModule;
        return false;
    }
    ok = instanceModule->getInputVariablesFormat(jsonInputs);
    if (!ok)
    {
        delete instanceModule;
        return false;
    }
    ok = instanceModule->getOutputVariablesFormat(jsonOutputs);
    if (!ok)
    {
        delete instanceModule;
        return false;
    }

    delete instanceModule;

    RETURN_IF_FALSE(JsonStringify::toCompactString(jsonParams, params));
    RETURN_IF_FALSE(JsonStringify::toCompactString(jsonInputs, inputs));
    RETURN_IF_FALSE(JsonStringify::toCompactString(jsonOutputs, outputs));

    module = AppApiCModuleModel(moduleName, params, inputs, outputs);
    return true;
}

AppApiCCatalogModel AppApiCModuleGetterService::getSequenceModules()
{
    BankManager const &bankManager = GlobalManager::instance().getBankMngr();

    QVector<AppApiCPluginModel> plugins;

    QVector<QString> bankNames = bankManager.getBankNames();
    for(auto bankName = bankNames.cbegin(); bankName != bankNames.cend(); ++bankName)
    {
        QVector<QString> moduleNames;
        bool ok = bankManager.getBankInterface(*bankName)->getSequenceModulesName(moduleNames);
        if (!ok)
        {
            qWarning() << "failed to get module names for plugin "
                       << bankName->toLocal8Bit().data();
            continue;
        }

        QVector<AppApiCModuleModel> modules;

        for(auto moduleName = moduleNames.cbegin(); moduleName != moduleNames.cend(); ++moduleName)
        {

            AppApiCModuleModel m;
            ok = getModule(*bankName, *moduleName, m);
            if (!ok)
            {
                qWarning() << "failed to get formats for module "
                           << moduleName->toLocal8Bit().data();
                continue;
            }
            modules.push_back(m);
        }
        plugins.push_back(AppApiCPluginModel(*bankName, modules));
    }

    return AppApiCCatalogModel(plugins);
}

AppApiCCatalogModel AppApiCModuleGetterService::reloadSequenceModules()
{
    GlobalManager::instance().accessBankMngr().reloadBanks();
    return getSequenceModules();
}
