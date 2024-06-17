// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "definesutility/definesutility.hpp"
#include "definesutility/errormanagement.hpp"


/** @def SIMPLE_PROPERTY_NOSETTER
    @brief Easily create a property member with public getter and accessor
    @param TYPE Type of the member to create
    @param NAME Name of the property, starting with an upper case letter
    @example SIMPLE_PROPERTY_NOSETTER(QString, UserName)
    @note Private member is inserted last so caller can add doxygen after this macro
    @note Known weakness: Created private member name starts with an underscore
          and an uppercase letter which does not confrom to our standard.
          This has been choosen as a simplicity trade-off acceptable since getter/setter
          are properly named and should be used instead of direct member accesses. */
#define SIMPLE_PROPERTY_NOSETTER(TYPE, NAME) \
        private:\
            Q_PROPERTY(TYPE NAME READ get ## NAME WRITE set ## NAME)\
        public:\
            const TYPE &get ## NAME() const { return _ ## NAME; }\
            TYPE &access ## NAME() { return _ ## NAME; }\
        private:\
            TYPE _ ## NAME {};

/** @def SIMPLE_PROPERTY
    @brief Easily create a property member with public getter, accessor and setter
    @param TYPE Type of the member to create
    @param NAME Name of the property, starting with an upper case letter
    @example SIMPLE_PROPERTY(QString, UserName)
    @note Private member is inserted last so caller can add doxygen after this macro
    @note Known weakness: Created private member name starts with an underscore
          and an uppercase letter which does not confrom to our standard.
          This has been choosen as a simplicity trade-off acceptable since getter/setter
          are properly named and should be used instead of direct member accesses. */
#define SIMPLE_PROPERTY(TYPE, NAME) \
        public:\
            void set ## NAME(const TYPE & NAME) { _ ## NAME = NAME; }\
        SIMPLE_PROPERTY_NOSETTER(TYPE, NAME)


/** @def SIMPLE_PROPERTY_PTR
    @brief Easily create a pointer property member with public getter and setter
    @param TYPE Pointed type of the member to create
    @param NAME Name of the property, starting with an upper case letter
    @example SIMPLE_PROPERTY_PTR(QLabel, Foo)
    @note Private member is inserted last so caller can add doxygen after this macro
    @note Known weakness: Created private member name starts with an underscore
          and an uppercase letter which does not confrom to our standard.
          This has been choosen as a simplicity trade-off acceptable since getter/setter
          are properly named and should be used instead of direct member accesses.
    @attention Setter do not deeply copy given pointer.
               Instead it update member pointer to point give item which must live
               as long as member holdler lives.
               The idea is then for our member to be the pointed data owner,
               and calls to the setter gives the ownership too. */
#define SIMPLE_PROPERTY_PTR(TYPE, NAME) \
        private:\
            Q_PROPERTY(TYPE* NAME READ get ## NAME WRITE set ## NAME)\
        public:\
            TYPE *get ## NAME() { return _ ## NAME; }\
            void set ## NAME(TYPE *NAME) { _ ## NAME = NAME; }\
        private:\
            TYPE* _ ## NAME {};

/* Windows requires an explicit export of DLL classes
 * for Qt5 dll-signal-to-app-slot to work at runtime.
 *
 * Without those explicit exports, Qt5 QObject::connect fails
 * at runtime by not finding signals from DLL.
 *
 * You can easily enable Qt GUI verbosity of such errors by calling QErrorMessage::qtHandler() ;
 * You can also test QObject::connect results against boolean value true.
 *
 * Maybe we want to test all connect results?
 *  Something like WARN_IF_FALSE(connect(.....));
 */
#if defined TESTBEDLIB_LIBRARY
#   define  TESTBEDLIB_DLLSPEC          Q_DECL_EXPORT
#   define  TESTBEDPLUGIN_DLLSPEC
#elif defined TESTBEDPLUGIN_LIBRARY
#   define  TESTBEDPLUGIN_DLLSPEC       Q_DECL_EXPORT
#else
#   define  TESTBEDLIB_DLLSPEC          Q_DECL_IMPORT
#   define  TESTBEDPLUGIN_DLLSPEC
#endif
