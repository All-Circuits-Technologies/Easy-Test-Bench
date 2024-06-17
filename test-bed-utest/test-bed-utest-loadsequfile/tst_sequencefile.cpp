// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "tst_sequencefile.hpp"

#include <QtTest>
#include <QCoreApplication>
#include <QFile>
#include <QMutex>

#include "core/globalmanager.hpp"
#include "jsonparsers/nodestojsonparser.hpp"
#include "jsonutility/jsonconverter.hpp"
#include "jsonutility/jsoncomparator.hpp"
#include "jsonutility/jsonhelper.hpp"
#include "jsonutility/jsonstringify.hpp"
#include "sequence/checksequencevalidity.hpp"
#include "sequence/nodessequence.hpp"
#include "sequence/nodes/asequencenode.hpp"
#include "sequence/sequencefile.hpp"
#include "testbedcore/parsers/jsonparsersconstants.hpp"
#include "testbedcore/testbedglobal.hpp"
#include "waitutility/waithelper.hpp"


SequenceFileTest::SequenceFileTest()
{
}

SequenceFileTest::~SequenceFileTest()
{
}

void SequenceFileTest::initTestCase()
{
    GlobalManager::create();
}

void SequenceFileTest::test_elementnodeparsing_data()
{
    // Parse node sequences
    QTest::addColumn<QString>("sequenceFilePath");
    QTest::addColumn<bool>("expectedSuccess");
    QTest::addColumn<bool>("strictFileComparingSuccess");
    QTest::addColumn<bool>("checkingBeforeLoadingSuccess");

    QTest::newRow("Test building minimalist nodesSequence")
            << ":/nodessequence-minimal.json" << true << true << false;

    QTest::newRow("Test building nodesSequence with all possible elements")
            << ":/nodessequence-withallelems.json" << true << true << false;

    QTest::newRow("Test building nodesSequence with an element in excess")
            << ":/nodessequence-withotherelem.json" << true << false << false;

    QTest::newRow("Test building initialNode with parenty")
            << ":/initialnode.json" << true << true << false;

    QTest::newRow("Test building mergeNode with parenty")
            << ":/mergenode.json" << true << true << false;

    QTest::newRow("Test building finalNode with parenty")
            << ":/finalnode.json" << true << true << false;

    QTest::newRow("Test building variableNode with parenty")
            << ":/variablenode.json" << true << true << false;

    QTest::newRow("Test building constantNode with parenty")
            << ":/constantnode.json" << true << true << false;

    QTest::newRow("Test building sequenceModuleNode with parenty")
            << ":/sequencemodulenode.json" << true << true << false;

    QTest::newRow("Test building forkNode and joinNode with parenty")
            << ":/forknode.json" << true << true << false;

    QTest::newRow("Test building groupNode with parenty")
            << ":/groupnode.json" << true << true << false;

    QTest::newRow("Test building decisionalNode with parenty")
            << ":/decisionalnode.json" << true << true << false;

    QTest::newRow("Test building inclusionNode with parenty")
            << ":/inclusionnode.json" << true << true << false;

    QTest::newRow("Test building complex decisional node")
            << ":/decisionalnode-complex.json" << true << true << false;

    QTest::newRow("Test building complex fork node")
            << ":/forknode-complex.json" << true << true << false;

    QTest::newRow("Test building complex group node")
            << ":/groupnode-complex.json" << true << true << false;

    QTest::newRow("Test wrong sequence with two join nodes for same fork")
            << ":/twojoinnodes-for-same-fork.json" << false << false << false;

    QTest::newRow("Test wrong sequence with two fork nodes for same join")
            << ":/twoforknodes-for-same-join.json" << false << false << false;

    QTest::newRow("Test building group node ready to be launched")
            << ":/valid-to-launch-groupnode.json" << true << true << true;
}

void SequenceFileTest::test_elementnodeparsing()
{
    QFETCH(QString, sequenceFilePath);
    QFETCH(bool, expectedSuccess);
    QFETCH(bool, strictFileComparingSuccess);
    QFETCH(bool, checkingBeforeLoadingSuccess);

    QFile *sequenceFile = new QFile(sequenceFilePath, this);

    QVERIFY(sequenceFile->exists());

    SequenceFile seqFile;

    QSharedPointer<NodesSequence> nodesSequence;
    loadSequenceFile(sequenceFile, seqFile, nodesSequence);

    QCOMPARE(!nodesSequence.isNull(), expectedSuccess);

    if(!expectedSuccess)
    {
        // No need to go further in that case
        return;
    }

    JsonArray sequenceArray;
    QVERIFY(NodesToJsonParser::sequenceFileToJsonArray(seqFile, sequenceArray));

    QByteArray sequenceParsedStr;

    QVERIFY(JsonStringify::toCompactString(sequenceArray, sequenceParsedStr));

    qInfo() << "Sequence parsed: " << sequenceParsedStr.constData();

    JsonArray expectedResult;
    QVERIFY(loadJsonArray(sequenceFilePath, expectedResult));

    QByteArray fromFile;

    QVERIFY(JsonStringify::toCompactString(expectedResult, fromFile));

    qInfo() << "From file: " << fromFile.constData();

    QCOMPARE(JsonComparator::isEqual(expectedResult, sequenceArray), strictFileComparingSuccess);

    QString error;
    QSharedPointer<ANode> guiltyNode;
    QCOMPARE(CheckSequenceValidity::isValid(nodesSequence, error, guiltyNode),
             checkingBeforeLoadingSuccess);
}

