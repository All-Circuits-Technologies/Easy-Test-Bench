// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "afromrefoperator.hpp"

#include "testbedcore/testbedglobal.hpp"


AFromRefOperator::AFromRefOperator(const QString &moduleClassName,
                                   TypeManaged::Enum typeManaged,
                                   QObject *parent) :
    ABaseOperator(moduleClassName, typeManaged, parent)
{
}

bool AFromRefOperator::initInputsFormat(JsonArray &inputsFormat) const
{
    return addInOutFormat(inputPinName, inputPinDesc, getTypeManaged(), inputsFormat);
}
