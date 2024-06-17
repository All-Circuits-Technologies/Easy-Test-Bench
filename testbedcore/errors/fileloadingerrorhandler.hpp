// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "errordescriptor.hpp"


/** @brief Class used to store and translate the error stack related to file loading */
class FileLoadingErrorHandler: public QObject
{
    Q_OBJECT
        
    public:
        /** @brief Class constructor
            @param parent An optional parent QObject */
        explicit FileLoadingErrorHandler(QObject *parent = nullptr);
        
    public:
        /** @brief Convert all error descriptors into messages understandable by the user
            @return A string list containing the messages */
        virtual QStringList getMessages();

        /** @brief Get a vector containing the error descriptors */
        QVector<ErrorDescriptor> &accessErrorDescriptors() { return _errorDescriptors; }

        /** @brief Add error descriptor
            @param errorDescriptor The descriptor of the error */
        void addErrorDescriptor(const ErrorDescriptor &errorDescriptor);

        /** @brief Get the boolean that informs if it's a missing project file error */
        bool isMissingProjectFileError() const { return _missingProjectFileError; }

        /** @brief Set the boolean that informs if it's a missing project file error */
        void setMissingProjectFileError(bool value) { _missingProjectFileError = value; }

        /** @brief Remove all error descriptors stored */
        void clearErrors();

    protected:
        /** @brief Convert an error descriptor into a message
            @param descriptor The error descriptor */
        virtual QString getDescriptorMessage(const ErrorDescriptor &descriptor);

        /** @brief Put the error descriptors parameters into the error message
            @param parameters The error descriptor parameters
            @param message The error message */
        void putMessageParameters(const QVector<QVariant> &parameters, QString &message);

    private:
        QVector<ErrorDescriptor> _errorDescriptors{};
        bool _missingProjectFileError = false;
};
