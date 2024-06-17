// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QJsonArray>

#include "ajsonparser.hpp"

class SequenceFile;


/** @brief Load the jsonArray from the sequence */
class SequenceFileJsonParser : public AJsonParser
{
    Q_OBJECT

    private:
    /** @brief Class constructor
        @param parent The class parent */
        explicit SequenceFileJsonParser(QObject *parent = nullptr);


    public:
        /** @brief Load and parse sequence file
            @param sequenceDevice The device to load and parse in order to get the sequence infos
            @param jsonArray Variable to stock the jsonArray in
            @return Return true if correct, false if an error occurred */
        static bool loadSequenceFile(QIODevice &sequenceDevice, QJsonArray &jsonArray);
};
