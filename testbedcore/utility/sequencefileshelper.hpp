// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

class QDir;


/** @brief This contains helpful methods to manage sequence files */
class SequenceFilesHelper
{
    private:
        /** @brief Private class constructor */
        explicit SequenceFilesHelper();

    public:
        /** @brief Get the absolute file path from sequence
            @note If the path is relative, we considere it's relative from the sequence folder
            @param sequenceDir The sequence directory
            @param filePath The file path to transform to absolute
            @return The path transformed into absolute, empty if a problem occurred */
        static QString getAbsFilePathFromSequ(const QDir &sequenceDir, const QString &filePath);
};