void SequenceFileTest::test_genericsequencebuildinglimits_data()
{
    QTest::addColumn<QString>("sequenceFilePath");
    QTest::addColumn<QString>("objectName");
    QTest::addColumn<QVector<QString>>("attributesToRemove");
    QTest::addColumn<JsonObject>("attributesToSet");
    QTest::addColumn<bool>("expectedBuildingSuccess");
    QTest::addColumn<bool>("strictFileComparingSuccess");
    QTest::addColumn<bool>("checkingBeforeLoadingSuccess");

    QTest::newRow("Test adding extra attribute nodesSequence with all possible elements")
            << ":/nodessequence-withallelems.json"
            << "nodesSequenceName"
            << QVector<QString>({})
            << JsonObject({ { "paupiette", QVariant("lardons") } })
            << true
            << false
            << false;

    QTest::newRow("Test adding extra attribute initialNode with parenty")
            << ":/initialnode.json"
            << "initialNodeName"
            << QVector<QString>({})
            << JsonObject({ { "paupiette", QVariant("lardons") } })
            << true
            << false
            << false;

    QTest::newRow("Test adding extra attribute mergeNode with parenty")
            << ":/mergenode.json"
            << "mergeNodeName"
            << QVector<QString>({})
            << JsonObject({ { "paupiette", QVariant("lardons") } })
            << true
            << false
            << false;

    QTest::newRow("Test adding extra attribute finalNode with parenty")
            << ":/finalnode.json"
            << "finalNodeName"
            << QVector<QString>({})
            << JsonObject({ { "paupiette", QVariant("lardons") } })
            << true
            << false
            << false;

    QTest::newRow("Test adding extra attribute variableNode with parenty")
            << ":/variablenode.json"
            << "variableNode"
            << QVector<QString>({})
            << JsonObject({ { "paupiette", QVariant("lardons") } })
            << true
            << false
            << false;

    QTest::newRow("Test adding extra attribute constantNode with parenty")
            << ":/constantnode.json"
            << "constantNode"
            << QVector<QString>({})
            << JsonObject({ { "paupiette", QVariant("lardons") } })
            << true
            << false
            << false;

    QTest::newRow("Test adding extra attribute sequenceModuleNode with parenty")
            << ":/sequencemodulenode.json"
            << "sequenceModuleNode"
            << QVector<QString>({})
            << JsonObject({ { "paupiette", QVariant("lardons") } })
            << true
            << false
            << false;

    QTest::newRow("Test adding extra attribute inclusionNode with parenty")
            << ":/inclusionnode.json"
            << "inclusionNodeName"
            << QVector<QString>({})
            << JsonObject({ { "paupiette", QVariant("lardons") } })
            << true
            << false
            << false;

    QTest::newRow("Test adding extra attribute complex decisional node")
            << ":/decisionalnode-complex.json"
            << "decisionalNodeName"
            << QVector<QString>({})
            << JsonObject({ { "paupiette", QVariant("lardons") } })
            << true
            << false
            << false;

    QTest::newRow("Test adding extra attribute complex fork node")
            << ":/forknode-complex.json"
            << "forkNodeName"
            << QVector<QString>({})
            << JsonObject({ { "paupiette", QVariant("lardons") } })
            << true
            << false
            << false;

    QTest::newRow("Test adding extra attribute complex group node")
            << ":/groupnode-complex.json"
            << "groupNodeName"
            << QVector<QString>({})
            << JsonObject({ { "paupiette", QVariant("lardons") } })
            << true
            << false
            << false;

    QTest::newRow("Test empty type for a node")
            << ":/initialnode.json"
            << "initialNodeName"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Generic::TypeName, QVariant("") } })
            << false
            << false
            << false;

    QTest::newRow("Test unknown type for a node")
            << ":/initialnode.json"
            << "initialNodeName"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Generic::TypeName, QVariant("lunatique") } })
            << false
            << false
            << false;

    QTest::newRow("Test empty parent name for a node")
            << ":/launch-sequence-minimal.json"
            << "initialNodeName"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::ParentInstanceNameAttribute,
                              QVariant("") } })
            << true
            << false
            << false;

    QTest::newRow("Test unknown parent name for a node")
            << ":/launch-sequence-minimal.json"
            << "initialNodeName"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::ParentInstanceNameAttribute,
                              QVariant("lunatique") } })
            << true
            << false
            << false;

    QTest::newRow("Test known but wrong parent name for a node")
            << ":/forknode-complex.json"
            << "sequenceModuleNode21"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::ParentInstanceNameAttribute,
                              QVariant("forkNodeName") } })
            << false
            << false
            << false;

    QTest::newRow("Test empty tonode name for a node")
            << ":/launch-sequence-minimal.json"
            << "initialNodeName"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::ToNodeNameAttribute, QVariant("") } })
            << true
            << true
            << false;

    QTest::newRow("Test unknown tonode name for a node")
            << ":/forknode-complex.json"
            << "sequenceModuleNode21"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::ToNodeNameAttribute,
                              QVariant("trublion") } })
            << false
            << false
            << false;

    QTest::newRow("Test unknown typemanaged for a variablenode")
            << ":/variablenode.json"
            << "variableNode"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                              QVariant("loustic") } })
            << false
            << false
            << false;

    QTest::newRow("Test unknown typemanaged for a constantnode")
            << ":/constantnode.json"
            << "constantNode2"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                              QVariant("fourmi") } })
            << false
            << false
            << false;

    QTest::newRow("Test an unknown unit with a variablenode")
            << ":/variablenode.json"
            << "variableNode"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::UnitNameAttribute,
                              QVariant("bpm") }})
            << true
            << true
            << false;

    QTest::newRow("Test min limit of powerOfTenLimit with a variablenode")
            << ":/variablenode.json"
            << "variableNode"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::UnitNameAttribute,
                              QVariant("-24") } })
            << true
            << true
            << false;

    QTest::newRow("Test max limit of powerOfTenLimit with a variablenode")
            << ":/variablenode.json"
            << "variableNode"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::UnitNameAttribute,
                              QVariant("24") } })
            << true
            << true
            << false;

    QTest::newRow("Test overflow min limit of powerOfTenLimit with a variablenode")
            << ":/variablenode.json"
            << "variableNode"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::UnitPowerOf10NameAttribute,
                              QVariant("-25") } })
            << false
            << false
            << false;

    QTest::newRow("Test overflow max limit of powerOfTenLimit with a variablenode")
            << ":/variablenode.json"
            << "variableNode"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::UnitPowerOf10NameAttribute,
                              QVariant("25") } })
            << false
            << false
            << false;

    QTest::newRow("Test wrong value(s) name for number and for constant node")
            << ":/constantnode.json"
            << "constantNode"
            << QVector<QString>({ JsonParsersConstants::Sequence::ValueNameAttribute })
            << JsonObject({ { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant("5,25") }})
            << false
            << false
            << false;

    QTest::newRow("Test wrong value(s) name for string and for constant node")
            << ":/constantnode.json"
            << "constantNode2"
            << QVector<QString>({ JsonParsersConstants::Sequence::ValueNameAttribute })
            << JsonObject({ { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant("constantValue") }})
            << false
            << false
            << false;

    QTest::newRow("Test wrong value(s) name for bool and for constant node")
            << ":/constantnode.json"
            << "constantNode3"
            << QVector<QString>({ JsonParsersConstants::Sequence::ValueNameAttribute })
            << JsonObject({ { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant(true) }})
            << false
            << false
            << false;

    QTest::newRow("Test wrong value(s) name for list<number> and for constant node")
            << ":/constantnode.json"
            << "constantNode4"
            << QVector<QString>({ JsonParsersConstants::Sequence::ValuesNameAttribute })
            << JsonObject({ { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(JsonArray({ "3", "52,36", "85"  })) }})
            << false
            << false
            << false;

    QTest::newRow("Test wrong value(s) name for list<string> and for constant node")
            << ":/constantnode.json"
            << "constantNode5"
            << QVector<QString>({ JsonParsersConstants::Sequence::ValuesNameAttribute })
            << JsonObject({ { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(JsonArray({ "value1", "valueJoker" })) }})
            << false
            << false
            << false;

    QTest::newRow("Test wrong value(s) name for list<bool> and for constant node")
            << ":/constantnode.json"
            << "constantNode6"
            << QVector<QString>({ JsonParsersConstants::Sequence::ValuesNameAttribute })
            << JsonObject({ { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(JsonArray({ true, false, true })) }})
            << false
            << false
            << false;

    QTest::newRow("Test unknown typemanaged for an input")
            << ":/launch-sequence-with-decisional.json"
            << "sequenceModuleNode3"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::InputsNameAttribute,
                    QVariant::fromValue(JsonArray({
                        QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputNumberPin" },
                            { JsonParsersConstants::Sequence::TypeManagedNameAttribute, "bateau" },
                            { JsonParsersConstants::Sequence::ReferenceNodeNameAttribute,
                              "variableNode1" }
                        }))
                    }))
               } })
            << false
            << false
            << false;

    QTest::newRow("Test unknown unit for an input")
            << ":/launch-sequence-with-decisional.json"
            << "sequenceModuleNode3"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::InputsNameAttribute,
                    QVariant::fromValue(JsonArray({
                        QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputNumberPin" },
                            { JsonParsersConstants::Sequence::TypeManagedNameAttribute, "number" },
                            { JsonParsersConstants::Sequence::ReferenceNodeNameAttribute,
                              "variableNode1" },
                            { JsonParsersConstants::Sequence::UnitNameAttribute, "bpm" },
                            { JsonParsersConstants::Sequence::UnitPowerOf10NameAttribute, "3" }
                        }))
                   }))
               }})
            << false
            << false
            << false;

    QTest::newRow("Test unknown internalbinding reference node for an input")
            << ":/launch-sequence-with-group.json"
            << "groupNodeName"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::InputsNameAttribute,
                   QVariant::fromValue(JsonArray({
                        QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "in1" },
                            { JsonParsersConstants::Sequence::TypeManagedNameAttribute, "number" },
                            { JsonParsersConstants::Sequence::ReferenceNodeNameAttribute,
                              "variableNode1" },
                            { JsonParsersConstants::Sequence::InternalBindingRefNodeNameAttribute,
                              "" }
                        }))
                   }))
               }})
            << true
            << false
            << false;

    QTest::newRow("Test unknown internalbinding reference node for an output")
            << ":/launch-sequence-with-group.json"
            << "groupNodeName"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::OutputsNameAttribute,
                   QVariant::fromValue(JsonArray({
                        QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "out1" },
                            { JsonParsersConstants::Sequence::TypeManagedNameAttribute, "number" },
                            { JsonParsersConstants::Sequence::VariableNodeNameAttribute,
                              "variableNode2" },
                            { JsonParsersConstants::Sequence::InternalBindingVarNodeNameAttribute,
                              "" }
                        }))
                   }))
               }})
            << true
            << false
            << false;

    QTest::newRow("Test unknown typemanaged for an output")
            << ":/launch-sequence-with-decisional.json"
            << "sequenceModuleNode3"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::OutputsNameAttribute,
                   QVariant::fromValue(JsonArray({
                        QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "outputNumberPin" },
                            { JsonParsersConstants::Sequence::TypeManagedNameAttribute, "bateau" },
                            { JsonParsersConstants::Sequence::ReferenceNodeNameAttribute,
                              "variableNode1" }
                        }))
                   }))
               }})
            << false
            << false
            << false;

    QTest::newRow("Test unknown unit for an output")
            << ":/launch-sequence-with-decisional.json"
            << "sequenceModuleNode3"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::OutputsNameAttribute,
                   QVariant::fromValue(JsonArray({
                        QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "outputNumberPin" },
                            { JsonParsersConstants::Sequence::TypeManagedNameAttribute, "number" },
                            { JsonParsersConstants::Sequence::VariableNodeNameAttribute,
                              "variableNode1" },
                            { JsonParsersConstants::Sequence::UnitNameAttribute, "bpm" },
                            { JsonParsersConstants::Sequence::UnitPowerOf10NameAttribute, "3" }
                        }))
                   }))
               }})
            << false
            << false
            << false;

    QTest::newRow("Test wrong number for a string value for a constantnode")
            << ":/constantnode.json"
            << "constantNode2"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::ValueNameAttribute, QVariant(2.3) }})
            << false
            << false
            << false;

    QTest::newRow("Test wrong boolean for a string value for a constantnode")
            << ":/constantnode.json"
            << "constantNode2"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::ValueNameAttribute, QVariant(true) }})
            << false
            << false
            << false;

    QTest::newRow("Test wrong string list for a string value for a constantnode")
            << ":/constantnode.json"
            << "constantNode2"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(JsonArray({ "grille-pain", "louttre du sud" }))}})
            << false
            << false
            << false;

    QTest::newRow("Test wrong string for a number value for a constantnode")
            << ":/constantnode.json"
            << "constantNode"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant("pinnochio") }})
            << false
            << false
            << false;

    QTest::newRow("Test wrong boolean for a number value for a constantnode")
            << ":/constantnode.json"
            << "constantNode"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant(false) }})
            << false
            << false
            << false;

    QTest::newRow("Test wrong number list for a number value for a constantnode")
            << ":/constantnode.json"
            << "constantNode"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(JsonArray({ 5.6, 8 })) }})
            << false
            << false
            << false;

    QTest::newRow("Test wrong string for a boolean value for a constantnode")
            << ":/constantnode.json"
            << "constantNode3"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant("luciole") }})
            << false
            << false
            << false;

    QTest::newRow("Test wrong number for a boolean value for a constantnode")
            << ":/constantnode.json"
            << "constantNode3"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::ValueNameAttribute, QVariant(5.3) }})
            << false
            << false
            << false;

    QTest::newRow("Test wrong boolean list for a boolean value for a constantnode")
            << ":/constantnode.json"
            << "constantNode3"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(JsonArray({ true, false })) }})
            << false
            << false
            << false;

    QTest::newRow("Test wrong string list for a boolean list value for a constantnode")
            << ":/constantnode.json"
            << "constantNode6"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ "mario", "luigi" })) }})
            << false
            << false
            << false;

    QTest::newRow("Test wrong number list for a boolean list value for a constantnode")
            << ":/constantnode.json"
            << "constantNode6"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ 2.3, 9 })) }})
            << false
            << false
            << false;

    QTest::newRow("Test wrong boolean for a boolean list value for a constantnode")
            << ":/constantnode.json"
            << "constantNode6"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant(false) }})
            << false
            << false
            << false;

    QTest::newRow("Test wrong boolean list for a string list value for a constantnode")
            << ":/constantnode.json"
            << "constantNode5"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ false, true })) }})
            << false
            << false
            << false;

    QTest::newRow("Test wrong number list for a string list value for a constantnode")
            << ":/constantnode.json"
            << "constantNode5"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ 2.3, 9 })) }})
            << false
            << false
            << false;

    QTest::newRow("Test wrong string for a string list value for a constantnode")
            << ":/constantnode.json"
            << "constantNode5"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant("curieux") }})
            << false
            << false
            << false;

    QTest::newRow("Test wrong boolean list for a number list value for a constantnode")
            << ":/constantnode.json"
            << "constantNode4"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ false, true })) }})
            << false
            << false
            << false;

    QTest::newRow("Test wrong string list for a number list value for a constantnode")
            << ":/constantnode.json"
            << "constantNode4"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ "rouge", "bleu" })) }})
            << false
            << false
            << false;

    QTest::newRow("Test wrong number for a number list value for a constantnode")
            << ":/constantnode.json"
            << "constantNode4"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant(56) }})
            << false
            << false
            << false;

    QTest::newRow("Test sequencemodulenode with an empty bank name")
            << ":/sequencemodulenode.json"
            << "sequenceModuleNode"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::BankNameAttribute, QVariant("") }})
            << false
            << false
            << false;

    QTest::newRow("Test sequencemodulenode with a wrong bank name")
            << ":/sequencemodulenode.json"
            << "sequenceModuleNode"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::BankNameAttribute,
                              QVariant("tartiflette") }})
            << false
            << false
            << false;

    QTest::newRow("Test sequencemodulenode with an empty module name")
            << ":/sequencemodulenode.json"
            << "sequenceModuleNode"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::ModuleNameAttribute,
                              QVariant("") }})
            << false
            << false
            << false;

    QTest::newRow("Test sequencemodulenode with a wrong module name")
            << ":/sequencemodulenode.json"
            << "sequenceModuleNode"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::ModuleNameAttribute,
                              QVariant("vert de gris") }})
            << false
            << false
            << false;

    QTest::newRow("Test sequencemodulenode with no module parameters")
            << ":/launch-sequence-with-decisional.json"
            << "sequenceModuleNode1"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::ModuleParametersNameAttribute,
                              QVariant::fromValue(JsonArray()) }})
            << false
            << false
            << false;

    QTest::newRow("Test sequencemodulenode with extra module parameters")
            << ":/launch-sequence-with-decisional.json"
            << "sequenceModuleNode1"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::ModuleParametersNameAttribute,
                   QVariant::fromValue(JsonArray({
                        QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "assignmentModifier" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, "0" }
                        })),
                        QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "Pablo" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, 3 }
                        }))
                   }))
               }})
            << true
            << false
            << true;

    QTest::newRow("Test sequencemodulenode with wrong module parameter type")
            << ":/launch-sequence-with-decisional.json"
            << "sequenceModuleNode1"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::ModuleParametersNameAttribute,
                   QVariant::fromValue(JsonArray({
                        QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "assignementModifier" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, "moto" }
                        }))
                   }))
               }})
            << false
            << false
            << false;

    QTest::newRow("Test wrong joinnode for the forknode")
            << ":/forknode-complex.json"
            << "forkNodeName"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::JoinNodeInstanceNameAttribute,
                              QVariant("sequenceModuleNode21")  }})
            << false
            << false
            << false;

    QTest::newRow("Test unknown joinnode for the forknode")
            << ":/forknode-complex.json"
            << "forkNodeName"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::JoinNodeInstanceNameAttribute,
                              QVariant("") }})
            << false
            << false
            << false;

    QTest::newRow("Test unknown forknode for the joinnode")
            << ":/forknode-complex.json"
            << "joinNodeName"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::ForkNodeInstanceNameAttribute,
                              QVariant("") }})
            << false
            << false
            << false;

    QTest::newRow("Test wrong tonodes for the forknode")
            << ":/forknode-complex.json"
            << "forkNodeName"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::ToNodesNameAttribute,
                              QVariant::fromValue(JsonArray({"sequenceModuleNode10",
                                                             "sequenceModuleNode10"})) }})
            << false
            << false
            << false;

    QTest::newRow("Test wrong value and operator transition for the decisionalnode")
            << ":/decisionalnode-complex.json"
            << "decisionalNodeName"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::ToNodesNameAttribute,
                   QVariant::fromValue(JsonArray({
                        QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Sequence::ToNodeNameAttribute,
                              "sequenceModuleNode2" },
                            { JsonParsersConstants::Sequence::OperatorAttribute, ">" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, "Martinique" }
                        }))
                   }))
               }})
            << false
            << false
            << false;

    QTest::newRow("Test wrong transition value type for the decisionalnode")
            << ":/decisionalnode-complex.json"
            << "decisionalNodeName"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::ToNodesNameAttribute,
                   QVariant::fromValue(JsonArray({
                        QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Sequence::ToNodeNameAttribute,
                              "sequenceModuleNode2" },
                            { JsonParsersConstants::Sequence::OperatorAttribute, "==" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, "Loupiotte" }
                        }))
                   }))
               }})
            << false
            << false
            << false;

    QTest::newRow("Test wrong operator name for the decisionalnode transition")
            << ":/decisionalnode-complex.json"
            << "decisionalNodeName"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::ToNodesNameAttribute,
                    QVariant::fromValue(JsonArray({
                        QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Sequence::ToNodeNameAttribute,
                              "sequenceModuleNode2" },
                            { JsonParsersConstants::Sequence::OperatorAttribute, "lui" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, "2" }
                        }))
                   }))
               }})
            << false
            << false
            << false;

    QTest::newRow("Test miss value attribute for the decisionalnode transition")
            << ":/decisionalnode-complex.json"
            << "decisionalNodeName"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::ToNodesNameAttribute,
                   QVariant::fromValue(JsonArray({
                        QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Sequence::ToNodeNameAttribute,
                              "sequenceModuleNode2" },
                            { JsonParsersConstants::Sequence::OperatorAttribute, "==" }
                        }))
                   }))
               }})
            << false
            << false
            << false;

    QTest::newRow("Test miss logical transition elements attribute for the decisionalnode")
            << ":/decisionalnode-complex.json"
            << "decisionalNodeName"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::ToNodesNameAttribute,
                   QVariant::fromValue(JsonArray({
                        QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Sequence::ToNodeNameAttribute, "sequenceModuleNode2" },
                            { JsonParsersConstants::Sequence::OperatorAttribute, "||" }
                        }))
                   }))
               }})
            << false
            << false
            << false;

    QTest::newRow("Test empty logical transition elements for the decisionalnode")
            << ":/launch-sequence-with-decisional.json"
            << "decisionalNodeName"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::ToNodesNameAttribute,
                   QVariant::fromValue(JsonArray({
                        QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Sequence::ToNodeNameAttribute, "mergeNodeName" },
                            { JsonParsersConstants::Sequence::OperatorAttribute, "||" },
                            { JsonParsersConstants::Sequence::ElementsNameAttribute,
                              QVariant::fromValue(JsonArray()) }
                        })),
                        QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Sequence::ToNodeNameAttribute,
                              "sequenceModuleNode4" },
                            { JsonParsersConstants::Sequence::OperatorAttribute, "==" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, "10" }
                        }))
                   }))
               }})
            << true
            << true
            << false;

    QTest::newRow("Test wrong value type for the decisionalnode")
            << ":/launch-sequence-with-decisional.json"
            << "decisionalNodeName"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::ToNodesNameAttribute,
                   QVariant::fromValue(JsonArray({
                        QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Sequence::ToNodeNameAttribute, "mergeNodeName" },
                            { JsonParsersConstants::Sequence::OperatorAttribute, "<" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(JsonArray({ QVariant("10") })) }
                        })),
                        QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Sequence::ToNodeNameAttribute,
                              "sequenceModuleNode4" },
                            { JsonParsersConstants::Sequence::OperatorAttribute, "==" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, "10" }
                        }))
                   }))
               }})
            << false
            << false
            << false;

    QTest::newRow("Test wrong other file for inclusion node")
            << ":/inclusionnode.json"
            << "inclusionNodeName"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::OtherFileReferencePathAttribute,
                              QVariant("belette-de-l_oued") }})
            << false
            << false
            << false;

    QTest::newRow("Test canBeUndefined and all set in group node")
            << ":/valid-to-launch-groupnode.json"
            << "groupNodeName"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::InputsNameAttribute,
                  QVariant::fromValue(JsonArray({
                      QVariant::fromValue(JsonObject({
                          { JsonParsersConstants::Generic::NameAttribute, "in1" },
                          { JsonParsersConstants::Sequence::TypeManagedNameAttribute, "number" },
                          { JsonParsersConstants::Sequence::ReferenceNodeNameAttribute,
                            "variableNode1" },
                          { JsonParsersConstants::Sequence::InternalBindingRefNodeNameAttribute,
                            "variableNode1" },
                          { JsonParsersConstants::Sequence::CanBeUndefinedAttribute, true }
                      }))
                  }))
                }})
            << true
            << true
            << true;

    QTest::newRow("Test canBeUndefined and not set external refNodeName in group node")
            << ":/valid-to-launch-groupnode.json"
            << "groupNodeName"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::InputsNameAttribute,
                  QVariant::fromValue(JsonArray({
                      QVariant::fromValue(JsonObject({
                          { JsonParsersConstants::Generic::NameAttribute, "in1" },
                          { JsonParsersConstants::Sequence::TypeManagedNameAttribute, "number" },
                          { JsonParsersConstants::Sequence::InternalBindingRefNodeNameAttribute,
                            "variableNode1" },
                          { JsonParsersConstants::Sequence::CanBeUndefinedAttribute, true }
                      }))
                  }))
                }})
            << true
            << true
            << false;

    QTest::newRow("Test if internal output node null and not external in group node")
            << ":/valid-to-launch-groupnode.json"
            << "groupNodeName"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::OutputsNameAttribute,
                  QVariant::fromValue(JsonArray({
                      QVariant::fromValue(JsonObject({
                          { JsonParsersConstants::Generic::NameAttribute, "out1" },
                          { JsonParsersConstants::Sequence::TypeManagedNameAttribute, "number" },
                          { JsonParsersConstants::Sequence::VariableNodeNameAttribute,
                            "variableNode2" }
                      }))
                  }))
                }})
            << true
            << true
            << false;

    QTest::newRow("Test if sequence is failing if the emit if error output is not present")
            << ":/launch-sequ-group-error-manage.json"
            << "groupNodeName"
            << QVector<QString>({})
            << JsonObject({ { JsonParsersConstants::Sequence::OutputsNameAttribute,
                              QVariant::fromValue(JsonArray({})) }})
            << false
            << false
            << false;
}

