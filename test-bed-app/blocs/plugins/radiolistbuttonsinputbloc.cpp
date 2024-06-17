// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "radiolistbuttonsinputbloc.hpp"

#include "models/plugins/aplugininputmodel.hpp"

#include <QQmlEngine>
#include <QTimer>


RadioListButtonsInputBloc::RadioListButtonsInputBloc(QObject *parent) : APluginInputBloc(parent)
{
    connect(this, &APluginInputBloc::inputModelChanged,
            this, &RadioListButtonsInputBloc::onModelUpdated);
}

RadioListButtonsInputBloc::~RadioListButtonsInputBloc()
{
}

void RadioListButtonsInputBloc::onBtnSelected(const QString &btnKey)
{
    bool found = false;
    RadioListButtonsInputModel * model = getRadioListButtonsInputModel();
    QVector<ButtonInputModel*> itemsList = model->getButtonsItemList();
    for(auto citer = itemsList.cbegin(); citer != itemsList.cend(); ++citer) {
        if((*citer)->getKey() == btnKey)
        {
            found = true;
            break;
        }
    }

    if(!found)
    {
        qWarning() << "The clicked button is unknown, the key doesn't exist: " << btnKey;
        return;
    }

    model->setValueSet(btnKey);
    model->finishView();
}

RadioListButtonsInputModel *RadioListButtonsInputBloc::getRadioListButtonsInputModel() const
{
    return getCastedModel<RadioListButtonsInputModel>();
}

void RadioListButtonsInputBloc::onModelUpdated()
{
    emit castedModelChanged(getRadioListButtonsInputModel());
}

void RadioListButtonsInputBloc::registerQmlTypes()
{
    qmlRegisterType<RadioListButtonsInputBloc>("app.blocs", 1, 0, "RadioListButtonsInputBloc");
}
