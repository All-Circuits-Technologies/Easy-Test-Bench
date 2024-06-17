// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "hmidisplaystringinputview.hpp"

#include <QRegularExpression>

#include "bankcoreshared/usebyplugin/icommoninterface.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"


HmiDisplayStringInputView::HmiDisplayStringInputView(ICommonInterface &commonItf, QObject *parent)
    : HmiDisplayHelper(commonItf, parent)
{
}

HmiDisplayStringInputView::~HmiDisplayStringInputView()
{
}

bool HmiDisplayStringInputView::displayHmi(const QString &name,
                                           const QString &description,
                                           const QString &imagePath,
                                           int minLength,
                                           int maxLength,
                                           const QString &patternRegex)
{
    JsonObject guiToDisplay;

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(name, description, guiToDisplay));

    RETURN_IF_FALSE(formatImage(imagePath, QString(), guiToDisplay));

    if(minLength != -1 || maxLength != -1)
    {
        RETURN_IF_FALSE(BankJsonFormatter::formatParamMinMaxString(
                            guiToDisplay,
                            minLength,
                            maxLength));
    }

    if(!patternRegex.isEmpty())
    {
        RETURN_IF_FALSE(BankJsonFormatter::formatParamPatternRegex(patternRegex, guiToDisplay));
    }

    if(_clearFieldIfError)
    {
        RETURN_IF_FALSE(BankJsonFormatter::formatParamClearInputIfError(_clearFieldIfError,
                                                                        guiToDisplay));
    }

    _minLength = minLength;
    _maxLength = maxLength;
    _patternRegex = patternRegex;

    return displayHmiImpl(name, guiToDisplay);
}

bool HmiDisplayStringInputView::parseDisplayResult(const JsonArray &valuesSet)
{
    QString inputText;
    RETURN_IF_FALSE(BankJsonParser::getStringValue(valuesSet, getName(), inputText));

    if(_minLength != -1 && inputText.length() < _minLength)
    {
        qWarning() << "The text received: " << inputText << " is inferior to the min length: "
                   << _minLength;
        return false;
    }

    if(_maxLength != -1 && inputText.length() > _maxLength)
    {
        qWarning() << "The text received: " << inputText << " is superior to the max length: "
                   << _maxLength;
        return false;
    }

    if(!_patternRegex.isEmpty())
    {
        QRegularExpression regExp(_patternRegex);

        if(!regExp.match(inputText).hasMatch())
        {
            qWarning() << "The text received: " << inputText << " doesn't match the regexp: "
                       << _patternRegex;
            return false;
        }
    }

    return true;
}
