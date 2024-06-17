// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "tst_variable_operations_plugin.hpp"

#include <QtTest>

#include <QCoreApplication>
#include <QVariantHash>

#include "jsonutility/jsoncomparator.hpp"
#include "jsonutility/jsonhelper.hpp"
#include "abstractpluginelements/defaultbankglobalmanager.hpp"
#include "variable-operations-plugin/operations/frommultirefnumberarithmeticoperator.hpp"
#include "variable-operations-plugin/operations/fromrefboolassignmentoperator.hpp"
#include "variable-operations-plugin/operations/fromrefnumberarithmeticoperator.hpp"
#include "variable-operations-plugin/operations/fromrefnumberassignmentoperator.hpp"
#include "variable-operations-plugin/operations/fromrefstringassignmentoperator.hpp"
#include "variable-operations-plugin/operations/valueboolassignmentoperator.hpp"
#include "variable-operations-plugin/operations/valuenumberassignmentoperator.hpp"
#include "variable-operations-plugin/operations/valuestringassignmentoperator.hpp"


VariableOperationsPlugin::VariableOperationsPlugin()
{
}

VariableOperationsPlugin::~VariableOperationsPlugin()
{
}

void VariableOperationsPlugin::initTestCase()
{
    JsonHelper::RegisterMetaTypes();
    DefaultBankGlobalManager::Create(_interface);
    Number::RegisterMetaTypes();
}

void VariableOperationsPlugin::cleanupTestCase()
{
}

void VariableOperationsPlugin::test_valueBoolAssignmentOperator_data()
{
    QTest::addColumn<QString>("moduleName");
    QTest::addColumn<JsonArray>("parameters");
    QTest::addColumn<JsonArray>("inputValue");
    QTest::addColumn<JsonArray>("outputFormat");
    QTest::addColumn<JsonArray>("outputExpected");
    QTest::addColumn<bool>("setModuleParametersResult");
    QTest::addColumn<bool>("launchModuleResult");

    QTest::newRow("Assign a boolean with true value")
            << "valueboolassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "assignmentModifier" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, true }
                         }))
                     })
            << JsonArray()
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                       { JsonParsersConstants::Generic::NameAttribute,
                                         "outputPin" },
                                       { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                         TypeManaged::toString(CommonJsonParser::
                                                                           getManagedType<bool>()) }
                                   }))
                               }))
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "outputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, true }
                         }))
                     })
            << true
            << true;

    QTest::newRow("Assign a boolean with false value")
            << "valueboolassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "assignmentModifier" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, false }
                         }))
                     })
            << JsonArray()
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                       { JsonParsersConstants::Generic::NameAttribute,
                                         "outputPin" },
                                       { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                         TypeManaged::toString(CommonJsonParser::
                                                                           getManagedType<bool>()) }
                                   }))
                               }))
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "outputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, false }
                         }))
                     })
            << true
            << true;

    QTest::newRow("Assign a bool with a string 'true' ")
            << "valueboolassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "assignmentModifier" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, "true" }
                         }))
                     })
            << JsonArray()
            << JsonArray()
            << JsonArray()
            << false
            << true;

    QTest::newRow("Assign a bool with a string 'false' ")
            << "valueboolassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "assignmentModifier" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, "false" }
                         }))
                     })
            << JsonArray()
            << JsonArray()
            << JsonArray()
            << false
            << true;

    QTest::newRow("Assign a bool with a int 1 ")
            << "valueboolassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "assignmentModifier" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, 1 }
                         }))
                     })
            << JsonArray()
            << JsonArray()
            << JsonArray()
            << false
            << true;

    QTest::newRow("Assign a bool with a int 0 ")
            << "valueboolassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "assignmentModifier" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, 0 }
                         }))
                     })
            << JsonArray()
            << JsonArray()
            << JsonArray()
            << false
            << true;

    QTest::newRow("Assign a bool with a int -10 ")
            << "valueboolassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "assignmentModifier" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, -10 }
                         }))
                     })
            << JsonArray()
            << JsonArray()
            << JsonArray()
            << false
            << true;

    QTest::newRow("Assign a bool with a string : 'Test' ")
            << "valueboolassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "assignmentModifier" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, "Test" }
                         }))
                     })
            << JsonArray()
            << JsonArray()
            << JsonArray()
            << false
            << true;
}

