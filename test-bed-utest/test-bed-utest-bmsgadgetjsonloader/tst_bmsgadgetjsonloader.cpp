// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "tst_bmsgadgetjsonloader.hpp"

#include "test-bed-lib/jsonparsers/bmsgadgetjsonloader.hpp"
#include "testbedcore/testbedglobal.hpp"

#include "collectionutility/qvariantlisthelper.hpp"
#include "test-bed-lib/topology/topologyfile.hpp"
#include "test-bed-lib/topology/topologyfilebindings.hpp"
#include "test-bed-lib/topology/topologyfileinstances.hpp"
#include "test-bed-lib/topology/topologyfilelibrary.hpp"
#include "test-bed-lib/topology/topologyfileroutes.hpp"

#include <QCoreApplication>
#include <QJsonDocument>
#include <QMetaObject>
#include <QMetaType>
#include <QtTest>

Q_DECLARE_METATYPE(QMetaObject *)
Q_DECLARE_METATYPE(const QMetaObject *)

BmsGadgetJsonLoaderTest::BmsGadgetJsonLoaderTest()
{
}

BmsGadgetJsonLoaderTest::~BmsGadgetJsonLoaderTest()
{
}

void BmsGadgetJsonLoaderTest::initTestCase()
{
    /* Registering meta-types for those types to be embeddable in QVariants,
     * and comparable through QVariant. We only register head types since they
     * already registers their inner types (ex: TopologyFileComponent registers
     * TopologyFileComponentPinGroup), and since registering the same type twice may
     * lead to Qt warnings ("Comparators already registered for type xxx") which
     * is not very beautiful.
     * Note: Route actually uses BindingPin hence BindingPin is meta-registered twice,
     * which lead to a remaining warning.
     * Note: TopologyFile actually contains the four heads (library/instances/bindings/routes)
     * and register them all, hence we only need to register this top-head for all TopologyFile*.
     */
    TopologyFile::RegisterMetaTypes();
}

void BmsGadgetJsonLoaderTest::cleanupTestCase()
{
}

