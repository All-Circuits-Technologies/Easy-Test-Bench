// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "permstorefilebrowsermodule.hpp"

#include "bankcoreshared/usebyplugin/icommoninterface.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/testbedglobal.hpp"


PermStoreFileBrowserModule::PermStoreFileBrowserModule(QObject *parent) :
    AFileBrowserModule(staticMetaObject.className(), parent)
{
}

bool PermStoreFileBrowserModule::initInputsFormat(JsonArray &inputsFormat) const
{
    JsonObject permStorageKey;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(InputPermStorageKeyName,
                                                         permStorageKey,
                                                         InputPermStorageKeyDesc));

    inputsFormat.append(permStorageKey);

    return true;
}

bool PermStoreFileBrowserModule::getDefaultValue(const JsonArray &jsonObjectInputValues,
                                                 QString &defaultValue)
{
    RETURN_IF_FALSE(BankJsonParser::getStringValue(jsonObjectInputValues,
                                                   InputPermStorageKeyName,
                                                   _permanentKey));

    return getCommonItf().getPermStoreValue(_permanentKey, defaultValue);
}

bool PermStoreFileBrowserModule::saveChosenValue(const QString &value)
{
    return getCommonItf().setPermStoreValue(_permanentKey, value);
}