void SequenceFileTest::test_genericsequencebuildinglimits()
{
    QFETCH(QString, sequenceFilePath);
    QFETCH(QString, objectName);
    QFETCH(QVector<QString>, attributesToRemove);
    QFETCH(JsonObject, attributesToSet);
    QFETCH(bool, expectedBuildingSuccess);
    QFETCH(bool, strictFileComparingSuccess);
    QFETCH(bool, checkingBeforeLoadingSuccess);

    JsonArray currentFileArray;
    QVERIFY(loadJsonArray(sequenceFilePath, currentFileArray));

    QVERIFY(removeAttributes(JsonParsersConstants::Generic::UniqueNameAttribute,
                             objectName,
                             attributesToRemove,
                             currentFileArray));

    QVERIFY(setAttributes(JsonParsersConstants::Generic::UniqueNameAttribute,
                          objectName,
                          attributesToSet,
                          currentFileArray));

    QTemporaryFile *tempFile = new QTemporaryFile(this);

    QVERIFY(createTemporaryFile(currentFileArray, *tempFile));

    SequenceFile seqFile;

    QSharedPointer<NodesSequence> nodesSequence;
    loadSequenceFile(tempFile, seqFile, nodesSequence);

    QCOMPARE(!nodesSequence.isNull(), expectedBuildingSuccess);

    if(!expectedBuildingSuccess)
    {
        // If no success is expected, it's not necessary to go further
        return;
    }

    JsonArray sequenceArray;
    QVERIFY(NodesToJsonParser::sequenceFileToJsonArray(seqFile, sequenceArray));

    QByteArray sequenceParsedStr;

    QVERIFY(JsonStringify::toCompactString(sequenceArray, sequenceParsedStr));

    qInfo() << "Sequence parsed: " << sequenceParsedStr.constData();

    QCOMPARE(JsonComparator::isEqual(currentFileArray, sequenceArray), strictFileComparingSuccess);

    QString error;
    QSharedPointer<ANode> guiltyNode;
    QCOMPARE(CheckSequenceValidity::isValid(nodesSequence, error, guiltyNode),
             checkingBeforeLoadingSuccess);
}

