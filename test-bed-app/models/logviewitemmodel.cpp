// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "logviewitemmodel.hpp"

#include <QQmlEngine>


LogViewItemModel::LogViewItemModel(QObject *parent) : QObject(parent)
{
}

void LogViewItemModel::setDescription(const QString &description)
{
    if(_description != description)
    {
        _description = description;
    }
}

QString LogViewItemModel::getCategoryStr() const
{
    return SequenceLogCategory::toString(_category);
}

void LogViewItemModel::setCategory(SequenceLogCategory::Enum category)
{
    if(_category != category)
    {
        _category = category;
    }
}

QString LogViewItemModel::getLevelStr() const
{
    return LogMsgType::toTrString(_level);
}

QString LogViewItemModel::getShortLevelStr() const
{
    return LogMsgType::toOneLetterTrString(_level);
}

void LogViewItemModel::setLevel(LogMsgType::Enum level)
{
    if(_level != level)
    {
        _level = level;
    }
}

QString LogViewItemModel::getDateTimeStr() const
{
    return _timeStamp.toString(StrDateTimeFormat);
}

QString LogViewItemModel::getTimeStr() const
{
    return _timeStamp.toString(StrTimeFormat);
}

void LogViewItemModel::setDateTime(const QDateTime &timeStamp)
{
    if(timeStamp != _timeStamp)
    {
        _timeStamp = timeStamp;
    }
}

void LogViewItemModel::setTimeStampMSecsSinceEpoch(qint64 timeStamp)
{
    setDateTime(QDateTime::fromMSecsSinceEpoch(timeStamp));
}

void LogViewItemModel::registerQmlType()
{
    qmlRegisterType<LogViewItemModel>("app.models", 1, 0, "LogViewItemModel");
}
