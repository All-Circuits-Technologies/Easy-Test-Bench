// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "aplugininputbloc.hpp"

#include <QTimer>
#include<QQmlEngine>


APluginInputBloc::APluginInputBloc(QObject *parent) : AbstractBloc(parent)
{
}

void APluginInputBloc::registerQmlTypes()
{
    qmlRegisterType<APluginInputBloc>("app.blocs", 1, 0, "APluginInputBloc");
}

void APluginInputBloc::initBloc()
{
    emit blocInitialized();
}

void APluginInputBloc::setInputModel(APluginInputModel *value)
{
    if(_inputModel != value)
    {
        _inputModel = value;
        emit inputModelChanged(_inputModel);
    }
}