void VariableOperationsPlugin::test_valueBoolAssignmentOperator()
{
    ValueBoolAssignmentOperator boolAssign;
    testOperator(&boolAssign);
}

void VariableOperationsPlugin::test_valueNumberAssignmentOperator_data()
{
    QTest::addColumn<QString>("moduleName");
    QTest::addColumn<JsonArray>("parameters");
    QTest::addColumn<JsonArray>("inputValue");
    QTest::addColumn<JsonArray>("outputFormat");
    QTest::addColumn<JsonArray>("outputExpected");
    QTest::addColumn<bool>("setModuleParametersResult");
    QTest::addColumn<bool>("launchModuleResult");

    QTest::newRow("Assign a Number with Number 5")
            << "valuenumberassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "assignmentModifier" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(5))}
                         }))
                     })
            << JsonArray()
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute, "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                         getManagedType<Number>()) }
                                   }))
                               }))
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "outputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(5)) }
                         }))
                     })
            << true
            << true;

    QTest::newRow("Assign a Number with true bool")
            << "valuenumberassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "assignmentModifier" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, true }
                         }))
                     })
            << JsonArray()
            << JsonArray()
            << JsonArray()
            << false
            << true;

    QTest::newRow("Assign a Number with false bool")
            << "valuenumberassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "assignmentModifier" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, false }
                         }))
                     })
            << JsonArray()
            << JsonArray()
            << JsonArray()
            << false
            << true;

    QTest::newRow("Assign a Number with a string 'true' ")
            << "valuenumberassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "assignmentModifier" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, "true" }
                         }))
                     })
            << JsonArray()
            << JsonArray()
            << JsonArray()
            << false
            << true;

    QTest::newRow("Assign a Number with a int 0 ")
            << "valuenumberassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "assignmentModifier" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, 0 }
                         }))
                     })
            << JsonArray()
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute, "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                         getManagedType<Number>()) }
                                   }))
                               }))
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "outputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(0)) }
                         }))
                     })
            << true
            << true;

    QTest::newRow("Assign a Number with a int -10 ")
            << "valuenumberassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "assignmentModifier" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, -10 }
                         }))
                     })
            << JsonArray()
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute, "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                         getManagedType<Number>()) }
                                   }))
                               }))
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "outputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number::fromInt8(-10)) }
                         }))
                     })
            << true
            << true;

    QTest::newRow("Assign a Number with a string : '1' ")
            << "valuenumberassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "assignmentModifier" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, "1" }
                         }))
                     })
            << JsonArray()
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute, "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                         getManagedType<Number>()) }
                                   }))
                               }))
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "outputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number("1")) }
                         }))
                     })
            << true
            << true;

    QTest::newRow("Assign a Number with a wrong number")
            << "valuenumberassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "assignmentModifier" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number("wrong number")) }
                         }))
                     })
            << JsonArray()
            << JsonArray()
            << JsonArray()
            << false
            << true;
}

void VariableOperationsPlugin::test_valueNumberAssignmentOperator()
{
    ValueNumberAssignmentOperator numberAssign ;
    testOperator(&numberAssign);
}

void VariableOperationsPlugin::test_valueStringAssignmentOperator_data()
{
    QTest::addColumn<QString>("moduleName");
    QTest::addColumn<JsonArray>("parameters");
    QTest::addColumn<JsonArray>("inputValue");
    QTest::addColumn<JsonArray>("outputFormat");
    QTest::addColumn<JsonArray>("outputExpected");
    QTest::addColumn<bool>("setModuleParametersResult");
    QTest::addColumn<bool>("launchModuleResult");

    QTest::newRow("Assign String 'String test'")
            << "valuestringassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "assignmentModifier" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(QString("String test")) }
                         }))
                     })
            << JsonArray()
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute, "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                        getManagedType<QString>()) }
                                   }))
                               }))
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "outputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(QString("String test")) }
                         }))
                     })
            << true
            << true;

    QTest::newRow("Assign String with int 2")
            << "valuestringassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "assignmentModifier" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, 2 }
                         }))
                     })
            << JsonArray()
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute, "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                        getManagedType<QString>()) }
                                   }))
                               }))
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "outputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, "2" }
                         }))
                     })
            << true
            << true;

    QTest::newRow("Assign String with bool true")
            << "valuestringassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "assignmentModifier" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, true }
                         }))
                     })
            << JsonArray()
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute, "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                        getManagedType<QString>()) }
                                   }))
                               }))
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "outputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, "true" }
                         }))
                     })
            << true
            << true;
}

