// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "stringinputmodel.hpp"

#include "definesutility/definesutility.hpp"
#include "jsonutility/jsonhelper.hpp"
#include "testbedcore/parsers/jsonparsersconstants.hpp"

#include <QQmlEngine>


StringInputModel::StringInputModel(QObject *parent) :
    APluginInputModel(parent)
{
}

StringInputModel::StringInputModel(const QSharedPointer<DisplayUiToken> &uiToken, QObject *parent):
    APluginInputModel(PluginWidgetType::String,
                      TypeManaged::String,
                      uiToken,
                      parent)
{
}

StringInputModel::~StringInputModel()
{
}

bool StringInputModel::parseAndSetAttributes(const JsonObject &elemDescription, bool update)
{
    Q_UNUSED(update)

    RETURN_IF_FALSE(APluginInputModel::parseAndSetAttributes(elemDescription, update));

    int minLength = _minLength;
    RETURN_IF_FALSE(JsonHelper::getInt32(elemDescription,
                                         JsonParsersConstants::Hmi::MinLengthAttribute,
                                         minLength,
                                         true));
    setMinLength(minLength);

    int maxLength = _maxLength;
    RETURN_IF_FALSE(JsonHelper::getInt32(elemDescription,
                                         JsonParsersConstants::Hmi::MaxLengthAttribute,
                                         maxLength,
                                         true));
    setMaxLength(maxLength);

    QString patternRegex = _regex.pattern();
    RETURN_IF_FALSE(JsonHelper::getString(elemDescription,
                                          JsonParsersConstants::Hmi::PatternRegexAttribute,
                                          patternRegex,
                                          true));
    setRegex(patternRegex);

    QString defaultValue = getDefaultValue().toString();
    RETURN_IF_FALSE(JsonHelper::getString(elemDescription,
                                          JsonParsersConstants::Hmi::DefaultValueAttribute,
                                          defaultValue,
                                          true));
    setDefaultValue(defaultValue);

    bool clearInputIfError = _clearInputIfError;
    RETURN_IF_FALSE(JsonHelper::getBoolean(elemDescription,
                                           JsonParsersConstants::Hmi::ClearFieldIfErrorAttribute,
                                           clearInputIfError,
                                           true));
    setClearInputIfError(clearInputIfError);

    return true;
}

void StringInputModel::setMinLength(int minLength)
{
    if(_minLength != minLength)
    {
        _minLength = minLength;
        emit minLengthChanged(_minLength);
    }
}

void StringInputModel::setMaxLength(int maxLength)
{
    if(_maxLength != maxLength)
    {
        _maxLength = maxLength;
        emit maxLengthChanged(_maxLength);
    }
}

void StringInputModel::setRegex(const QString &regex)
{
    if(regex.isEmpty())
    {
        setRegex(QRegularExpression());
    }
    else
    {
        setRegex(QRegularExpression(regex));
    }
}

void StringInputModel::setRegex(const QRegularExpression &regex)
{
    if(_regex != regex)
    {
        _regex = regex;
        emit regExpChanged(_regex);
    }
}

void StringInputModel::setClearInputIfError(bool clearInputIfError)
{
    if(_clearInputIfError != clearInputIfError)
    {
        _clearInputIfError = clearInputIfError;
        emit clearInputIfErrorChanged(_clearInputIfError);
    }
}

void StringInputModel::registerQmlType()
{
    qmlRegisterType<StringInputModel>("app.models", 1, 0, "StringInputModel");
}
