// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "radiolistbuttonsinputmodel.hpp"

#include <QQuickView>

#include "jsonutility/jsonhelper.hpp"
#include "testbedcore/parsers/jsonparsersconstants.hpp"

#include <QQmlEngine>


RadioListButtonsInputModel::RadioListButtonsInputModel(QObject *parent) :
    APluginInputModel(parent)
{
}

RadioListButtonsInputModel::RadioListButtonsInputModel(
        const QSharedPointer<DisplayUiToken> &uiToken,
        QObject *parent) :
    APluginInputModel(PluginWidgetType::RadioListButtons,
                      TypeManaged::String,
                      uiToken,
                      parent),
    _buttons(new GenericListModel<ButtonInputModel>(this))
{
    connect(_buttons, &GenericListModelData::countChanged,
            this,     &RadioListButtonsInputModel::buttonsListChanged);
}

RadioListButtonsInputModel::~RadioListButtonsInputModel()
{
}

bool RadioListButtonsInputModel::parseAndSetAttributes(const JsonObject &elemDescription,
                                                       bool update)
{
    APluginInputModel::parseAndSetAttributes(elemDescription, update);

    QString defaultKeyChecked = getDefaultValue().toString();
    RETURN_IF_FALSE(JsonHelper::getString(elemDescription,
                                          JsonParsersConstants::Hmi::DefaultCheckedAttribute,
                                          defaultKeyChecked,
                                          true));
    setDefaultValue(defaultKeyChecked);

    return manageButtonsParsing(elemDescription, defaultKeyChecked, update);
}

bool RadioListButtonsInputModel::manageButtonsParsing(const JsonObject &elemDescription,
                                                      const QString &defaultKey,
                                                      bool update)
{
    if(!elemDescription.contains(JsonParsersConstants::Hmi::ListAttribute))
    {
        if(!update)
        {
            qWarning() << "The list attribute is needed but missing in the json object to create a "
                       << "radioListButtons";
            return false;
        }

        // Nothing to do
        return true;
    }

    JsonArray buttonsList;
    RETURN_IF_FALSE(JsonHelper::getArray(elemDescription,
                                         JsonParsersConstants::Hmi::ListAttribute,
                                         buttonsList));

    QVector<ButtonInputModel *> newBtnsList;
    for(int idx = 0; idx < buttonsList.length(); ++idx)
    {
        JsonObject buttonObj;
        RETURN_IF_FALSE(JsonHelper::getObject(buttonsList, idx, buttonObj));

        QString buttonKey;
        RETURN_IF_FALSE(JsonHelper::getString(buttonObj,
                                              JsonParsersConstants::Hmi::KeyAttribute,
                                              buttonKey));

        // The button doesn't already exist, we create it
        ButtonInputModel *btnModel = new ButtonInputModel(QSharedPointer<DisplayUiToken>(), this);

        // Init the model
        RETURN_IF_FALSE(btnModel->initModel(buttonObj));

        if(defaultKey != buttonKey)
        {
            // We update the button default value
            btnModel->setDefaultChecked(true);
        }

        newBtnsList.append(btnModel);
    }

    _buttons->clearAll();
    for(ButtonInputModel *model : newBtnsList)
    {
        _buttons->append(model);
    }

    return true;
}

void RadioListButtonsInputModel::registerQmlType()
{
    qmlRegisterType<RadioListButtonsInputModel>("app.models", 1, 0, "RadioListButtonsInputModel");
}
