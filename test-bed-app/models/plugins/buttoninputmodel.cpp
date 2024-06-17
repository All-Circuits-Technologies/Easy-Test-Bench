// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "buttoninputmodel.hpp"

#include "jsonutility/jsonhelper.hpp"
#include "testbedcore/parsers/jsonparsersconstants.hpp"

#include <QQmlEngine>


ButtonInputModel::ButtonInputModel(QObject *parent) :
    APluginInputModel(parent)
{
}

ButtonInputModel::ButtonInputModel(const QSharedPointer<DisplayUiToken> &uiToken,
                                   QObject *parent) :
    APluginInputModel(PluginWidgetType::OkButton,
                      TypeManaged::String,
                      uiToken,
                      parent)
{
}

ButtonInputModel::~ButtonInputModel()
{
}

void ButtonInputModel::setDefaultChecked(bool checked)
{
    setDefaultValue(checked);
}

bool ButtonInputModel::parseAndSetAttributes(const JsonObject &elemDescription, bool update)
{
    APluginInputModel::parseAndSetAttributes(elemDescription, update);

    QString key = _key;
    RETURN_IF_FALSE(JsonHelper::getString(elemDescription,
                                          JsonParsersConstants::Hmi::KeyAttribute,
                                          key,
                                          update));
    setKey(key);

    QString label = _label;
    RETURN_IF_FALSE(JsonHelper::getString(elemDescription,
                                          JsonParsersConstants::Hmi::LabelAttribute,
                                          label,
                                          update));
    setLabel(label);

    QString keyShorcut(_keyShortcut);
    RETURN_IF_FALSE(JsonHelper::getString(elemDescription,
                                          JsonParsersConstants::Hmi::KeyShortcutAttribute,
                                          keyShorcut,
                                          true));
    setKeyShortcut(keyShorcut);

    RETURN_IF_FALSE(manageColorParseAndSet(elemDescription,
                                           JsonParsersConstants::Hmi::FontColorAttribute,
                                           _fontColor,
                                           &ButtonInputModel::setFontColor));

    RETURN_IF_FALSE(manageColorParseAndSet(elemDescription,
                                           JsonParsersConstants::Hmi::BackgroundColorAttribute,
                                           _backgroundColor,
                                           &ButtonInputModel::setBackgroundColor));

    bool defaultChecked = getDefaultValue().toBool();
    RETURN_IF_FALSE(JsonHelper::getBoolean(elemDescription,
                                           JsonParsersConstants::Hmi::DefaultCheckedAttribute,
                                           defaultChecked,
                                           true));
    setDefaultValue(defaultChecked);

    return true;
}

void ButtonInputModel::setKey(const QString &key)
{
    if(key != _key)
    {
        _key = key;
        emit keyChanged(_key);
    }
}

void ButtonInputModel::setLabel(const QString &label)
{
    if(label != _label)
    {
        _label = label;
        emit labelChanged(_label);
    }
}

void ButtonInputModel::setKeyShortcut(const QString &keyShortcut)
{
    QString tmpShortcut;

    if(keyShortcut.length() > 1)
    {
        qWarning() << "The shortcut given is not a single char: " << keyShortcut
                   << ", to make it works we get the first letter";
        tmpShortcut = keyShortcut.at(0);
    }
    else
    {
        tmpShortcut = keyShortcut;
    }

    if(tmpShortcut != _keyShortcut)
    {
        _keyShortcut = tmpShortcut;
        emit keyShortcutChanged(tmpShortcut);
    }
}

void ButtonInputModel::setFontColor(const QColor &fontColor)
{
    if(_fontColor != fontColor)
    {
        _fontColor = fontColor;
        emit fontColorChanged(fontColor);
    }
}

void ButtonInputModel::setBackgroundColor(const QColor &backgroundColor)
{
    if(_backgroundColor != backgroundColor)
    {
        _backgroundColor = backgroundColor;
        emit backgroundColorChanged(_backgroundColor);
    }
}

bool ButtonInputModel::manageColorParseAndSetImpl(const JsonObject &elemDescription,
                                                  const QString &attributeName,
                                                  const QColor &currentColor,
                                                  const std::function<void (const QColor &)> &setter)
{
    if(!elemDescription.contains(attributeName))
    {
        // The attribute hasn't been found, do nothing
        return true;
    }

    QString tmpColorStr = currentColor.name();
    RETURN_IF_FALSE(JsonHelper::getString(elemDescription,
                                          attributeName,
                                          tmpColorStr));

    QColor tmpColor;
    if(tmpColorStr.isEmpty())
    {
        // Reset color, do nothing
    }
    else if(!QColor::isValidColor(tmpColorStr))
    {
        qWarning() << "The " << attributeName << " color string given: " << tmpColorStr
                   << ", can't be parsed to a QColor";
        return false;
    }
    else
    {
        tmpColor = QColor(tmpColorStr);
    }

    setter(tmpColor);
    return true;
}

void ButtonInputModel::registerQmlType()
{
    qmlRegisterType<ButtonInputModel>("app.models", 1, 0, "ButtonInputModel");
}