void BmsGadgetJsonLoaderTest::test_createqobject_data()
{
    QTest::addColumn<QString>("json");
    QTest::addColumn<const QMetaObject *>("defaultMetaclass");
    QTest::addColumn<bool>("expectedSuccess");
    QTest::addColumn<BmsGadget *>("expectedObject");

    QTest::newRow("TopologyFile")
        << STRINGIFY({
               "name" : "My simple topology",
               "revision" : 17,

               "librariesFilesPath" : ["topology/001-library1.tbtl"],
               "instancesFilesPath" : ["topology/002-instances1.tbti"],
               "bindingsFilesPath" : ["topology/003-bindings1.tbtb"],
               "routesFilesPath" : ["topology/004-routes1.tbtr"]
           })
        << &TopologyFile::staticMetaObject
        << true
        << static_cast<BmsGadget *>(new TopologyFile("My simple topology",
                                                     17,
                                                     {"topology/001-library1.tbtl"},
                                                     {"topology/002-instances1.tbti"},
                                                     {"topology/003-bindings1.tbtb"},
                                                     {"topology/004-routes1.tbtr"}));

    QTest::newRow("TopologyFileComponentPin")
        << STRINGIFY({"label" : "slot0", "kind" : "agilent-card-slot", "direction" : "input"})
        << &TopologyFileComponentPin::staticMetaObject
        << true
        << static_cast<BmsGadget *>(new TopologyFileComponentPin(
               "slot0", "agilent-card-slot", "input"));

    QTest::newRow("TopologyFileComponentPinGroup")
        << STRINGIFY({
               "groupName" : "slots",
               "elements" : [
                   {"label" : "RS232", "kind" : "stream/rs232", "direction" : "inout"},
                   {"label" : "GP-IB", "kind" : "stream/ieee488", "direction" : "inout"}
               ]
           })
        << &TopologyFileComponentPinGroup::staticMetaObject
        << true
        << static_cast<BmsGadget *>(new TopologyFileComponentPinGroup(
               "slots",
               {TopologyFileComponentPin("RS232", "stream/rs232", "inout"),
                TopologyFileComponentPin("GP-IB", "stream/ieee488", "inout")}));

    QTest::newRow("TopologyFileComponent")
        << STRINGIFY({
               "name" : "34970A",
               "description" : "Agilent DMM mainframe",
               "type" : "basic",
               "category" : "Agilent",
               "pins" : [
                   {
                       "groupName" : "CTRL",
                       "elements" : [
                           {"label" : "RS232", "kind" : "stream/rs232", "direction" : "inout"},
                           {"label" : "GP-IB", "kind" : "stream/ieee488", "direction" : "inout"}
                       ]
                   },
                   {
                       "groupName" : "slots",
                       "elements" : [
                           {"label" : "slot0", "kind" : "agilent-card-slot", "direction" : "input"},
                           {"label" : "slot1", "kind" : "agilent-card-slot", "direction" : "input"},
                           {"label" : "slot2", "kind" : "agilent-card-slot", "direction" : "input"}
                       ]
                   }
               ],
               "params" : []
           })
        << &TopologyFileComponent::staticMetaObject
        << true
        << static_cast<BmsGadget *>(new TopologyFileComponent(
               "34970A",
               "Agilent DMM mainframe",
               "basic",
               "Agilent",
               {TopologyFileComponentPinGroup("CTRL",
                                              {TopologyFileComponentPin("RS232", "stream/rs232", "inout"),
                                               TopologyFileComponentPin("GP-IB", "stream/ieee488", "inout")}),
                TopologyFileComponentPinGroup("slots",
                                              {TopologyFileComponentPin("slot0", "agilent-card-slot", "input"),
                                               TopologyFileComponentPin("slot1", "agilent-card-slot", "input"),
                                               TopologyFileComponentPin("slot2", "agilent-card-slot", "input")})}));

    QTest::newRow("TopologyFileInstances")
        << STRINGIFY({"instances" : [
               {"name" : "myk2k", "component" : "Keithley2000", "params" : []},
               {"name" : "myfakebox", "component" : "Fakebox", "params" : [
                    {"key" : "strparam", "value" : "text"},
                    {"key" : "intparam", "value" : 5},
                    {"key" : "boolparam", "value" : true},
                    {"key" : "boolparam2", "value" : false},
                    {"key" : "objparam", "value" : {"metatype" : "TopologyFileComponentPin",
                                                    "label" : "slot0",
                                                    "kind" : "agilent-card-slot",
                                                    "direction" : "input"}}
                ]}
           ]})
        << &TopologyFileInstances::staticMetaObject << true << static_cast<BmsGadget *>(new TopologyFileInstances({TopologyFileInstance("myk2k", "Keithley2000", {}), TopologyFileInstance("myfakebox", "Fakebox", {TopologyFileInstanceParam("strparam", "text"), TopologyFileInstanceParam("intparam", 5), TopologyFileInstanceParam("boolparam", true), TopologyFileInstanceParam("boolparam2", false), TopologyFileInstanceParam("objparam", QVariant::fromValue(TopologyFileComponentPin("slot0", "agilent-card-slot", "input")))})}));

    QTest::newRow("TopologyFileBindingPin")
        << STRINGIFY({
               "instanceName" : "foo",
               "PinName" : "bar"
           })
        << &TopologyFileBindingPin::staticMetaObject
        << true
        << static_cast<BmsGadget *>(new TopologyFileBindingPin("foo", "bar"));

    QTest::newRow("TopologyFileBinding")
        << STRINGIFY({"pins" : [ {"instanceName" : "foo", "PinName" : "LED1"},
                                 {"instanceName" : "bar", "PinName" : "V+"},
                                 {"instanceName" : "baz", "PinName" : "GND"} ]})
        << &TopologyFileBinding::staticMetaObject
        << true
        << static_cast<BmsGadget *>(new TopologyFileBinding({{"foo", "LED1"},
                                                             {"bar", "V+"},
                                                             {"baz", "GND"}}));

    QTest::newRow("TopologyFileBindings")
        << STRINGIFY({"bindings" :
                          [ {"pins" : [ {"instanceName" : "foo", "PinName" : "LED1"},
                                        {"instanceName" : "bar", "PinName" : "V+"},
                                        {"instanceName" : "baz", "PinName" : "GND"} ]},
                            {"pins" : [ {"instanceName" : "foo2", "PinName" : "LED2"},
                                        {"instanceName" : "bar2", "PinName" : "V++"},
                                        {"instanceName" : "baz2", "PinName" : "GND2"} ]} ]})
        << &TopologyFileBindings::staticMetaObject
        << true
        << static_cast<BmsGadget *>(new TopologyFileBindings({TopologyFileBinding({{"foo", "LED1"},
                                                                                   {"bar", "V+"},
                                                                                   {"baz", "GND"}}),
                                                              TopologyFileBinding({{"foo2", "LED2"},
                                                                                   {"bar2", "V++"},
                                                                                   {"baz2", "GND2"}})}));

    QTest::newRow("TopologyFileRoutes")
        << STRINGIFY({"routes" :
                          [ {"fromPin" : {"instanceName" : "foo", "pinName" : "LED1"},
                             "toPin" : {"instanceName" : "bar", "pinName" : "LED2"},
                             "path" : [ {"instance" : "my-matrix", "fromPin" : "COL7", "toPin" : "ROW9"} ]} ]})
        << &TopologyFileRoutes::staticMetaObject
        << true
        << static_cast<BmsGadget *>(new TopologyFileRoutes({TopologyFileRoute({"foo", "LED1"}, /* from */
                                                                              {"bar", "LED2"}, /* to */
                                                                              {                /* hops */
                                                                               TopologyFileRoutePoint("my-matrix",
                                                                                                      "COL7",
                                                                                                      "ROW9")})}));
}