void SequenceFileTest::test_simplesequencelaunching_data()
{
    // Sequence launching
    QTest::addColumn<QString>("sequenceFilePath");
    QTest::addColumn<bool>("expectedLoadingSuccess");
    QTest::addColumn<bool>("expectedSuccess");

    QTest::newRow("Test launching minimalist sequence")
            << ":/launch-sequence-minimal.json" << true << true;

    QTest::newRow("Test launching sequence with a sequence module")
            << ":/launch-sequ-with-seqmodule.json" << true << true;

    QTest::newRow("Test launching sequence with a fork and multiple sequence modules")
            << ":/launch-sequence-with-fork.json" << true << true;

    QTest::newRow("Test launching sequence with a decisional node")
            << ":/launch-sequence-with-decisional.json" << true << true;

    QTest::newRow("Test launching sequence with a complex decisional node")
            << ":/launch-sequ-cmplx-decisional.json" << true << true;

    QTest::newRow("Test launching sequence with a group node")
            << ":/launch-sequence-with-group.json" << true << true;

    QTest::newRow("Test launching sequence with an inclusion node")
            << ":/launch-sequ-incl-master.json" << true << true;

    QTest::newRow("Test launching failing slave sequence")
            << ":/launch-sequ-incl-slave-err.json" << true << false;

    QTest::newRow("Test launching sequence with an inclusion node which will fire an error")
            << ":/launch-sequ-incl-master-slave-err.json" << true << true;

    QTest::newRow("Test launching sequence with a group managing internaly an error")
            << ":/launch-sequ-group-error-manage.json" << true << true;
}

