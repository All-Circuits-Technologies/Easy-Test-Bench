// SPDX-FileCopyrightText: 2020 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "sequencelogdata.hpp"


SequenceLogData::SequenceLogData() :
    _timestampMSecsSinceEpoch(-1),
    _type(LogMsgType::Debug),
    _category(SequenceLogCategory::ApplicationScope)
{
}

SequenceLogData::SequenceLogData(LogMsgType::Enum typeParam,
                                 SequenceLogCategory::Enum categoryParam,
                                 const QString &textParam) :
    _timestampMSecsSinceEpoch(QDateTime::currentMSecsSinceEpoch()),
    _type(typeParam),
    _category(categoryParam),
    _text(textParam)
{
}

SequenceLogData::SequenceLogData(qint64 timestampMSecsSinceEpochParam,
                                 LogMsgType::Enum typeParam,
                                 SequenceLogCategory::Enum categoryParam,
                                 const QString &textParam) :
    _timestampMSecsSinceEpoch(timestampMSecsSinceEpochParam),
    _type(typeParam),
    _category(categoryParam),
    _text(textParam)
{
}

bool SequenceLogData::isValid() const
{
    return _timestampMSecsSinceEpoch >= 0;
}

void SequenceLogData::RegisterMetaTypes()
{
    qRegisterMetaType<SequenceLogData>();
}
