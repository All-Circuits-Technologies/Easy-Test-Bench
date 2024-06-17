// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QString>
#include <QVariant>


/** @brief Contains the details of a known setting */
struct KnownSettingDetails
{
    QString name;                   /*!< @brief The default name of known setting */
    QString optionLetterName;       /*!< @brief The letter name used for command line option
                                         @note If not set (and so null), no option letter name is
                                               added */
    QString optionDescription;      /*!< @brief The description displayed when using command line
                                                option */
    QString optionValueName;        /*!< @brief The name of the option value in command line
                                                option */
    QVariant constantDefaultValue;  /*!< @brief The default constant value to use if no value
                                                hasn't been found anywhere */
};
