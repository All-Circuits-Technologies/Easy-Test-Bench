// SPDX-FileCopyrightText: 2020 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "logs/sequencelogcategory.hpp"
#include "logsutility/logmsgtype.hpp"

#include <QDateTime>
#include <QString>


/** @brief This class represents a sequence log object
    @note Depending of the nature of the log the LogMsgType can mean different things.
          * If the log category is SequenceLogCategory::ApplicationScope, and if:
                ** The log is about application debugging, the LogMsgType enums means what they are,
                        ex: LogMsgType::Debug means LogMsgType::Debug.
          * If the log category is SequenceLogCategory::BusinessScope, and if:
                ** The log is about a test:
                        LogMsgType::Debug       -> unused
                        LogMsgType::Info        -> test success
                        LogMsgType::Warning     -> unused
                        LogMsgType::Critical    -> test error
                        LogMsgType::Fatal       -> unused
                ** The log is about a business problem, only LogMsgType::Warning is used to
                   signify the problem */
class SequenceLogData
{
    public:
        /** @brief Default constructor, create an invalid log */
        SequenceLogData();

        /** @brief Construct a sequence log with the current datetime
            @note The log timestamps is set to the current datetime
            @param typeParam The log msg type
            @param categoryParam The log msg category
            @param textParam The sequence log text */
        SequenceLogData(LogMsgType::Enum typeParam,
                        SequenceLogCategory::Enum categoryParam,
                        const QString &textParam);

        /** @brief Construct a sequence log
            @param timestampMSecsSinceEpochParam The log timestamp to set
            @param typeParam The log msg type
            @param categoryParam The log msg category
            @param textParam The sequence log text*/
        SequenceLogData(qint64 timestampMSecsSinceEpochParam,
                        LogMsgType::Enum typeParam,
                        SequenceLogCategory::Enum categoryParam,
                        const QString &textParam);

    public:
        /** @brief Get the log timestamp, written in milliseconds since epoch
            @note If the value is negative, the log is considered as invalid */
        qint64 getTimestampMSecsSinceEpoch() const { return _timestampMSecsSinceEpoch; }

        /** @brief Set the log timestamp, written in milliseconds since epoch
            @note A negative value means that the log is invalid */
        void setTimestampMSecsSinceEpoch(qint64 timestamp)
        { _timestampMSecsSinceEpoch = timestamp; }

        /** @brief Get log type */
        LogMsgType::Enum getType() const { return _type; }

        /** @brief Set the log type */
        void setType(LogMsgType::Enum logType) { _type = logType; }

        /** @brief Get log category */
        SequenceLogCategory::Enum getCategory() const { return _category; }

        /** @brief Set the log category */
        void setCategory(SequenceLogCategory::Enum category) { _category = category; }

        /** @brief Get the log text */
        const QString &getText() const { return _text; }

        /** @brief Set the log text */
        void setText(const QString &text) { _text = text; }

        /** @brief Set the log invalid */
        void setInvalid() { _timestampMSecsSinceEpoch = -1; }

        /** @brief Test if the log is valid
            @note An invalid log is a log with a negative timestamp
            @return True if the log is valid */
        bool isValid() const;

    public:
        /** @brief Register class meta types */
        static void RegisterMetaTypes();

    private:
        qint64 _timestampMSecsSinceEpoch;
        LogMsgType::Enum _type;
        SequenceLogCategory::Enum _category;
        QString _text;
};

Q_DECLARE_METATYPE(SequenceLogData)
