// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once


/** @brief This namespace contains useful methods to register the Meta and QML types of HMI types
           C++ classes */
namespace TypesRegistrator
{
    /** @brief Register meta type linked to this namespace */
    void registerMetaTypes();

    /** @brief Register QML type linked to this namespace */
    void registerQmlTypes();
};
