// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "appinterfacemanager.hpp"

#include "bank/displayuitoken.hpp"
#include "jsonutility/jsonhelper.hpp"
#include "models/plugins/radiolistbuttonsinputmodel.hpp"
#include "models/plugins/selectfileinputmodel.hpp"
#include "models/plugins/stringinputmodel.hpp"
#include "testbedcore/parsers/jsonparsersconstants.hpp"
#include "types/pluginwidgettype.hpp"


AppInterfaceManager::AppInterfaceManager(QObject *parent) : ADisplayUserInterface(parent)
{
}

AppInterfaceManager::~AppInterfaceManager()
{
}

void AppInterfaceManager::displayWidgetImpl(const QSharedPointer<DisplayUiToken> &uiToken,
                                            const JsonObject &elemToDisplay)
{
    if(!createInputModel(uiToken, elemToDisplay))
    {
        uiToken->finish(false);
        return;
    }

    emit displayPluginView(_pluginsData.last());
}

void AppInterfaceManager::updateWidgetImpl(const QSharedPointer<DisplayUiToken> &uiToken,
                                           const JsonObject &elemToUpdate)
{
    if( uiToken.isNull() || uiToken->isFinished() || uiToken->isCanceled())
    {
        qWarning() << "Wa can't update a view which doesn't exist, finished or cancelled";
        return;
    }


    for(auto citer = _pluginsData.cbegin(); citer != _pluginsData.cend(); ++citer)
    {
        if((*citer)->getUiToken() == uiToken)
        {
            (*citer)->updateModel(elemToUpdate);
            break;
        }
    }
}

void AppInterfaceManager::onPluginInputModelRemoved()
{
    APluginInputModel *inputModel = qobject_cast<APluginInputModel*>(sender());

    if(inputModel == nullptr)
    {
        qWarning() << "Only a plugin input model has to be connected or has to call the slot: "
                   << "onPluginInputModelRemoved ; can't proceed";
        return;
    }

    _pluginsData.removeOne(inputModel);
}

bool AppInterfaceManager::createInputModel(const QSharedPointer<DisplayUiToken> &uiToken,
                                           const JsonObject &elemToDisplay)
{
    if(uiToken->isCanceled())
    {
        // If cancelled, we stop here
        return false;
    }

    QString objectType;
    RETURN_IF_FALSE(JsonHelper::getString(elemToDisplay,
                                          JsonParsersConstants::Generic::TypeName,
                                          objectType));

    PluginWidgetType::Enum widgetType = PluginWidgetType::parseFromString(objectType);

    APluginInputModel *inputModel = nullptr;
    switch(widgetType)
    {
        case PluginWidgetType::RadioListButtons:
            inputModel = new RadioListButtonsInputModel(uiToken, this);
            break;

        case PluginWidgetType::SelectFile:
            inputModel = new SelectFileInputModel(uiToken, this);
            break;

        case PluginWidgetType::String:
            inputModel = new StringInputModel(uiToken, this);
            break;

        case PluginWidgetType::OkButton:
        case PluginWidgetType::Unknown:
            break;
    }

    if(inputModel == nullptr)
    {
        qWarning() << "The type of the widget to display is not managed by the application: "
                   << objectType;
        return false;
    }

    if(!inputModel->initModel(elemToDisplay))
    {
        delete inputModel;
        return false;
    }

    _pluginsData.append(inputModel);

    connect(inputModel, &APluginInputModel::closeItem,
            this,       &AppInterfaceManager::onPluginInputModelRemoved);

    return true;
}
