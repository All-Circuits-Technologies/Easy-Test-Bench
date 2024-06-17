// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once


/** @brief This namespace contains useful methods to register the Meta and QML types of the app
           blocs C++ classes */
namespace ModelsRegistrator
{
    /** @brief Register meta type linked to this namespace */
    void registerMetaTypes();

    /** @brief Register QML type linked to this namespace */
    void registerQmlTypes();
};
