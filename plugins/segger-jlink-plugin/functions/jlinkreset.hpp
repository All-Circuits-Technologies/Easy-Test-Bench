// SPDX-FileCopyrightText: 2020 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "functions/ajlinkintffromparams.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief Class to reset a board using JLink.exe */
class JLinkReset : public AJLinkIntfFromParams
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(JLinkReset)

    public:
        /** @brief Class constructor
            @param parent The parent class */
        Q_INVOKABLE explicit JLinkReset(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~JLinkReset() override;

    protected:
        /** @copydoc AJLinkIntfFromParams::mainJLinkProcess */
        virtual bool mainJLinkProcess(const JsonArray &jsonObjectInputValues,
                                      const QString &jLinkPgmPath,
                                      JsonArray &output) override;

    private:
        /** @brief Call to retrieve inputValues and execute JLink.exe
            @param jLinkPgmPath The JLink program path
            @return True if no problem occurred */
       bool operate(const QString &jLinkPgmPath);

    private:
        static const constexpr char *hmiName = "Reset board";
        static const constexpr char *hmiDescription = QT_TR_NOOP("(TR) Board is reset, this "
                                                                 "can take a few seconds");
};