void VariableOperationsPlugin::test_valueStringAssignmentOperator()
{
    ValueStringAssignmentOperator stringAssign;
    testOperator(&stringAssign);
}

void VariableOperationsPlugin::test_fromRefBoolAssignmentOperator_data()
{
    QTest::addColumn<QString>("moduleName");
    QTest::addColumn<JsonArray>("parameters");
    QTest::addColumn<JsonArray>("inputValue");
    QTest::addColumn<JsonArray>("outputFormat");
    QTest::addColumn<JsonArray>("outputExpected");
    QTest::addColumn<bool>("setModuleParametersResult");
    QTest::addColumn<bool>("launchModuleResult");

    QTest::newRow("Assign same bool from ref")
            << "fromrefboolassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "assignmentModifier" },
                            { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ "sameKey" }))}
                         }))
                     })
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, true}
                         }))
                     })
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute,
                                        "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                           getManagedType<bool>()) }
                                   }))
                               }))
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "outputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, true }
                         }))
                     })
            << true
            << true;

    QTest::newRow("Assign !bool from ref")
            << "fromrefboolassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "assignmentModifier" },
                            { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ "notKey" }))}
                         }))
                     })
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, true}
                         }))
                     })
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute,
                                        "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                           getManagedType<bool>()) }
                                   }))
                               }))
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "outputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, false }
                         }))
                     })
            << true
            << true;

    QTest::newRow("Assign string 'true' from ref")
            << "fromrefboolassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "assignmentModifier" },
                            { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ "notKey" }))}
                         }))
                     })
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, "true"}
                         }))
                     })
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute,
                                        "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                           getManagedType<bool>()) }
                                   }))
                               }))
            << JsonArray()
            << true
            << false;

    QTest::newRow("Assign string 'false' from ref")
            << "fromrefboolassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "assignmentModifier" },
                            { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ "notKey" }))}
                         }))
                     })
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, "false"}
                         }))
                     })
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute,
                                        "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                           getManagedType<bool>()) }
                                   }))
                               }))
            << JsonArray()
            << true
            << false;

    QTest::newRow("Assign int 0 from ref")
            << "fromrefboolassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "assignmentModifier" },
                            { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ "notKey" }))}
                         }))
                     })
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, 0}
                         }))
                     })
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute,
                                        "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                           getManagedType<bool>()) }
                                   }))
                               }))
            << JsonArray()
            << true
            << false;

    QTest::newRow("Assign int 1 from ref")
            << "fromrefboolassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "assignmentModifier" },
                            { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ "notKey" }))}
                         }))
                     })
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, 1}
                         }))
                     })
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute,
                                        "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                           getManagedType<bool>()) }
                                   }))
                               }))
            << JsonArray()
            << true
            << false;

    QTest::newRow("Assign int -10 from ref")
            << "fromrefboolassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "assignmentModifier" },
                            { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ "notKey" }))}
                         }))
                     })
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, -10}
                         }))
                     })
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute,
                                        "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                           getManagedType<bool>()) }
                                   }))
                               }))
            << JsonArray()
            << true
            << false;
}

void VariableOperationsPlugin::test_fromRefBoolAssignmentOperator()
{
    FromRefBoolAssignmentOperator fromRefBoolAssign;
    testOperator(&fromRefBoolAssign);
}

