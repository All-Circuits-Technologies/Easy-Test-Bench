// SPDX-FileCopyrightText: 2021 - 2022 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "aresponse.hpp"
#include "backresponsecategory.hpp"
#include "backresponsetype.hpp"


/** @brief This class is used to manage BackResponse coming from EM_Test instruments*/
class BackResponse : public AResponse
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param response The back response from instruments
            @param parent The parent instance class */
        explicit BackResponse(const QString &response, QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~BackResponse() override;

    public:
        /** @brief Parse the back response
            @note Before parsing Back response = RR,15;
                  After parsing Back Response = RR15
            @return return QString formated back response*/
        virtual bool parseResponse(const QString &response) override;

        /** @brief identify the backResponse type (info or error)
            @return return the type of the backResponse*/
        const BackResponseCategory::Enum getBackResponseCategory() const;

        /** @brief return the backResponseEnum value
            @return return the type of the backResponse*/
        const BackResponseType::Enum &getBackResponseEnum() const { return _backResponseEnum; };

    private:
        BackResponseType::Enum _backResponseEnum;
};
