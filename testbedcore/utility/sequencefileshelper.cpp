// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "sequencefileshelper.hpp"

#include <QDebug>
#include <QDir>
#include <QFileInfo>


SequenceFilesHelper::SequenceFilesHelper()
{
}

QString SequenceFilesHelper::getAbsFilePathFromSequ(const QDir &sequenceDir,
                                                    const QString &filePath)
{
    QFileInfo fileFromSeq(filePath);

    if(fileFromSeq.isRelative())
    {
        fileFromSeq.setFile(sequenceDir, filePath);
    }

    if(!fileFromSeq.exists())
    {
        qWarning() << "Cannot use in the bank module the unexistent file : "
                   << fileFromSeq.absoluteFilePath();
        return {};
    }

    return fileFromSeq.absoluteFilePath();
}
