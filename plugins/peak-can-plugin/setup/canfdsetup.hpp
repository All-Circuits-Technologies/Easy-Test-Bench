// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "setup/acansetup.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief This module is used to setup a CAN FD communication */
class CanFdSetup : public ACanSetup
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(CanFdSetup)

    public:
        /** @brief Class constructor
            @param parent The object parent */
        Q_INVOKABLE explicit CanFdSetup(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~CanFdSetup() override;

    protected:
        /** @copydoc ACanSetup::initParamsFormat
            @see ACanSetup::addCommonParamsFormat
            @note @ref FClockInMHzParamName : number (This is the clock frequency in MHz)
            @note @ref NomBrpParamName : number (This is the BRP param value for the nominal
                                                 bitrate)
            @note @ref NomTseg1ParamName : number (This is the TSEG1 param value for the nominal
                                                   bitrate)
            @note @ref NomTseg2ParamName : number (This is the TSEG2 param value for the nominal
                                                   bitrate)
            @note @ref NomSjwParamName : number (This is the SJW param value for the nominal
                                                 bitrate)
            @note @ref DataBrpParamName : number (This is the BRP param value for the nominal
                                                  bitrate)
            @note @ref DataTseg1ParamName : number (This is the TSEG1 param value for the nominal
                                                    bitrate)
            @note @ref DataTseg2ParamName : number (This is the TSEG2 param value for the nominal
                                                    bitrate)
            @note @ref DataSjwParamName : number (This is the SJW param value for the nominal
                                                  bitrate) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @copydoc DefaultSequenceInstanceModule::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

    private:
        static const constexpr char *FClockInMHzParamName = "fClockInMHz";
        static const constexpr char *FClockInMHzParamDesc = "This is the clock frequency in MHz";

        static const constexpr char *NomBrpParamName = "nomBrp";
        static const constexpr char *NomBrpParamDesc = "This is the BRP param value for the "
                                                       "nominal bitrate";

        static const constexpr char *NomTseg1ParamName = "nomTseg1";
        static const constexpr char *NomTseg1ParamDesc = "This is the TSEG1 param value for the "
                                                         "nominal bitrate";

        static const constexpr char *NomTseg2ParamName = "nomTseg2";
        static const constexpr char *NomTseg2ParamDesc = "This is the TSEG2 param value for the "
                                                         "nominal bitrate";

        static const constexpr char *NomSjwParamName = "nomSjw";
        static const constexpr char *NomSjwParamDesc = "This is the SJW param value for the "
                                                       "nominal bitrate";

        static const constexpr char *DataBrpParamName = "dataBrp";
        static const constexpr char *DataBrpParamDesc = "This is the BRP param value for the "
                                                        "nominal bitrate";

        static const constexpr char *DataTseg1ParamName = "dataTseg1";
        static const constexpr char *DataTseg1ParamDesc = "This is the TSEG1 param value for the "
                                                          "nominal bitrate";

        static const constexpr char *DataTseg2ParamName = "dataTseg2";
        static const constexpr char *DataTseg2ParamDesc = "This is the TSEG2 param value for the "
                                                          "nominal bitrate";

        static const constexpr char *DataSjwParamName = "dataSjw";
        static const constexpr char *DataSjwParamDesc = "This is the SJW param value for the "
                                                        "nominal bitrate";
};