void VariableOperationsPlugin::test_fromRefNumberAssignmentOperator_data()
{
    QTest::addColumn<QString>("moduleName");
    QTest::addColumn<JsonArray>("parameters");
    QTest::addColumn<JsonArray>("inputValue");
    QTest::addColumn<JsonArray>("outputFormat");
    QTest::addColumn<JsonArray>("outputExpected");
    QTest::addColumn<bool>("setModuleParametersResult");
    QTest::addColumn<bool>("launchModuleResult");

    QTest::newRow("Assign same number from ref")
            << "fromrefnumberassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "simpleOperation" },
                            { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ "sameKey" }))}
                         }))
                     })
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(5))}
                         }))
                     })
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute,
                                        "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                         getManagedType<Number>()) }
                                   }))
                               }))
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "outputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(5)) }
                         }))
                     })
            << true
            << true;

    QTest::newRow("Assign absolute positive number from ref")
            << "fromrefnumberassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "simpleOperation" },
                            { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ "absolute" }))}
                         }))
                     })
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(5))}
                         }))
                     })
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute,
                                        "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                         getManagedType<Number>()) }
                                   }))
                               }))
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "outputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(5)) }
                         }))
                     })
            << true
            << true;

    QTest::newRow("Assign absolute negative number from ref")
            << "fromrefnumberassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "simpleOperation" },
                            { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ "absolute" }))}
                         }))
                     })
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(5,0,false))}
                         }))
                     })
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute,
                                        "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                         getManagedType<Number>()) }
                                   }))
                               }))
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "outputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(5)) }
                         }))
                     })
            << true
            << true;

    QTest::newRow("Assign opposite positive number from ref")
            << "fromrefnumberassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "simpleOperation" },
                            { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ "oppositeKey" }))}
                         }))
                     })
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(5))}
                         }))
                     })
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute,
                                        "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                         getManagedType<Number>()) }
                                   }))
                               }))
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "outputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number("-5")) }
                         }))
                     })
            << true
            << true;

    QTest::newRow("Assign opposite negative number from ref")
            << "fromrefnumberassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "simpleOperation" },
                            { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ "oppositeKey" }))}
                         }))
                     })
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(5,0,false))}
                         }))
                     })
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute,
                                        "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                         getManagedType<Number>()) }
                                   }))
                               }))
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "outputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(5)) }
                         }))
                     })
            << true
            << true;

    QTest::newRow("Assign a Number with true bool")
            << "fromrefnumberassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "simpleOperation" },
                            { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ "sameKey" }))}
                         }))
                     })
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, true}
                         }))
                     })
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute,
                                        "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                         getManagedType<Number>()) }
                                   }))
                               }))
            << JsonArray()
            << true
            << false;

    QTest::newRow("Assign a Number with false bool")
            << "fromrefnumberassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "simpleOperation" },
                            { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ "sameKey" }))}
                         }))
                     })
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, false}
                         }))
                     })
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute,
                                        "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                         getManagedType<Number>()) }
                                   }))
                               }))
            << JsonArray()
            << true
            << false;

    QTest::newRow("Assign a Number with a string 'true' ")
            << "fromrefnumberassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "simpleOperation" },
                            { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ "sameKey" }))}
                         }))
                     })
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, "true"}
                         }))
                     })
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute,
                                        "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                         getManagedType<Number>()) }
                                   }))
                               }))
            << JsonArray()
            << true
            << false;

    QTest::newRow("Assign a Number with a int 0 ")
            << "fromrefnumberassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "simpleOperation" },
                            { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ "sameKey" }))}
                         }))
                     })
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(0))}
                         }))
                     })
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute,
                                        "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                         getManagedType<Number>()) }
                                   }))
                               }))
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "outputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, 0 }
                         }))
                     })
            << true
            << true;

    QTest::newRow("Assign a Number with a int -10 ")
            << "fromrefnumberassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "simpleOperation" },
                            { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ "oppositeKey" }))}
                         }))
                     })
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, -10}
                         }))
                     })
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute,
                                        "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                         getManagedType<Number>()) }
                                   }))
                               }))
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "outputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number("10")) }
                         }))
                     })
            << true
            << true;

    QTest::newRow("Assign a Number with a string : '1' ")
            << "fromrefnumberassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "simpleOperation" },
                            { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ "sameKey" }))}
                         }))
                     })
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, "1"}
                         }))
                     })
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute,
                                        "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                         getManagedType<Number>()) }
                                   }))
                               }))
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "outputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, "1" }
                         }))
                     })
            << true
            << true;

    QTest::newRow("Assign a Number with a wrong number")
            << "fromrefnumberassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "simpleOperation" },
                            { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ "sameKey" }))}
                         }))
                     })
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number("douze"))}
                         }))
                     })
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute,
                                        "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                         getManagedType<Number>()) }
                                   }))
                               }))
            << JsonArray()
            << true
            << false;

    QTest::newRow("Assign number from ref wrong NameAttribute")
            << "fromrefnumberassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "simpleOperationZZ" },
                            { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ "Absolute" }))}
                         }))
                     })
            << JsonArray()
            << JsonArray()
            << JsonArray()
            << false
            << true;

    QTest::newRow("Assign number from ref wrong ValuesNameAttribute")
            << "fromrefnumberassignmentoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "simpleOperation" },
                            { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ "AbsoluteZZ" }))}
                         }))
                     })
            << JsonArray()
            << JsonArray()
            << JsonArray()
            << false
            << true;
}

