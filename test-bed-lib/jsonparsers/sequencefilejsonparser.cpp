// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "sequencefilejsonparser.hpp"

#include <QDebug>


SequenceFileJsonParser::SequenceFileJsonParser(QObject *parent) :
    AJsonParser(parent)
{
}

bool SequenceFileJsonParser::loadSequenceFile(QIODevice &sequenceDevice, QJsonArray &jsonArray)
{
    return loadJsonFileAndGetArray(sequenceDevice, jsonArray);
}