void BmsGadgetJsonLoaderTest::test_createqobject()
{
    QFETCH(QString, json);
    QFETCH(const QMetaObject *, defaultMetaclass);
    QFETCH(bool, expectedSuccess);
    QFETCH(BmsGadget *, expectedObject);
    const QMetaObject *metaclass = nullptr;

    /* Re-generate a QObject-based instance based on JSON data */
    BmsGadgetJsonLoader loader;
    BmsGadget *generatedObject = nullptr;
    const bool createObjectCallSuccess = loader.createObject(
        QJsonDocument::fromJson(json.toLatin1()).object(),
        &generatedObject,
        defaultMetaclass,
        &metaclass);

    /* Test boolean result and pointers nullity */
    QCOMPARE(createObjectCallSuccess, expectedSuccess);
    QCOMPARE(!!generatedObject, !!expectedObject);

    /* Invoke genetated object operator== */
    int metaComparisonValue = -1;
    bool metaComparisonSuccess = false;
    metaComparisonSuccess = QMetaType::compare(generatedObject,
                                               expectedObject,
                                               QMetaType::type(metaclass->className()),
                                               &metaComparisonValue);
    QVERIFY(metaComparisonSuccess == true);
    QCOMPARE(metaComparisonValue, 0);

    /* Manual verification of generated object content */
    if (generatedObject && expectedObject)
    {
        for (int i = 0; i < metaclass->propertyCount(); ++i)
        {
            const QMetaProperty property = metaclass->property(i);
            QVariant generatedPropertyValue;
            QVariant expectedPropertyValue;

            generatedPropertyValue = property.readOnGadget(generatedObject);
            expectedPropertyValue = property.readOnGadget(expectedObject);

            QVERIFY2(generatedPropertyValue.isValid() == expectedPropertyValue.isValid(),
                     property.name());
            QVERIFY2(generatedPropertyValue == expectedPropertyValue,
                     property.name());
        }
    }
}

QTEST_MAIN(BmsGadgetJsonLoaderTest)