void VariableOperationsPlugin::test_fromRefNumberAssignmentOperator()
{
    FromRefNumberAssignmentOperator fromRefNumberAssign;
    testOperator(&fromRefNumberAssign);
}

void VariableOperationsPlugin::test_fromRefStringAssignmentOperator_data()
{
    QTest::addColumn<QString>("moduleName");
    QTest::addColumn<JsonArray>("parameters");
    QTest::addColumn<JsonArray>("inputValue");
    QTest::addColumn<JsonArray>("outputFormat");
    QTest::addColumn<JsonArray>("outputExpected");
    QTest::addColumn<bool>("setModuleParametersResult");
    QTest::addColumn<bool>("launchModuleResult");

    QTest::newRow("Assign string from ref")
            << "fromrefstringassignmentoperator"
            << JsonArray()
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(QString("String"))}
                         }))
                     })
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute,
                                        "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                        getManagedType<QString>()) }
                                   }))
                               }))
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "outputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(QString("String")) }
                         }))
                     })
            << true
            << true;

    QTest::newRow("Assign string from ref bool")
            << "fromrefstringassignmentoperator"
            << JsonArray()
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, true}
                         }))
                     })
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute,
                                        "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                        getManagedType<QString>()) }
                                   }))
                               }))
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "outputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, "true" }
                         }))
                     })
            << true
            << true;

    QTest::newRow("Assign string from int 2")
            << "fromrefstringassignmentoperator"
            << JsonArray()
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, 2}
                         }))
                     })
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute,
                                        "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                        getManagedType<QString>()) }
                                   }))
                               }))
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "outputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, "2" }
                         }))
                     })
            << true
            << true;
}

void VariableOperationsPlugin::test_fromRefStringAssignmentOperator()
{
    FromRefStringAssignmentOperator fromRefStringAssign;
    testOperator(&fromRefStringAssign);
}

void VariableOperationsPlugin::test_fromRefNumberArithmeticOperator_data()
{
    QTest::addColumn<QString>("moduleName");
    QTest::addColumn<JsonArray>("parameters");
    QTest::addColumn<JsonArray>("inputValue");
    QTest::addColumn<JsonArray>("outputFormat");
    QTest::addColumn<JsonArray>("outputExpected");
    QTest::addColumn<bool>("setModuleParametersResult");
    QTest::addColumn<bool>("launchModuleResult");

    QTest::newRow("+ Number 5")
            << "fromrefnumberarithmeticoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "arithmeticOperator" },
                            { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ QVariant::fromValue(QString("addKey")) }))}
                         })),
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "arithmeticValue" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(5))}

                         }))
                     })
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(5))}
                         }))
                     })
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute,
                                        "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                         getManagedType<Number>()) }
                                   }))
                               }))
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "outputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(10)) }
                         }))
                     })
            << true
            << true;

    QTest::newRow("- Number 5")
            << "fromrefnumberarithmeticoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "arithmeticOperator" },
                            { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ QVariant::fromValue(QString("subKey")) }))}
                         })),
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "arithmeticValue" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(5))}

                         }))
                     })
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(5))}
                         }))
                     })
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute,
                                        "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                         getManagedType<Number>()) }
                                   }))
                               }))
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "outputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(0)) }
                         }))
                     })
            << true
            << true;

    QTest::newRow("* Number 5")
            << "fromrefnumberarithmeticoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "arithmeticOperator" },
                            { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ QVariant::fromValue(QString("multKey")) }))}
                         })),
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "arithmeticValue" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(5))}

                         }))
                     })
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(5))}
                         }))
                     })
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute,
                                        "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                         getManagedType<Number>()) }
                                   }))
                               }))
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "outputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(25)) }
                         }))
                     })
            << true
            << true;

    QTest::newRow("/ Number 5")
            << "fromrefnumberarithmeticoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "arithmeticOperator" },
                            { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ QVariant::fromValue(QString("divKey")) }))}
                         })),
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "arithmeticValue" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(5))}

                         }))
                     })
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(5))}
                         }))
                     })
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute,
                                        "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                         getManagedType<Number>()) }
                                   }))
                               }))
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "outputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(1)) }
                         }))
                     })
            << true
            << true;

    QTest::newRow(" + string '5'")
            << "fromrefnumberarithmeticoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "arithmeticOperator" },
                            { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ QVariant::fromValue(QString("addKey")) }))}
                         })),
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "arithmeticValue" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute, "5"}

                         }))
                     })
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(5))}
                         }))
                     })
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute,
                                        "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                         getManagedType<Number>()) }
                                   }))
                               }))
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "outputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(10)) }
                         }))
                     })
            << true
            << true;
}