void SequenceFileTest::test_simplesequencelaunching()
{
    QFETCH(QString, sequenceFilePath);
    QFETCH(bool, expectedLoadingSuccess);
    QFETCH(bool, expectedSuccess);

    QFile *sequenceFile = new QFile(sequenceFilePath);

    QVERIFY(sequenceFile->exists());

    SequenceFile seqFile;

    QSharedPointer<NodesSequence> nodesSequence;
    loadSequenceFile(sequenceFile, seqFile, nodesSequence);

    QCOMPARE(!nodesSequence.isNull(), expectedLoadingSuccess);

    // *********************** Usefull to see what has been loaded ************************** //
    JsonArray sequenceArray;
    QVERIFY(NodesToJsonParser::sequenceFileToJsonArray(seqFile, sequenceArray));

    QByteArray sequenceParsedStr;

    QVERIFY(JsonStringify::toCompactString(sequenceArray, sequenceParsedStr));

    qInfo() << "Sequence parsed: " << sequenceParsedStr.constData();
    // ************************************************************************************* //

    QVERIFY(!nodesSequence.isNull());

    bool receivedFinished = false;
    bool success = false;

    connect(nodesSequence.data(), &NodesSequence::finished,
            this,
            [&receivedFinished, &success](bool successFinish) {
                success = successFinish;
                receivedFinished =  true;
            });

    QVERIFY(nodesSequence->startSequence());

    QVERIFY(WaitHelper::pseudoWait(receivedFinished, 5000));

    QCOMPARE(success, expectedSuccess);
}

