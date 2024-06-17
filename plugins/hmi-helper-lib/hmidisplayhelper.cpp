// SPDX-FileCopyrightText: 2020 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "hmidisplayhelper.hpp"

#include "bankcoreshared/usebyplugin/icommoninterface.hpp"
#include "jsonutility/jsonhelper.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/jsonparsersconstants.hpp"
#include "testbedcore/testbedglobal.hpp"
#include "testbedcore/utility/sequencefileshelper.hpp"


HmiDisplayHelper::HmiDisplayHelper(ICommonInterface &commonItf, QObject *parent) :
    QObject(parent),
    _commonItf(commonItf)
{
}

HmiDisplayHelper::~HmiDisplayHelper()
{
    cancelDisplaying();
}

bool HmiDisplayHelper::displayHmiImpl(const QString &name, const JsonObject &elemToDisplay)
{
    if(_displayStatus >= HmiDisplayStatus::Displaying)
    {
        qWarning() << "The HMI is already displayed or has been displayed, cannot proceed";
        return false;
    }

    if(elemToDisplay.isEmpty())
    {
        qWarning() << "No element to display, can't proceed";
        return false;
    }

    connect(&_commonItf, &ICommonInterface::guiResult, this, &HmiDisplayHelper::onGuiResult);

    _name = name;

    if(!_commonItf.displayParameterInput(elemToDisplay, _uuidToken))
    {
        setDisplayStatus(HmiDisplayStatus::Error);
        return false;
    }

    setDisplayStatus(HmiDisplayStatus::Displaying);
    return true;
}

bool HmiDisplayHelper::updateHmiImpl(const JsonObject &elemToUpdate)
{
    if(_displayStatus != HmiDisplayStatus::Displaying)
    {
        qWarning() << "The HMI is not displaying, cannot try to update HMI";
        return false;
    }

    if(elemToUpdate.isEmpty())
    {
        qWarning() << "No element to update, can't proceed";
        return false;
    }

    QString name;
    RETURN_IF_FALSE(JsonHelper::getString(elemToUpdate,
                                          JsonParsersConstants::Generic::NameAttribute,
                                          name,
                                          true));

    if(!name.isEmpty() && name != _name)
    {
        qWarning() << "We can't update the name of a displaying gui, current: " << _name
                   << ", wanted: " << name;
        return false;
    }

    return _commonItf.updateDisplay(_uuidToken, elemToUpdate);
}

bool HmiDisplayHelper::formatImage(const QString &imagePath,
                                   const QString &tokenToGetInSequenceDir,
                                   JsonObject &guiToDisplay)
{
    if(imagePath.isEmpty())
    {
        // Nothing to do
        return true;
    }

    QString imageFilePath;
    if(!tokenToGetInSequenceDir.isEmpty())
    {
        QDir seqDir;
        if(!accessCommonItf().getSequenceDirectory(tokenToGetInSequenceDir, seqDir))
        {
            qWarning() << "Can't get the directory path of the sequence: "
                       << tokenToGetInSequenceDir;
            return false;
        }

        imageFilePath = SequenceFilesHelper::getAbsFilePathFromSequ(seqDir, imagePath);

        if(imageFilePath.isEmpty())
        {
            return false;
        }
    }
    else
    {
        if(!QFileInfo::exists(imagePath))
        {
            qWarning() << "We haven't the information of sequence dir and the image given doesn't "
                       << "exist at the path: " << imagePath << ", can't process";
            return false;
        }

        imageFilePath = imagePath;
    }

    if(!BankJsonFormatter::formatParamImagePath(imageFilePath, guiToDisplay))
    {
        qWarning() << "Can't format the image file: " + imageFilePath.toLatin1()
                   << ", into the JsonObject to display";
        return false;
    }

    return true;
}

bool HmiDisplayHelper::cancelDisplaying()
{
    if(_displayStatus != HmiDisplayStatus::Displaying)
    {
        // Nothing to cancel, do nothing
        return true;
    }

    bool cancelSuccess = _commonItf.cancelDisplay(_uuidToken);

    setDisplayStatus(HmiDisplayStatus::Error);

    return cancelSuccess;
}

bool HmiDisplayHelper::updateHmiDescription(const QString &description)
{
    JsonObject guiToUpdate;

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDescription(description, guiToUpdate));

    return updateHmiImpl(guiToUpdate);
}

bool HmiDisplayHelper::updateHmiDescAndImage(const QString &description,
                                             const QString &imagePath,
                                             const QString &tokenToGetInSequenceDir)
{
    JsonObject guiToUpdate;

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDescription(description, guiToUpdate));

    RETURN_IF_FALSE(formatImage(imagePath, tokenToGetInSequenceDir, guiToUpdate));

    return updateHmiImpl(guiToUpdate);
}

bool HmiDisplayHelper::parseDisplayResult(const JsonArray &valuesSet)
{
    Q_UNUSED(valuesSet)
    return true;
}

void HmiDisplayHelper::onGuiResult(bool success,
                                   const QString &uuidToken,
                                   const JsonArray &valueSet)
{
    if(uuidToken != _uuidToken)
    {
        // Not concerned by this result
        return;
    }

    if(!success)
    {
        setDisplayStatus(HmiDisplayStatus::Error);
        return;
    }

    if(!parseDisplayResult(valueSet))
    {
        setDisplayStatus(HmiDisplayStatus::Error);
        return;
    }

    _resultReceived = valueSet;
    setDisplayStatus(HmiDisplayStatus::Success);
}

void HmiDisplayHelper::setDisplayStatus(HmiDisplayStatus::Enum newStatus)
{
    if(_displayStatus != newStatus)
    {
        if((_displayStatus == HmiDisplayStatus::Displaying) &&
           (newStatus      >  HmiDisplayStatus::Displaying))
        {
            disconnect(&_commonItf, &ICommonInterface::guiResult,
                       this,        &HmiDisplayHelper::onGuiResult);
        }

        _displayStatus = newStatus;

        if(_displayStatus > HmiDisplayStatus::Displaying)
        {
            // If we are here it means that the displaying is finished (because of a success or an
            // error)
            emit finished(_displayStatus == HmiDisplayStatus::Success);
        }
    }
}