void VariableOperationsPlugin::test_fromRefNumberArithmeticOperator()
{
    FromRefNumberArithmeticOperator fromRefNumArithmetic;
    testOperator(&fromRefNumArithmetic);
}

void VariableOperationsPlugin::test_fromMultiRefNumberArithmeticOperator_data()
{
    QTest::addColumn<QString>("moduleName");
    QTest::addColumn<JsonArray>("parameters");
    QTest::addColumn<JsonArray>("inputValue");
    QTest::addColumn<JsonArray>("outputFormat");
    QTest::addColumn<JsonArray>("outputExpected");
    QTest::addColumn<bool>("setModuleParametersResult");
    QTest::addColumn<bool>("launchModuleResult");

    QTest::newRow("+ operator")
            << "frommultirefnumberarithmeticoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "arithmeticOperator" },
                            { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ QVariant::fromValue(QString("addKey")) }))}
                         }))
                     })
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(5))}
                         })),
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputSecPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(5))}
                         }))
                     })
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute,
                                        "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                         getManagedType<Number>()) }
                                   }))
                               }))
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "outputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(10)) }
                         }))
                     })
            << true
            << true;

    QTest::newRow("+ operator overflow")
            << "frommultirefnumberarithmeticoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "arithmeticOperator" },
                            { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ QVariant::fromValue(QString("addKey")) }))}
                         }))
                     })
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(18446744073709551614U))}
                         })),
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputSecPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(2))}
                         }))
                     })
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute,
                                        "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                         getManagedType<Number>()) }
                                   }))
                               }))
            << JsonArray()
            << true
            << false;

    QTest::newRow("- operator")
            << "frommultirefnumberarithmeticoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "arithmeticOperator" },
                            { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ QVariant::fromValue(QString("subKey")) }))}
                         }))
                     })
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(5))}
                         })),
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputSecPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(5))}
                         }))
                     })
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute,
                                        "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                         getManagedType<Number>()) }
                                   }))
                               }))
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "outputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(0)) }
                         }))
                     })
            << true
            << true;

    QTest::newRow("- operator overflow")
            << "frommultirefnumberarithmeticoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "arithmeticOperator" },
                            { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ QVariant::fromValue(QString("subKey")) }))}
                         }))
                     })
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(18446744073709551614U,0,false))}
                         })),
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputSecPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(2))}
                         }))
                     })
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute,
                                        "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                         getManagedType<Number>()) }
                                   }))
                               }))
            << JsonArray()
            << true
            << false;

    QTest::newRow("* operator")
            << "frommultirefnumberarithmeticoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "arithmeticOperator" },
                            { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ QVariant::fromValue(QString("multKey")) }))}
                         }))
                     })
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(5))}
                         })),
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputSecPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(5))}
                         }))
                     })
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute,
                                        "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                         getManagedType<Number>()) }
                                   }))
                               }))
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "outputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(25)) }
                         }))
                     })
            << true
            << true;

    QTest::newRow("* operator overflow")
            << "frommultirefnumberarithmeticoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "arithmeticOperator" },
                            { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ QVariant::fromValue(QString("multKey")) }))}
                         }))
                     })
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(18446744073709551614U,0,false))}
                         })),
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputSecPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(2))}
                         }))
                     })
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute,
                                        "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                         getManagedType<Number>()) }
                                   }))
                               }))
            << JsonArray()
            << true
            << false;

    QTest::newRow("/ operator")
            << "frommultirefnumberarithmeticoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "arithmeticOperator" },
                            { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ QVariant::fromValue(QString("divKey")) }))}
                         }))
                     })
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(5))}
                         })),
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputSecPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(5))}
                         }))
                     })
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute,
                                        "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                         getManagedType<Number>()) }
                                   }))
                               }))
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "outputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(1)) }
                         }))
                     })
            << true
            << true;

    QTest::newRow("/ operator by 0")
            << "frommultirefnumberarithmeticoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "arithmeticOperator" },
                            { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ QVariant::fromValue(QString("divKey")) }))}
                         }))
                     })
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(5))}
                         })),
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "inputSecPin" },
                            { JsonParsersConstants::Sequence::ValueNameAttribute,
                              QVariant::fromValue(Number(0))}
                         }))
                     })
            << JsonArray(JsonArray({
                                   QVariant::fromValue(JsonObject({
                                      { JsonParsersConstants::Generic::NameAttribute,
                                        "outputPin" },
                                      { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                                        TypeManaged::toString(CommonJsonParser::
                                                                         getManagedType<Number>()) }
                                   }))
                               }))
            << JsonArray()
            << true
            << false;

    QTest::newRow("Wrong NameAttribute")
            << "frommultirefnumberarithmeticoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute,"arithmeticOperatorZZ" },
                            { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({ QVariant::fromValue(QString("divKey")) }))}
                         }))
                     })
            << JsonArray()
            << JsonArray()
            << JsonArray()
            << false
            << true;

    QTest::newRow("Wrong ValuesNameAttribute")
            << "frommultirefnumberarithmeticoperator"
            << JsonArray({
                         QVariant::fromValue(JsonObject({
                            { JsonParsersConstants::Generic::NameAttribute, "arithmeticOperator" },
                            { JsonParsersConstants::Sequence::ValuesNameAttribute,
                              QVariant::fromValue(JsonArray({QVariant::fromValue(QString("42"))}))}
                         }))
                     })
            << JsonArray()
            << JsonArray()
            << JsonArray()
            << false
            << true;
}

