// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "tst_seriallinkplugin.hpp"

#include <QtTest>
#include <QCoreApplication>
#include <QSignalSpy>

#include "test-bed-lib/core/globalmanager.hpp"
#include "test-bed-lib/sequence/nodessequence.hpp"
#include "test-bed-lib/sequence/sequencefile.hpp"


SerialLinkPluginTest::SerialLinkPluginTest()
{
}

SerialLinkPluginTest::~SerialLinkPluginTest()
{
}

void SerialLinkPluginTest::initTestCase()
{
    GlobalManager::create();
}

void SerialLinkPluginTest::test_seriallinkmodule_data()
{
    QTest::addColumn<QString>("sequenceFilePath");
    QTest::addColumn<bool>("expectedSequenceSuccess");

    /* Non-mandatory searches will always succeed, even when device is not found */
    QTest::newRow("Search first serial port [not mandatory]")
            << ":/search-first-serial-link.json" << true;

    QTest::newRow("Search first APS printer (by USB vendor ID) [not mandatory]")
            << ":/search-first-aps-serial-link.json" << true;

    /* Mandatory searches will fail if serial is not found */
    QTest::newRow("Search dummy serial port [mandatory] (should fail as expected)")
            << ":/search-non-existent-serial.json" << false;

    /* Setup first port should succeed (requires at least one serial port on the computer) */
    QTest::newRow("Setup first found serial (115200 bauds) (requires at least one serial port)")
            << ":/setup-first-serial-link.json" << true;

    /* Setup first port should succeed (requires at least one serial port on the computer) */
    QTest::newRow("Write and read echo on first serial port (requires TX wired to RX)")
            << ":/echo-first-serial-link.json" << true;
}

void SerialLinkPluginTest::test_seriallinkmodule()
{
    QFETCH(QString, sequenceFilePath);
    QFETCH(bool, expectedSequenceSuccess);

    runSequence(sequenceFilePath, expectedSequenceSuccess);
}

void SerialLinkPluginTest::loadSequenceFile(QFile *tmpSeqFile,
                                            SequenceFile &sequenceFile,
                                            QSharedPointer<NodesSequence> &nodeSequenceLoaded)
{
    QVERIFY(sequenceFile.initAndSetSequenceFile(tmpSeqFile));

    QSignalSpy allThreadsSpy(&sequenceFile, &SequenceFile::allThreadsRunning);
    QVERIFY(sequenceFile.loadSequenceDataAndLaunchThreads(1));

    QVERIFY(allThreadsSpy.wait(5000));

    QVector<SequenceThread *> sequenceThreads = sequenceFile.accessSequenceThreads();

    QCOMPARE(sequenceThreads.length(), 1);

    SequenceThread *sequenceThread = sequenceThreads.first();

    QSignalSpy spyLoading(sequenceThread, &SequenceThread::nodesSequenceLoaded);

    sequenceThread->setSequenceData(sequenceFile.accessJsonArray());
    sequenceThread->cleanAndLoadNodesSequence();

    QVERIFY(spyLoading.wait(5000));

    QCOMPARE(spyLoading.count(), 1);

    QList<QVariant> arguments = spyLoading.takeFirst();

    QCOMPARE(arguments.length(), 1);

    if(arguments.at(0).toBool())
    {
        nodeSequenceLoaded = sequenceThread->getLinkedNodesSequence().constCast<NodesSequence>();
        return;
    }

    nodeSequenceLoaded = QSharedPointer<NodesSequence>();
}

void SerialLinkPluginTest::runSequence(const QSharedPointer<NodesSequence> &nodesSequence,
                                       bool expectedFound)
{
    QVERIFY(!nodesSequence.isNull());

    QSignalSpy spy(nodesSequence.data(), &NodesSequence::finished);

    connect(nodesSequence->getStateMachine().data(),
            &QStateMachine::destroyed,
            this,
            [=]() { qDebug() << "QStateMachine destroyed"; });

    connect(nodesSequence.data(), &NodesSequence::destroyed,
            this,                 [=]() { qDebug() << "Nodes sequence destroyed"; });

    QVERIFY(nodesSequence->startSequence());

    QVERIFY(spy.wait(5000));

    QCOMPARE(spy.count(), 1);

    QList<QVariant> arguments = spy.takeFirst();

    QCOMPARE(arguments.length(), 1);

    QCOMPARE(arguments.at(0).toBool(), expectedFound);
}

void SerialLinkPluginTest::runSequence(const QString &filePath, bool expectedFound)
{
    QFile *fileIo = new QFile(filePath);
    SequenceFile sequenceFile;
    QSharedPointer<NodesSequence> nodesSequence;

    loadSequenceFile(fileIo, sequenceFile, nodesSequence);
    QVERIFY(!nodesSequence.isNull());
    runSequence(nodesSequence, expectedFound);
}

QTEST_MAIN(SerialLinkPluginTest)
