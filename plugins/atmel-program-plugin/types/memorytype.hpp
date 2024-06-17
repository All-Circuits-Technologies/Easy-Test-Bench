// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>


/** @brief Describes the type of memory which can be managed by the atprogram */
class MemoryType : public QObject
{
    Q_OBJECT

    public:
        enum Enum {
            Flash,
            Eeprom,
            Unknown
        };
        Q_ENUM(Enum)

    public:
        /** @brief Return the memory type stringified
            @note This can't be used with the program arguments
            @param memoryType The cmd to stringify
            @return The stringified memory type */
        static QString toString(Enum memoryType);

        /** @brief Parse the memory type from string
            @param value The memory type to parse
            @return The value parsed */
        static Enum parseFromString(const QString &value);

        /** @brief Return the memory type stringified
            @note The string can be used as it is in the program arguments
            @param memoryType The cmd to stringify
            @return An usable argument */
        static QString toProgramArgumentStr(Enum memoryType);

        /** @brief Get all the memory types enum values
            @param excludeUnknown If true, the unknown value isn't returned (and so you get all the
                                  memory type available)
            @return The list of memory types */
        static const QVector<Enum> &getAllEnums(bool excludeUnknown = false);

    private:
        /** @brief arguments to pass to the commands in the arguments section,
                     Format: atprogram [options] <command> <arguments>
            @note From microchip doc:
                    -fl --flash: Program flash address space. tinyAVR, megaAVR, and AVR XMEGA
                                 only */
        static const constexpr char *ShortFlashName = "-fl";

        /** @brief arguments to pass to the commands in the arguments section,
                     Format: atprogram [options] <command> <arguments>
            @note From microchip doc:
                    -ee --eeprom: Program EEPROM address space */
        static const constexpr char *ShortEepromName = "-ee";

    private:
        static QVector<Enum> EnumList;
        static QVector<Enum> EnumListWithoutUnknown;
};
