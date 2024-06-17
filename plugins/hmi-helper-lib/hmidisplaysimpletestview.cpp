// SPDX-FileCopyrightText: 2020 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "hmidisplaysimpletestview.hpp"

#include <QDir>

#include "bankcoreshared/usebyplugin/icommoninterface.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/testbedglobal.hpp"


HmiDisplaySimpleTestView::HmiDisplaySimpleTestView(ICommonInterface &commonItf, QObject *parent) :
    HmiDisplayHelper(commonItf, parent)
{
}

bool HmiDisplaySimpleTestView::displayHmi(const QString &name,
                                          const QString &description,
                                          quint8 buttons,
                                          const QString &imagePath,
                                          const QString &tokenToGetInSequenceDir,
                                          ButtonEnum defaultButton)
{
    _buttonClicked = NoButton;
    JsonObject guiToDisplay;

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRadioListButtonsRequired(name,
                                                                           description,
                                                                           guiToDisplay));

    RETURN_IF_FALSE(formatImage(imagePath, tokenToGetInSequenceDir, guiToDisplay));

    RETURN_IF_FALSE(formatButtons(buttons, defaultButton, guiToDisplay));

    _description = description;

    return displayHmiImpl(name, guiToDisplay);
}

bool HmiDisplaySimpleTestView::updateHmi(const QString &description,
                                         quint8 buttons,
                                         const QString &imagePath,
                                         const QString &tokenToGetInSequenceDir,
                                         ButtonEnum defaultButton)
{
    JsonObject guiToDisplay;

    QString tmpDescription(description);

    if(description.isEmpty())
    {
        tmpDescription = _description;
    }

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRadioListButtonsRequired(getName(),
                                                                           tmpDescription,
                                                                           guiToDisplay));

    RETURN_IF_FALSE(formatImage(imagePath, tokenToGetInSequenceDir, guiToDisplay));

    RETURN_IF_FALSE(formatButtons(buttons, defaultButton, guiToDisplay));

    return updateHmiImpl(guiToDisplay);
}

void HmiDisplaySimpleTestView::setButtonName(HmiDisplaySimpleTestView::ButtonEnum button,
                                             const QString &labelName,
                                             char keyShortCut)
{
    _buttonsInfoOverriden[button] = { labelName, keyShortCut };
}

bool HmiDisplaySimpleTestView::formatButtons(quint8 buttons,
                                             ButtonEnum defaultButton,
                                             JsonObject &guiToDisplay)
{
    auto fillParamButton = [this](ButtonEnum buttonId,
                                  const QString &buttonKey,
                                  const QString &buttonLabel,
                                  JsonObject &guiToDisplay,
                                  char buttonShortcut,
                                  const QString &fontColor,
                                  const QString &backgroundColor)
    {
        QString label = buttonLabel;
        char shortcut = buttonShortcut;

        if(_buttonsInfoOverriden.contains(buttonId))
        {
            const ButtonInfo &info = _buttonsInfoOverriden[buttonId];
            label = info.label;
            shortcut = info.shortcut;
        }

        return BankJsonFormatter::formatParamAddListButtonElem(buttonKey,
                                                               label,
                                                               guiToDisplay,
                                                               shortcut,
                                                               fontColor,
                                                               backgroundColor);
    };

    QString defaultButtonKey;

    if((buttons & OkButton) != 0)
    {
        RETURN_IF_FALSE(fillParamButton(OkButton,
                                        okButtonKey,
                                        tr(okButtonLabel),
                                        guiToDisplay,
                                        okKeyShorcut,
                                        okFontColor,
                                        okBackgroundColor));

        if(defaultButton == OkButton)
        {
            defaultButtonKey = okButtonKey;
        }
    }

    if((buttons & CancelButton) != 0)
    {
        RETURN_IF_FALSE(fillParamButton(CancelButton,
                                        cancelButtonKey,
                                        tr(cancelButtonLabel),
                                        guiToDisplay,
                                        cancelKeyShorcut,
                                        cancelFontColor,
                                        cancelBackgroundColor));

        if(defaultButton == CancelButton)
        {
            defaultButtonKey = cancelButtonKey;
        }
    }

    if((buttons & NotOkButton) != 0)
    {
        RETURN_IF_FALSE(fillParamButton(NotOkButton,
                                        notOkButtonKey,
                                        tr(notOkButtonLabel),
                                        guiToDisplay,
                                        notOkKeyShorcut,
                                        notOkFontColor,
                                        notOkBackgroundColor));

        if(defaultButton == NotOkButton)
        {
            defaultButtonKey = notOkButtonKey;
        }
    }

    if(!defaultButtonKey.isEmpty())
    {
        RETURN_IF_FALSE(BankJsonFormatter::formatParamRadioListDefault(defaultButtonKey,
                                                                       guiToDisplay));
    }

    setButtons(buttons);
    return true;
}

bool HmiDisplaySimpleTestView::parseDisplayResult(const JsonArray &valuesSet)
{
    QString buttonClicked;
    RETURN_IF_FALSE(BankJsonParser::getStringValue(valuesSet, getName(), buttonClicked));

    if(((_buttons & NotOkButton)  != 0) && buttonClicked == notOkButtonKey)
    {
        _buttonClicked = NotOkButton;
        return false;
    }

    if(((_buttons & CancelButton) != 0) && buttonClicked == cancelButtonKey)
    {
        _buttonClicked = CancelButton;
        return false;
    }

    if(((_buttons & OkButton) != 0) && buttonClicked == okButtonKey)
    {
        _buttonClicked = OkButton;
        return true;
    }

    // Unknown button
    qWarning() << "Unknown clicked button: " << buttonClicked;
    return false;
}
