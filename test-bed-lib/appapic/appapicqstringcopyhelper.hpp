// SPDX-FileCopyrightText: 2024 Hugo Landre <hugo.landre@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include <QString>

/** @brief Helper class to copy QStrings */
class AppApiCQStringCopyHelper
{
    public:
        /** @brief helper function to copy QStrings to char *
         *  @note caller should free the returned pointer */
        static char* copyQString(const QString &s);

};
