// SPDX-FileCopyrightText: 2024 Hugo Landre <hugo.landre@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "appapicqstringcopyhelper.hpp"

char *AppApiCQStringCopyHelper::copyQString(const QString &s)
{
    const int size = s.size() + 1;
    char *c = new char[size];
    strcpy_s(c, size, s.toStdString().c_str());
    return c;
}