void SequenceFileTest::test_stoppingsequence_data()
{
    // Sequence launching
    QTest::addColumn<QString>("sequenceFilePath");
    QTest::addColumn<bool>("expectedLoadingSuccess");
    QTest::addColumn<bool>("expectedRunningSuccess");

    QTest::newRow("Test launching sequence with a long wait to be able to stop sequence")
            << ":/launch-sequence-long-wait.json" << true << false;
}

void SequenceFileTest::test_stoppingsequence()
{
    QFETCH(QString, sequenceFilePath);
    QFETCH(bool, expectedLoadingSuccess);
    QFETCH(bool, expectedRunningSuccess);

    QFile *sequenceFile = new QFile(sequenceFilePath);

    QVERIFY(sequenceFile->exists());

    SequenceFile seqFile;
    QSharedPointer<NodesSequence> nodesSequence;
    loadSequenceFile(sequenceFile, seqFile, nodesSequence);

    QCOMPARE(!nodesSequence.isNull(), expectedLoadingSuccess);

    // *********************** Usefull to see what has been loaded ************************** //
    JsonArray sequenceArray;
    QVERIFY(NodesToJsonParser::sequenceFileToJsonArray(seqFile, sequenceArray));

    QByteArray sequenceParsedStr;

    QVERIFY(JsonStringify::toCompactString(sequenceArray, sequenceParsedStr));

    qInfo() << "Sequence parsed: " << sequenceParsedStr.constData();
    // ************************************************************************************* //

    QVERIFY(!nodesSequence.isNull());

    bool receivedFinished = false;
    bool success = false;

    connect(nodesSequence.data(), &NodesSequence::finished,
            this,
            [&receivedFinished, &success](bool successFinish) {
                success = successFinish;
                receivedFinished =  true;
            });

    QVERIFY(nodesSequence->startSequence());

    QTest::qWait(100);

    QVERIFY(nodesSequence->stopSequence(StopAskedType::EndOfProcess));

    QVERIFY(WaitHelper::pseudoWait(receivedFinished, 5000));

    QCOMPARE(success, expectedRunningSuccess);
}

