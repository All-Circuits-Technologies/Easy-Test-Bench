// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "aupdatehexfile.hpp"

#include "abstractpluginelements/defaultbankglobalmanager.hpp"
#include "intelhexfileutility/intelhexdevice.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/testbedglobal.hpp"


AUpdateHexFile::AUpdateHexFile(const QString &moduleClassName,
                               QObject *parent) :
    DefaultSequenceInstanceModule(moduleClassName,
                                  DefaultBankGlobalManager::Instance(),
                                  parent)
{
}

bool AUpdateHexFile::initInputsFormat(JsonArray &inputsFormat) const
{
    JsonObject hexFileInput;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(InputIntelHexFilePathName,
                                                         hexFileInput,
                                                         InputIntelHexFilePathDesc));

    inputsFormat.append(hexFileInput);
    return true;
}

bool AUpdateHexFile::updateHexProcess(const JsonArray &jsonObjectInputValues,
                                      quint16 address,
                                      const QByteArray &value)
{
    QString hexFilePath;
    RETURN_IF_FALSE(BankJsonParser::getStringValue(jsonObjectInputValues,
                                                   InputIntelHexFilePathName,
                                                   hexFilePath));

    IntelHexDevice hexDevice;
    RETURN_IF_FALSE(hexDevice.linkToHexFile(hexFilePath));
    RETURN_IF_FALSE(hexDevice.loadDevice());

    return hexDevice.setData(address, value, true);
}
