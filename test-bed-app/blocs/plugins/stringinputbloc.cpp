// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "stringinputbloc.hpp"

#include <QQmlEngine>


StringInputBloc::StringInputBloc(QObject *parent) :
    APluginInputBloc(parent)
{
    connect(this, &APluginInputBloc::inputModelChanged,
            this, &StringInputBloc::onModelUpdated);
}

StringInputModel *StringInputBloc::getStringInputModel()
{
    return getCastedModel<StringInputModel>();
}

void StringInputBloc::setStringInputModel(StringInputModel *stringInputModel)
{
    setInputModel(stringInputModel);
}

void StringInputBloc::onAcceptClicked(const QString &inputText)
{
    getInputModel()->setValueSet(inputText);
    getInputModel()->finishView();
}

void StringInputBloc::onModelUpdated()
{
    emit castedModelChanged(getStringInputModel());
}

void StringInputBloc::registerQmlTypes()
{
    qmlRegisterType<StringInputBloc>("app.blocs", 1, 0, "StringInputBloc");
}