void VariableOperationsPlugin::test_fromMultiRefNumberArithmeticOperator()
{
    FromMultiRefNumberArithmeticOperator fromRefNumberMultiArithmetic;
    testOperator(&fromRefNumberMultiArithmetic);
}

void VariableOperationsPlugin::testOperator(ABaseOperator * baseType)
{

    QFETCH(QString, moduleName);
    QFETCH(JsonArray, parameters);
    QFETCH(JsonArray, inputValue);
    QFETCH(JsonArray, outputFormat);
    QFETCH(JsonArray, outputExpected);
    QFETCH(bool, setModuleParametersResult);
    QFETCH(bool, launchModuleResult);

    JsonArray errors;
    QString nameModule;

    QVERIFY(baseType->getModuleName(nameModule));
    QCOMPARE(nameModule,moduleName);

    QCOMPARE(baseType->setModuleParameters(parameters, errors),
             setModuleParametersResult);

    if(setModuleParametersResult)
    {
        JsonArray output;
        QVERIFY(baseType->getOutputVariablesFormat(output));

        QSignalSpy spy(baseType, &ABaseOperator::moduleFinished);
        QVERIFY(baseType->launchModule(inputValue));

        spy.wait(5000);

        QCOMPARE(spy.count(), 1);

        QList<QVariant> arguments = spy.takeFirst();

        QCOMPARE(arguments.at(0).toBool(), launchModuleResult);
        if(launchModuleResult)
        {
            if(arguments.at(1).canConvert<JsonArray>())
            {
                QVERIFY(JsonComparator::isEqual(outputExpected,arguments.at(1).value<JsonArray>()));
            }
            else
            {
                QFAIL("The output isn't a JsonArray");
            }
        }
    }
}

QTEST_MAIN(VariableOperationsPlugin)
