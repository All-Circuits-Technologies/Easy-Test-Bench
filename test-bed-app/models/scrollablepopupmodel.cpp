// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "scrollablepopupmodel.hpp"

#include <QDebug>
#include <QQmlEngine>


ScrollablePopUpModel::ScrollablePopUpModel(PopUpBtnType::Enums neededBtns,
                                           PopUpBtnType::Enum primaryBtn,
                                           QObject *parent) :
    QObject(parent)
{
    bool modified;
    setBtnsWithoutSignal(neededBtns, primaryBtn, modified);
}

ScrollablePopUpModel::~ScrollablePopUpModel()
{
}

QVector<PopUpBtnType::Enum> ScrollablePopUpModel::getNeededBtns() const
{
    return PopUpBtnType::getEnumsList(_neededBtns);
}

void ScrollablePopUpModel::setBtnsWithoutSignal(PopUpBtnType::Enums neededBtns,
                                                PopUpBtnType::Enum primaryBtn,
                                                bool &atLeastOneModified)
{
    if((neededBtns != 0) &&
       primaryBtn != PopUpBtnType::Unknown &&
       (!neededBtns.testFlag(primaryBtn)))
    {
        qWarning() << "The primary btn wanted: " << primaryBtn << ", is not in the needed btns; "
                      "therefore we will use the first one as primary: " << primaryBtn;
        primaryBtn = PopUpBtnType::Unknown;
    }

    atLeastOneModified = false;

    if(_neededBtns != neededBtns)
    {
        _neededBtns = neededBtns;
        atLeastOneModified = true;
    }

    if(_primaryBtn != primaryBtn)
    {
        _primaryBtn = primaryBtn;
        atLeastOneModified = true;
    }
}

void ScrollablePopUpModel::setBtns(PopUpBtnType::Enums neededBtns, PopUpBtnType::Enum primaryBtn)
{
    bool atLeastOneModified;
    setBtnsWithoutSignal(neededBtns, primaryBtn, atLeastOneModified);

    if(atLeastOneModified)
    {
        emit btnsUpdated();
    }
}

bool ScrollablePopUpModel::isPrimary(PopUpBtnType::Enum btnType) const
{
    return (btnType == _primaryBtn);
}

QString ScrollablePopUpModel::getBtnLabel(PopUpBtnType::Enum btnType) const
{
    return tr(PopUpBtnType::toDisplayString(btnType).toLatin1().data());
}

void ScrollablePopUpModel::fireBtnClick(PopUpBtnType::Enum btnType)
{
    switch(btnType)
    {
        case PopUpBtnType::OkBtn:
            emit okBtnClicked();
            break;
        case PopUpBtnType::NoBtn:
            emit noBtnClicked();
            break;
        case PopUpBtnType::YesBtn:
            emit yesBtnClicked();
            break;
        case PopUpBtnType::Unknown:
            // Do nothing
            break;
    }
}

void ScrollablePopUpModel::fireDisplayPopUp(const QString &message)
{
    emit displayPopUp(message);
}

void ScrollablePopUpModel::registerQmlType()
{
    qmlRegisterType<ScrollablePopUpModel>("app.models", 1, 0, "ScrollablePopUpModel");
}
