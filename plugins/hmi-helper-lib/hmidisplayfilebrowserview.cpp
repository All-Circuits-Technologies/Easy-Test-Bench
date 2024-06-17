// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "hmidisplayfilebrowserview.hpp"

#include "bankcoreshared/usebyplugin/icommoninterface.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"


HmiDisplayFileBrowserView::HmiDisplayFileBrowserView(ICommonInterface &commonItf,
                                                     QObject *parent) :
    HmiDisplayHelper(commonItf, parent)
{
}

HmiDisplayFileBrowserView::~HmiDisplayFileBrowserView()
{
}

bool HmiDisplayFileBrowserView::displayHmi(const QString &sequenceToken,
                                           const QString &name,
                                           const QString &description,
                                           const QVector<QString> &filesExtensions,
                                           bool selectDir,
                                           const QString &defaultPathValue,
                                           const QDir *currentDirectory)
{
    QString directory;

    if(currentDirectory != nullptr)
    {
        if(!currentDirectory->exists())
        {
            qWarning() << "The given directory doesn't exist: " << currentDirectory->absolutePath()
                       << ", rollback to use the sequence path";
        }
        else
        {
            directory = currentDirectory->absolutePath();
        }
    }

    if(directory.isEmpty())
    {
        QDir sequenceDir;
        if(!accessCommonItf().getSequenceDirectory(sequenceToken, sequenceDir))
        {
            qWarning() << "Can't get the sequence directory, to find the image";
            return false;
        }

        directory = sequenceDir.absolutePath();
    }

    JsonObject guiToDisplay;

    RETURN_IF_FALSE(BankJsonFormatter::formatParamSelectFileRequired(name,
                                                                     description,
                                                                     guiToDisplay,
                                                                     selectDir,
                                                                     filesExtensions,
                                                                     directory));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueString(defaultPathValue,
                                                                     guiToDisplay));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, guiToDisplay));

    _selectDir = selectDir;

    return displayHmiImpl(name, guiToDisplay);
}

bool HmiDisplayFileBrowserView::parseDisplayResult(const JsonArray &valuesSet)
{
    QString filePath;
    RETURN_IF_FALSE(BankJsonParser::getStringValue(valuesSet, getName(), filePath));

    QFileInfo fileInfo(filePath);
    if(!fileInfo.exists())
    {
        qWarning() << "The path chosen doesn't exist: " << filePath;
        return false;
    }

    if(_selectDir && !fileInfo.isDir())
    {
        qWarning() << "The element chosen by the user isn't a directory: " << filePath;
        return false;
    }

    if(!_selectDir && !fileInfo.isFile())
    {
        qWarning() << "The element chosen by the user isn't a file: " << filePath;
        return false;
    }

    return true;
}
