// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "afilebrowsermodule.hpp"

#include <QDir>

#include "abstractpluginelements/defaultbankglobalmanager.hpp"
#include "bankcoreshared/usebyplugin/icommoninterface.hpp"
#include "hmi-helper-lib/hmidisplayfilebrowserview.hpp"
#include "testbedcore/logs/sequencelogformatter.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/utility/pauseandstophelper.hpp"

const QRegExp AFileBrowserModule::ValidateFileExtensions = QRegExp(FileExtensionsRegExp);


AFileBrowserModule::AFileBrowserModule(const QString &moduleClassName,
                                       QObject *parent) :
    DefaultSequenceInstanceModule(moduleClassName,
                                  DefaultBankGlobalManager::Instance(),
                                  parent),
    _commonItf(DefaultBankGlobalManager::Instance()->accessCommonInterface())
{
}

bool AFileBrowserModule::initParamsFormat(JsonArray &paramsFormat) const
{
    JsonObject selectDir;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamBoolRequired(SelectDirParamName,
                                                               SelectDirParamDesc,
                                                               selectDir));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueBool(false, selectDir));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, selectDir));

    paramsFormat.append(selectDir);

    JsonObject fileExtensions;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringListRequired(FileExtensionsParamName,
                                                                     FileExtensionsParamDesc,
                                                                     fileExtensions));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, fileExtensions));

    paramsFormat.append(fileExtensions);

    JsonObject currentDirectory;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(FileDialogDirectoryParamName,
                                                                 FileDialogDirectoryParamDesc,
                                                                 currentDirectory));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, currentDirectory));

    paramsFormat.append(currentDirectory);

    return true;
}

bool AFileBrowserModule::initOutputsFormat(JsonArray &outputsFormat) const
{
    JsonObject filePath;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(OutputFilePathSelectedName,
                                                         filePath,
                                                         OutputFilePathSelectedDesc));

    outputsFormat.append(filePath);

    return true;
}

void AFileBrowserModule::processModule(const JsonArray &jsonObjectInputValues)
{
    if(!PauseAndStopHelper::managePauseAndStop(getSequenceState()))
    {
        emit moduleFinished(false, {});
        return;
    }

    if(!processDisplay(jsonObjectInputValues))
    {
        onProcessEnded(false);
    }
}

bool AFileBrowserModule::validateAndSetParameters(const JsonArray &parameters, JsonArray &errors)
{
    RETURN_IF_FALSE(DefaultSequenceInstanceModule::validateAndSetParameters(parameters, errors));

    QString error;
    if(!BankJsonParser::getBoolValue(parameters,
                                      SelectDirParamName,
                                     _selectDirectory,
                                     &error,
                                     true))
    {
        fillErrorArray(SelectDirParamName, error, errors);
        return false;
    }

    QString currentDir;
    if(!BankJsonParser::getStringValue(parameters,
                                        FileDialogDirectoryParamName,
                                       currentDir,
                                       &error,
                                       true))
    {
        fillErrorArray(FileDialogDirectoryParamName, error, errors);
        return false;
    }

    if(!currentDir.isEmpty())
    {
        _fileDialogDirectory = new QDir(currentDir);
        if(!_fileDialogDirectory->exists())
        {
            delete _fileDialogDirectory;
            _fileDialogDirectory = nullptr;
            error = QString("The current directory given: %1, doesn't exist").arg(currentDir);
            fillErrorArray(FileDialogDirectoryParamName, error, errors);
            return false;
        }
    }

    if(!BankJsonParser::getArrayStringValue(parameters,
                                             FileExtensionsParamName,
                                            _filesExtensions,
                                            &error,
                                            true))
    {
        fillErrorArray(FileExtensionsParamName, error, errors);
        return false;
    }

    if(!_filesExtensions.isEmpty())
    {
        QVector<QString>::const_iterator citer = _filesExtensions.cbegin();
        for(; citer != _filesExtensions.cend(); ++citer)
        {
            if(!ValidateFileExtensions.exactMatch(*citer))
            {
                error = QString("The file extension: %1, is not rightly formatted").arg(*citer);
                _filesExtensions.clear();
                fillErrorArray(FileExtensionsParamName, error, errors);
                return false;
            }
        }
    }

    return true;
}

void AFileBrowserModule::onProcessEnded(bool success)
{
    QString filePath;
    success = manageProcessEnd(success, filePath);

    if(!success)
    {
        emit newLog(SequenceLogFormatter::formatABusinessProblem(tr("(TR) A problem occured while "
                                                                    "trying to get the file "
                                                                    "path")));
        emit moduleFinished(false, {});
        return;
    }

    JsonArray output;
    BankJsonParser::setStringValue(OutputFilePathSelectedName, filePath, output);

    emit newLog(SequenceLogData(LogMsgType::Info,
                                SequenceLogCategory::BusinessScope,
                                QString("The file path has been got: %1").arg(filePath)));

    emit moduleFinished(true, output);
}

bool AFileBrowserModule::manageProcessEnd(bool success, QString &filePath)
{
    JsonArray result;
    if(_hmiDisplay != nullptr)
    {
        manageHmiConnection(*_hmiDisplay, false);
        result = _hmiDisplay->getResultReceived();
        _hmiDisplay->cancelDisplaying();
        _hmiDisplay->deleteLater();
        _hmiDisplay = nullptr;
    }

    if(!success)
    {
        // We test success only at the end in order to first remove the hmi display object
        return false;
    }

    RETURN_IF_FALSE(BankJsonParser::getStringValue(result,
                                                   _selectDirectory ? HmiDisplayDirName :
                                                       HmiDisplayFileName,
                                                   filePath));

    return saveChosenValue(filePath);
}

void AFileBrowserModule::onStopAsked()
{
    if(_hmiDisplay == nullptr)
    {
        // Nothing to do here, we may be out of the scope
        return;
    }

    onProcessEnded(false);
}

bool AFileBrowserModule::processDisplay(const JsonArray &jsonObjectInputValues)
{
    QString storeValue;
    RETURN_IF_FALSE(getDefaultValue(jsonObjectInputValues, storeValue));

    _hmiDisplay = new HmiDisplayFileBrowserView(_commonItf, this);

    manageHmiConnection(*_hmiDisplay, true);

    if(!_hmiDisplay->displayHmi(getSequenceToken(),
                                 _selectDirectory ? HmiDisplayDirName :
                                     HmiDisplayFileName,
                                 tr(_selectDirectory ? HmiDisplayDirDesc :
                                        HmiDisplayFileDesc),
                                   _filesExtensions,
                                   _selectDirectory,
                                   storeValue,
                                _fileDialogDirectory)){
        delete _hmiDisplay;
        _hmiDisplay = nullptr;
        return false;
    }

    return true;
}

void AFileBrowserModule::manageHmiConnection(HmiDisplayFileBrowserView &hmiDisplay, bool toConnect)
{
    if(toConnect)
    {
        connect(&hmiDisplay, &HmiDisplayFileBrowserView::finished,
                this,        &AFileBrowserModule::onProcessEnded);
        connect(&getSequenceState(), &SequenceStateHelper::stopAsked,
                this,                &AFileBrowserModule::onStopAsked);
    }
    else
    {
        disconnect(&hmiDisplay, &HmiDisplayFileBrowserView::finished,
                   this,        &AFileBrowserModule::onProcessEnded);
        disconnect(&getSequenceState(), &SequenceStateHelper::stopAsked,
                   this,                &AFileBrowserModule::onStopAsked);
    }
}
