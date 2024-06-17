// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once


/** @brief The type of @ref ReferencesHolder */
namespace ReferencesHolderType
{
    enum Enum
    {
        CanHaveInternalRefs, //!< @brief Say that the ReferencesHolder can hold internal references
        OnlyExternalRefs     /*!< @brief Say that the ReferencesHolder can't hold internal
                                         references */
    };
}
