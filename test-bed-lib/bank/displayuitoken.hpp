// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "jsonutility/jsontypedefs.hpp"
#include "testbedcore/testbedglobal.hpp"


/** @brief The token is used as promise in order to know the results of the element displayed */
class TESTBEDLIB_DLLSPEC DisplayUiToken : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The class parent */
        explicit DisplayUiToken(QObject *parent = nullptr);

        /** @brief Copy constructor
            @param uiToken The object to copy */
        explicit DisplayUiToken(const DisplayUiToken &uiToken);

        /** @brief Class destructor */
        virtual ~DisplayUiToken() override;

    public:
        /** @brief Call to test if the displaying is finished, and a result is available
            @return True if a result is available */
        bool isFinished() const { return _finished; }

        /** @brief Test if the displaying has been canceled by the one who have asked the
                   displaying */
        bool isCanceled() const { return _canceled; }

    public slots:
        /** @brief Call in order to declare the token finished
            @param success True if no error occurred in the UI displaying
            @param valuesSet The values set in the UI displaying */
        void finish(bool success, const JsonArray &valuesSet = JsonArray());

        /** @brief Emit the closeWidget signal */
        void cancelDisplay();

    public:
        /** @brief Register the meta types linked to this class */
        static void RegisterMetaTypes();

        /** @brief Generate a token and set it in a QSharedPointer
            @return The token generated */
        static QSharedPointer<DisplayUiToken> generateToken();

        /** @brief Set the uuid of the token */
        void setUuid(const QString &uuid) { _uuid = uuid; }

        /** @brief Get the uuid of the token*/
        const QString &getUuid() const { return _uuid; }

    signals:
        /** @brief Emitted when the token is finished
            @param success True if no error occurred in the UI displaying
            @param uuidToken uuid of the token
            @param valuesSet The values set in the UI displaying */
        void finished(bool success, const QString &uuidToken, const JsonArray &valuesSet);

        /** @brief Emitted to close the gui corresponding to this token */
        void closeWidget();

    private:
        bool _finished{false};
        bool _canceled{false};
        QString _uuid;
};

Q_DECLARE_METATYPE(DisplayUiToken)