void SequenceFileTest::test_pausingsequence_data()
{
    // Sequence launching
    QTest::addColumn<QString>("sequenceFilePath");
    QTest::addColumn<QString>("nodeNameToWaitExitSignal");
    QTest::addColumn<bool>("expectedLoadingSuccess");
    QTest::addColumn<bool>("expectedRunningSuccess");

    QTest::newRow("Test launching sequence with several short waits a to be able to pause sequence")
            << ":/launch-seq-with-short-waits.json" << "sequenceModuleNode2" << true << true;
}

void SequenceFileTest::test_pausingsequence()
{
    QFETCH(QString, sequenceFilePath);
    QFETCH(QString, nodeNameToWaitExitSignal);
    QFETCH(bool, expectedLoadingSuccess);
    QFETCH(bool, expectedRunningSuccess);

    QFile *sequenceFile = new QFile(sequenceFilePath);

    QVERIFY(sequenceFile->exists());

    SequenceFile seqFile;

    QSharedPointer<NodesSequence> nodesSequence;
    loadSequenceFile(sequenceFile, seqFile, nodesSequence);

    QCOMPARE(!nodesSequence.isNull(), expectedLoadingSuccess);

    // *********************** Usefull to see what has been loaded ************************** //
    JsonArray sequenceArray;
    QVERIFY(NodesToJsonParser::sequenceFileToJsonArray(seqFile, sequenceArray));

    QByteArray sequenceParsedStr;

    QVERIFY(JsonStringify::toCompactString(sequenceArray, sequenceParsedStr));

    qInfo() << "Sequence parsed: " << sequenceParsedStr.constData();
    // ************************************************************************************* //

    QVERIFY(!nodesSequence.isNull());

    bool receivedFinished = false;
    bool success = false;

    connect(nodesSequence.data(), &NodesSequence::finished,
            this,
            [&receivedFinished, &success](bool successFinish) {
                success = successFinish;
                receivedFinished =  true;
            });

    QSharedPointer<ASequenceNode> seqModule = nodesSequence->getSequenceNode(
                                                                        nodeNameToWaitExitSignal);

    QVERIFY(!seqModule.isNull());

    QMutex mutex;

    bool countActivated = false;
    int signalReceivedNb = 0;

    connect(seqModule.data(),
            &ASequenceNode::exit,
            this,
            [&]()
            {
                if(!countActivated)
                {
                    return;
                }

                signalReceivedNb++;

                // If pause is called just before the end of the sequence module, one exit can fire
                // and so to prevent that we had a tolerance of 1 signal before returning an error

                if(signalReceivedNb >= 2)
                {
                    mutex.unlock();
                }
            });

    QVERIFY(mutex.tryLock());

    QVERIFY(nodesSequence->startSequence());

    QTest::qWait(500);

    qInfo() << "Set pause to true";
    QVERIFY(nodesSequence->managePauseAndResume(true));

    countActivated = true;

    if(mutex.tryLock(2000))
    {
        mutex.unlock();
    }

    countActivated = false;

    QVERIFY(nodesSequence->managePauseAndResume(false));
    qInfo() << "Set pause to false";

    QVERIFY(signalReceivedNb < 2);

    QVERIFY(WaitHelper::pseudoWait(receivedFinished, 20000));

    QCOMPARE(success, expectedRunningSuccess);
}

void SequenceFileTest::test_stoppausingsequence_data()
{
    // Sequence launching
    QTest::addColumn<QString>("sequenceFilePath");
    QTest::addColumn<QString>("nodeNameToWaitExitSignal");
    QTest::addColumn<bool>("expectedLoadingSuccess");
    QTest::addColumn<bool>("expectedRunningSuccess");

    QTest::newRow("Pause a sequence and then stop it")
            << ":/launch-seq-with-short-waits.json" << "sequenceModuleNode2" << true << false;
}

void SequenceFileTest::test_stoppausingsequence()
{
    QFETCH(QString, sequenceFilePath);
    QFETCH(QString, nodeNameToWaitExitSignal);
    QFETCH(bool, expectedLoadingSuccess);
    QFETCH(bool, expectedRunningSuccess);

    QFile *sequenceFile = new QFile(sequenceFilePath);

    QVERIFY(sequenceFile->exists());

    SequenceFile seqFile;

    QSharedPointer<NodesSequence> nodesSequence;
    loadSequenceFile(sequenceFile, seqFile, nodesSequence);

    QCOMPARE(!nodesSequence.isNull(), expectedLoadingSuccess);

    // *********************** Usefull to see what has been loaded ************************** //
    JsonArray sequenceArray;
    QVERIFY(NodesToJsonParser::sequenceFileToJsonArray(seqFile, sequenceArray));

    QByteArray sequenceParsedStr;

    QVERIFY(JsonStringify::toCompactString(sequenceArray, sequenceParsedStr));

    qInfo() << "Sequence parsed: " << sequenceParsedStr.constData();
    // ************************************************************************************* //

    QVERIFY(!nodesSequence.isNull());

    bool receivedFinished = false;
    bool success = false;

    connect(nodesSequence.data(), &NodesSequence::finished,
            this,
            [&receivedFinished, &success](bool successFinish) {
                success = successFinish;
                receivedFinished =  true;
            });

    QSharedPointer<ASequenceNode> seqModule = nodesSequence->getSequenceNode(
                                                                        nodeNameToWaitExitSignal);

    QVERIFY(!seqModule.isNull());

    QVERIFY(nodesSequence->startSequence());

    QTest::qWait(500);

    qInfo() << "Set pause to true";
    QVERIFY(nodesSequence->managePauseAndResume(true));

    QTest::qWait(500);

    qInfo() << "Stop sequence";
    QVERIFY(nodesSequence->stopSequence(StopAskedType::EndOfProcess));

    QVERIFY(WaitHelper::pseudoWait(receivedFinished, 5000));

    QCOMPARE(success, expectedRunningSuccess);
}

