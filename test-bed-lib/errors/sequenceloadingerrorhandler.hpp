// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "errors/fileloadingerrorhandler.hpp"

#include "testbedcore/testbedglobal.hpp"


/** @brief Class used to store and translate the error stack related to sequence file loading **/
class TESTBEDLIB_DLLSPEC SequenceLoadingErrorHandler : public FileLoadingErrorHandler
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent An optional parent QObject **/
        explicit SequenceLoadingErrorHandler(QObject *parent = nullptr);

    private:
        /** @brief Convert an error descriptor into a message **/
        /** @param descriptor The error descriptor **/
        QString getDescriptorMessage(const ErrorDescriptor &descriptor) override;
};
