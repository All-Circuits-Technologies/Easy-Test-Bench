// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

class QIODevice;


/** @def AJSONPARSER_LOG_ERR
    @brief Log a parsing error and save it for later re-read by caller
    @param MSG Error description
    @see setErrorIfEmpty
    @see getError */
#define AJSONPARSER_LOG_ERR(MSG) do{ qCritical() << (MSG); setErrorIfEmpty(MSG); } while(0)


/** @brief Abstract class which contains generic methods for loading and parsing JSON file */
class AJsonParser : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The parent class */
        explicit AJsonParser(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~AJsonParser() override = default;

    public:
        /** @brief Get first error found during parsing
            @note You should only read it if parsing failed.
                  If no error has been logged, a generic "unknown" error is returned.
            @see clearError
            @see logError
            @return First error seen during parsing. */
        QString getError() const;

    protected:
        /** @brief Load JSON file and get the json object representation
            @param device The device to get the JSON object from
            @param jsonObject The JSON object got from file
            @return True if no problem occurs */
        static bool loadJsonFileAndGetObject(QIODevice &device,
                                             QJsonObject &jsonObject);

        /** @brief Load JSON file and get the json array representation
            @param device The device to get the JSON array from
            @param jsonArray The JSON array got from file
            @return True if no problem occurs */
        static bool loadJsonFileAndGetArray(QIODevice &device, QJsonArray &jsonArray);

        /** @brief Clears error so it have a generic description
            @note Call it upon each new parsing requests */
        void clearError();

        /** @brief Save error if none saved yet
            @param error The error to set */
        void setErrorIfEmpty(const QString &error);

    private:
        /** @brief Description of the first encountered error if any.
            @note Invalid (null) if no errors logged, cleared upon each new parse requests.
            @see AJSONPARSER_LOG_ERR */
        QString _error{};
};