void SequenceFileTest::loadSequenceFile(QFile *tmpSequenceFile,
                                        SequenceFile &sequenceFile,
                                        QSharedPointer<NodesSequence> &nodeSequenceLoaded)
{
    QVERIFY(sequenceFile.initAndSetSequenceFile(tmpSequenceFile));

    QVERIFY(sequenceFile.loadSequenceDataAndLaunchThreads(1));

    QVERIFY(WaitHelper::pseudoWait(sequenceFile, &SequenceFile::areAllThreadsRunning, 5000));

    QVector<SequenceThread *> sequenceThreads = sequenceFile.accessSequenceThreads();

    QCOMPARE(sequenceThreads.length(), 1);

    SequenceThread *sequenceThread = sequenceThreads.first();

    QVERIFY(sequenceThread->setSequenceData(sequenceFile.accessJsonArray()));
    if(sequenceThread->cleanAndLoadNodesSequence())
    {
        nodeSequenceLoaded = sequenceThread->getLinkedNodesSequence().constCast<NodesSequence>();
        return;
    }

    nodeSequenceLoaded = QSharedPointer<NodesSequence>();
}

bool SequenceFileTest::loadJsonArray(const QString &docPath, JsonArray &jsonArray)
{
    QFile jsonFile(docPath);

    if(!jsonFile.exists())
    {
        qWarning() << "The file: " << docPath << ", doesn't exist";
        return false;
    }

    QJsonArray qtJsonArray;
    QJsonDocument document;

    if(!JsonHelper::loadJsonDevice(jsonFile, document))
    {
        qWarning() << "A problem occured when trying to parse to Json, the file: " << docPath
                   << ", given";
        return false;
    }

    RETURN_IF_FALSE(JsonHelper::getArray(document, qtJsonArray));

    jsonArray = JsonConverter::convertJsonArray(qtJsonArray);

    return true;
}

bool SequenceFileTest::createTemporaryFile(const JsonArray &data, QTemporaryFile &tempFile)
{
    if(!tempFile.open())
    {
        qWarning() << "The temporary file: " << tempFile.fileTemplate();
        return false;
    }

    QByteArray strData;

    if(!JsonStringify::toCompactString(data, strData))
    {
        qWarning() << "The stringifing of the sequence array failed, to add it in a temporary file";
        return false;
    }

    qInfo() << "Data to write: " << strData.constData();

    if(tempFile.write(strData) == -1)
    {
        qWarning() << "An error occured when writting data to the temporary file: "
                   << tempFile.fileTemplate();
        return false;
    }

    tempFile.close();

    return true;
}

bool SequenceFileTest::setAttribute(const QString &attributeName,
                                    const QVariant &attributeValue,
                                    JsonObject &object)
{
    if(object.contains(attributeName))
    {
        object[attributeName] = attributeValue;
    }
    else
    {
        object.insert(attributeName, attributeValue);
    }

    return true;
}

bool SequenceFileTest::removeAttribute(const QString &attributeName, JsonObject &object)
{
    object.remove(attributeName);

    return true;
}

bool SequenceFileTest::setAttributes(const QString &distinctiveObjectAttributeName,
                                     const QVariant &distinctiveObjectAttributeValue,
                                     const JsonObject &elementsToSet,
                                     JsonArray &array)
{
    JsonArray::iterator iter;
    RETURN_IF_FALSE(findObject(distinctiveObjectAttributeName,
                               distinctiveObjectAttributeValue,
                               array,
                               iter));

    JsonObject object = iter->value<JsonObject>();

    JsonObject::const_iterator iterToSet = elementsToSet.cbegin();

    for(; iterToSet != elementsToSet.cend(); ++iterToSet)
    {
        if(!setAttribute(iterToSet.key(), iterToSet.value(), object))
        {
            return false;
        }
    }

    *iter = object;

    return true;
}

bool SequenceFileTest::removeAttributes(const QString &distinctiveObjectAttributeName,
                                        const QVariant &distinctiveObjectAttributeValue,
                                        const QVector<QString> &attributesNameToRemove,
                                        JsonArray &array)
{
    JsonArray::iterator iter;
    RETURN_IF_FALSE(findObject(distinctiveObjectAttributeName,
                               distinctiveObjectAttributeValue,
                               array,
                               iter));

    JsonObject object = iter->value<JsonObject>();

    for(const QString &attributeNameToRemove : attributesNameToRemove)
    {
        if(!removeAttribute(attributeNameToRemove, object))
        {
            return false;
        }
    }

    *iter = object;

    return true;
}

bool SequenceFileTest::findObject(const QString &distinctiveObjectAttributeName,
                                  const QVariant &distinctiveObjectAttributeValue,
                                  JsonArray &array,
                                  JsonArray::iterator &objectIter)
{
    JsonArray::iterator iter = array.begin();

    for(; iter != array.end(); ++iter)
    {
        if(!iter->canConvert<JsonObject>())
        {
            qWarning() << "The element: " << *iter << ", is not a JsonObject";
            return false;
        }

        JsonObject object = iter->value<JsonObject>();

        if(!object.contains(distinctiveObjectAttributeName))
        {
            qWarning() << "The object: " << object << ", has no attribute named: "
                       << distinctiveObjectAttributeName;
            return false;
        }

        const QVariant &disctinctiveValue = object[distinctiveObjectAttributeName];

        if(disctinctiveValue != distinctiveObjectAttributeValue)
        {
            continue;
        }

        objectIter = iter;

        return true;
    }

    qWarning() << "No object has been found with the attribute: \""
               << distinctiveObjectAttributeName << "\": " << distinctiveObjectAttributeValue;
    return false;
}

QTEST_MAIN(SequenceFileTest)
