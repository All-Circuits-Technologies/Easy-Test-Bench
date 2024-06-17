// SPDX-FileCopyrightText: 2020 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "functions/ajlinkintffromparams.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief The module erases a board connected to a JLink probe. */
class JLinkErase : public AJLinkIntfFromParams
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(JLinkErase)

    public:
        /** @brief Class constructor
            @param parent The parent class */
        Q_INVOKABLE explicit JLinkErase(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~JLinkErase() override;

    protected:
        /** @copydoc AJLinkIntfFromParams::mainJLinkProcess */
        virtual bool mainJLinkProcess(const JsonArray &jsonObjectInputValues,
                                      const QString &jLinkPgmPath,
                                      JsonArray &output) override;

    private:
        /** @brief Call to execute JLink.exe
            @param jLinkPgmPath The JLink program path
            @return True if no problem occurred */
       bool operate(const QString &jLinkPgmPath);

    private:
        static const constexpr char *HmiName = "Erase board";
        static const constexpr char *HmiDescription = QT_TR_NOOP("(TR) Board is being erased, this "
                                                                 "can take a few seconds");
};
