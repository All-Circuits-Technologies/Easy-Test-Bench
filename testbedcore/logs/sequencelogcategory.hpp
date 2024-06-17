// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>
#include <QVector>


/** @brief Defines categories for the sequence logs */
class SequenceLogCategory : public QObject
{
    Q_OBJECT

    public:
        enum Enum : quint8
        {
            BusinessScope       = 0x01,
            ApplicationScope    = 0x02,
            Channel1            = 0x04,  /**< @brief Extra log category which can be used in
                                                     sequences.
                                                     The meaning of Channel1 depends of the
                                                     sequence.
                                                     For instance: to manage the writing of a CSV
                                                     file by logging into this specific channel */
            Channel2            = 0x08  /**< @brief Extra log category which can be used in
                                                    sequences.
                                                    The meaning of Channel2 depends of the
                                                    sequence.
                                                    For instance: to manage the writing of a CSV
                                                    file by logging into this specific channel */
        };
        Q_ENUM(Enum)
        Q_DECLARE_FLAGS(Enums, Enum)

    public:
        /** @brief Register the meta types linked to the class */
        static void RegisterMetaTypes();

        /** @brief Cast the value to string
            @param value The value to stringify
            @return The value stringified */
        static QString toString(Enum value);

        /** @brief Parse the flag from string values
            @note If one of the enum string can't be parsed, the method will returns a void Flag
            @param values Strings representation of enum to transform in flag
            @return The flag parsed from vector */
        static Enums parseFromVector(const QVector<QString> &values);

        /** @brief Parse the enum from string
            @param value The string value to parse
            @param ok If not null, equals true if no problem occurs and if the parsing succeeds
            @param defaultValue Default value returned when the parsing failed
            @return The enum parsed from string or the default value if the parsing failed */
        static Enum parseFromString(const QString &value,
                                    bool *ok = nullptr,
                                    Enum defaultValue = ApplicationScope);

        /** @brief Get all the available flags */
        static Enums getAllFlags();

        /** @brief Get all the available flags
            @note Return the same flags as @ref SequenceLogCategory::getAllFlags method */
        static QVector<Enum> getAllEnums();

    private:
        static Enums AllElements;
        static QVector<Enum> EnumList;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(SequenceLogCategory::Enums)
Q_DECLARE_METATYPE(SequenceLogCategory::Enums)
