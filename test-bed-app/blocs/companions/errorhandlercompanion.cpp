// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "errorhandlercompanion.hpp"

#include "core/hmimanager.hpp"
#include "services/sizemanager.hpp"


ErrorHandlerCompanion::ErrorHandlerCompanion(QObject *parent)
    : QObject{parent},
      _popUpModel(new ScrollablePopUpModel(PopUpBtnType::OkBtn, PopUpBtnType::OkBtn, this))
{
}

ErrorHandlerCompanion::~ErrorHandlerCompanion()
{
}

QString ErrorHandlerCompanion::formatErrorMessages(const QStringList &messages)
{
    int marginRelativeSize = HmiManager::instance().accessManager<SizeManager>()
                                                    .getSizeBasedOnAppWidth(errorMsgsMarginSize);

    QString errorMessage = "";

    for(int idx = messages.length() -1 ; idx >= 0; idx--)
    {
        errorMessage += QString(liTag).arg(marginRelativeSize).arg(messages.at(idx));
    }

    errorMessage = QString(ulTag).arg(errorMessage);

    return errorMessage;
}

void ErrorHandlerCompanion::popUpBtnsFormat(bool withClosedQuestion)
{
    if(withClosedQuestion)
    {
        _popUpModel->setBtns(PopUpBtnType::YesBtn | PopUpBtnType::NoBtn, PopUpBtnType::YesBtn);
    }
    else
    {
        _popUpModel->setBtns(PopUpBtnType::OkBtn, PopUpBtnType::OkBtn);
    }
}
