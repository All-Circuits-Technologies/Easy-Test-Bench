// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>
#include <QTemporaryFile>

#include "jsonutility/jsontypedefs.hpp"

class NodesSequence;
class SequenceFile;


class SequenceFileTest : public QObject
{
    Q_OBJECT

    public:
        SequenceFileTest();
        ~SequenceFileTest();

    private slots:
        void initTestCase();
        void test_elementnodeparsing_data();
        void test_elementnodeparsing();
        void test_genericsequencebuildinglimits_data();
        void test_genericsequencebuildinglimits();
        void test_simplesequencelaunching_data();
        void test_simplesequencelaunching();
        void test_stoppingsequence_data();
        void test_stoppingsequence();
        void test_pausingsequence_data();
        void test_pausingsequence();
        void test_stoppausingsequence_data();
        void test_stoppausingsequence();

    private:
        void loadSequenceFile(QFile *tmpSequenceFile,
                              SequenceFile &sequenceFile,
                              QSharedPointer<NodesSequence> &nodeSequenceLoaded);

        bool loadJsonArray(const QString &docPath, JsonArray &jsonArray);

        bool createTemporaryFile(const JsonArray &data, QTemporaryFile &tempFile);

        bool setAttribute(const QString &attributeName,
                          const QVariant &attributeValue,
                          JsonObject &object);

        bool removeAttribute(const QString &attributeName,
                             JsonObject &object);

        bool setAttributes(const QString &distinctiveObjectAttributeName,
                           const QVariant &distinctiveObjectAttributeValue,
                           const JsonObject &elementsToSet,
                           JsonArray &array);

        bool removeAttributes(const QString &distinctiveObjectAttributeName,
                              const QVariant &distinctiveObjectAttributeValue,
                              const QVector<QString> &attributesNameToRemove,
                              JsonArray &array);

        bool findObject(const QString &distinctiveObjectAttributeName,
                        const QVariant &distinctiveObjectAttributeValue,
                        JsonArray &array,
                        JsonArray::iterator &objectIter);
};
