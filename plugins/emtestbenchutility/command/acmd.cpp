// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "acmd.hpp"

#include <QDebug>

#include "byteutility/checksum8bitshelper.hpp"


ACmd::ACmd(const QString &header, QObject *parent) :
    QObject(parent),
    _header(header)
{
}

ACmd::~ACmd()
{
}

bool ACmd::formatCommand(QByteArray &formatedCommand)
{
    QVector<quint16> parameters;
    quint8 checksum;
    QByteArray cmd;

    cmd.append(_header.toLatin1());

    if(!getParameters(parameters))
    {
        qWarning() << "Can not get parameters of unknown cmd";
        return false;
    }

    QVector<quint16>::const_iterator iter = parameters.cbegin();
    for (; iter != parameters.cend(); ++iter)
    {
        cmd.append(comma)
           .append(QByteArray::number(*iter));
    }

    cmd.append(semicolon);

    if(!Checksum8BitsHelper::calculateChecksum(cmd, checksum))
    {
        qWarning() << "Can not calculate command checksum";
        return false;
    }

    formatedCommand.append(cmd)
                   .append(QByteArray::fromHex(QString::number(checksum, 16).toLatin1()))
                   .append(linefeed);

    return true;
}
