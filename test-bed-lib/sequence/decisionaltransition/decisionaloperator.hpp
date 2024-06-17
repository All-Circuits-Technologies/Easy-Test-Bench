// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QString>


/** @brief Decisional operator managed by transitions */
class DecisionalOperator
{
    public:
        enum Enum : qint32 {
            EqualTo = 0,
            NotEqualTo,
            GreaterThan,
            GreaterOrEqualTo,
            LesserThan,
            LesserOrEqualTo,
            And,
            Or,
            Unknown
        };

    public:
        /** @brief Parse operator from string
            @param strOperator The string to parse in order to get the decisional operator
            @return The operator parsed, or unknown if a problem occurred */
        static Enum parseFromString(const QString &strOperator)
        {
            Enum decisionalOp = Unknown;

            if(strOperator == "==")
            {
                decisionalOp = EqualTo;
            }
            else if(strOperator == "!=")
            {
                decisionalOp = NotEqualTo;
            }
            else if(strOperator == ">")
            {
                decisionalOp = GreaterThan;
            }
            else if(strOperator == "<")
            {
                decisionalOp = LesserThan;
            }
            else if(strOperator == ">=")
            {
                decisionalOp = GreaterOrEqualTo;
            }
            else if(strOperator == "<=")
            {
                decisionalOp = LesserOrEqualTo;
            }
            else if(strOperator == "&&" || strOperator.toLower() == "and")
            {
                decisionalOp = And;
            }
            else if(strOperator == "||" || strOperator.toLower() == "or")
            {
                decisionalOp = Or;
            }

            return decisionalOp;
        }

        /** @brief Stringify the value given
            @param value The enum to stringify
            @return The enum stringified */
        static QString toString(Enum value)
        {
            switch(value)
            {
                case EqualTo:
                    return "==";
                case NotEqualTo:
                    return "!=";
                case GreaterThan:
                    return ">";
                case LesserThan:
                    return "<";
                case GreaterOrEqualTo:
                    return ">=";
                case LesserOrEqualTo:
                    return "<=";
                case And:
                    return "&&";
                case Or:
                    return "||";
                case Unknown:
                    return "unknown";
            }

            return "unknown";
        }

        /** @brief Parse the enum from integer
            @param value The integer value to parsed
            @return The value parsed or unknown */
        static Enum parseFromInt(qint32 value)
        {
            switch (value)
            {
                case EqualTo:
                case NotEqualTo:
                case GreaterThan:
                case GreaterOrEqualTo:
                case LesserThan:
                case LesserOrEqualTo:
                case And:
                case Or:
                case Unknown:
                    return static_cast<Enum>(value);
            }

            return Unknown;
        }
};
