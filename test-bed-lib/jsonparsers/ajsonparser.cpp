// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "ajsonparser.hpp"

#include "jsonutility/jsonhelper.hpp"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>


AJsonParser::AJsonParser(QObject *parent) :
    QObject(parent)
{
}

QString AJsonParser::getError() const
{
    /* This method should only be called when an error occurred (when parsing failed).
     * If no error has been logged, then we want to reply with a generic message instead of null.
     * This is done in clearError which actually set a generic error description.
     */
    return _error.isEmpty() ? tr("(TR) Unknown error") : _error;
}

bool AJsonParser::loadJsonFileAndGetObject(QIODevice &device,
                                           QJsonObject &jsonObject)
{
    QJsonDocument document;

    if(!JsonHelper::loadJsonDevice(device, document))
    {
        qWarning() << "A problem occurred when trying to parse to Json, the device given";
        return false;
    }

    return JsonHelper::getObject(document, jsonObject);
}

bool AJsonParser::loadJsonFileAndGetArray(QIODevice &device, QJsonArray &jsonArray)
{
    QJsonDocument document;

    if(!JsonHelper::loadJsonDevice(device, document))
    {
        qWarning() << "A problem occurred when trying to parse to Json, the device given";
        return false;
    }

    return JsonHelper::getArray(document, jsonArray);
}

void AJsonParser::clearError()
{
    /* We want it to be cleared, not to be loaded with a generic "unknown" default value
     * otherwise setErrorIfEmpty would not work
     */
    _error.clear();
}

void AJsonParser::setErrorIfEmpty(const QString &error)
{
    if(_error.isEmpty())
    {
        _error = error;
    }
}
