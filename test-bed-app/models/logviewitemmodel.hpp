// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "logsutility/logmsgtype.hpp"
#include "testbedcore/logs/sequencelogcategory.hpp"

#include <QDateTime>


/** @brief Class that manages the content of a logview item */
class LogViewItemModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString description READ getDescription CONSTANT)
    Q_PROPERTY(QDateTime dateTime READ getDateTime CONSTANT)
    Q_PROPERTY(QString dateTimeStr READ getDateTimeStr CONSTANT)
    Q_PROPERTY(QString timeStr READ getTimeStr CONSTANT)
    Q_PROPERTY(SequenceLogCategory::Enum category READ getCategory CONSTANT)
    Q_PROPERTY(QString categoryStr READ getCategoryStr CONSTANT)
    Q_PROPERTY(LogMsgType::Enum level READ getLevel CONSTANT)
    Q_PROPERTY(QString levelStr READ getLevelStr CONSTANT)
    Q_PROPERTY(QString shortLevelStr READ getShortLevelStr CONSTANT)

    public:
        /** @brief Get the log description
            @param parent An optional parent QObject*/
        LogViewItemModel(QObject *parent = nullptr);

        /** @brief Get the log description */
        const QString &getDescription() const { return _description; }

        /** @brief Get the log description */
        void setDescription(const QString &description);

        /** @brief Get the log category */
        SequenceLogCategory::Enum getCategory() const { return _category; }

        /** @brief Get the string representation of the log category */
        QString getCategoryStr() const;

        /** @brief Set the log category */
        void setCategory(SequenceLogCategory::Enum category);

        /** @brief Get the log level */
        LogMsgType::Enum getLevel() const { return _level; }

        /** @brief Get the string representation of the level */
        QString getLevelStr() const;

        /** @brief Get the short string representation of the level */
        QString getShortLevelStr() const;

        /** @brief Set the log level */
        void setLevel(LogMsgType::Enum level);

        /** @brief Get the date time value */
        const QDateTime &getDateTime() const { return _timeStamp; }

        /** @brief Get the complete log date and time in string format */
        QString getDateTimeStr() const;

        /** @brief Get the time log in string format */
        QString getTimeStr() const;

        /** @brief Set the log date time */
        void setDateTime(const QDateTime &timeStamp);

        /** @brief Give the timestamp in milliseconds since epoch to set the log date time */
        void setTimeStampMSecsSinceEpoch(qint64 timeStamp);

    public:
        /** @brief Register QML type linked to this model */
        static void registerQmlType();

    private:
        static const constexpr char *StrTimeFormat =     { "HH:mm:ss.zzz" };
        static const constexpr char *StrDateTimeFormat = { "yyyy-MM-dd HH:mm:ss.zzz" };

    private:
        QString _description;
        SequenceLogCategory::Enum _category{SequenceLogCategory::ApplicationScope};
        LogMsgType::Enum _level{LogMsgType::Unknown};
        QDateTime _timeStamp;
};
