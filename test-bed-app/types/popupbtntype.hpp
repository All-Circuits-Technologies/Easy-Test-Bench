// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>


/** @brief This is the type of Pop up btn */
class PopUpBtnType : public QObject
{
    Q_OBJECT

    public:
        /** @brief The type of button */
        enum Enum
        {
            Unknown = 0x00,
            OkBtn   = 0x01,
            YesBtn  = 0x02,
            NoBtn   = 0x04
        };
        Q_ENUM(Enum)
        Q_DECLARE_FLAGS(Enums, Enum)
        Q_FLAG(Enums)

    public:
        /** @brief Returns a string which can be displayed  (and translated) in view
            @param btnType The type of button to get its display string
            @return The string to display */
        static QString toDisplayString(PopUpBtnType::Enum btnType);

        /** @brief Get all the available enums */
        static const QVector<Enum> &getAllEnums();

        /** @brief Convert the given flag to a list of Enum
            @param btnTypes The flag to convert
            @return The list of enum contains in the flag */
        static QVector<Enum> getEnumsList(Enums btnTypes);

        /** @brief Register meta type linked to this enum */
        static void registerMetaTypes();

    private:
        static const constexpr char* OkLabel = QT_TR_NOOP("(TR) Ok");
        static const constexpr char* YesLabel = QT_TR_NOOP("(TR) Yes");
        static const constexpr char* NoLabel = QT_TR_NOOP("(TR) No");

        static QVector<Enum> EnumList;
};

Q_DECLARE_METATYPE(PopUpBtnType::Enum)
Q_DECLARE_METATYPE(PopUpBtnType::Enums)
Q_DECLARE_OPERATORS_FOR_FLAGS(PopUpBtnType::Enums)
