// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "registering/registermoduleutility.hpp"

#include "bankcoreshared/usebylib/isequenceinstancemodule.hpp"


/** @brief This class helps to register sequence modules
    @note The main goal is to allow the quick creation and registering of a sequence module
    @note When registering a class, it's necessary to make Q_INVOKABLE a constructor with a
          QObject *parent */
class RegisterSequenceModuleUtility : public RegisterModuleUtility
{
    private:
        /** @brief Class constructor */
        explicit RegisterSequenceModuleUtility();

    public:
        /** @brief Register the class given and return the singleton instance
            @return The singleton instance of this class */
        template<class T>
        static const RegisterSequenceModuleUtility &registerModule();

    public:
        /** @brief Access to the instance of the class singleton
            @return The class singleton */
        static RegisterSequenceModuleUtility &Instance();

    private:
        inline static RegisterSequenceModuleUtility *_instance = nullptr;
};

template<class T>
const RegisterSequenceModuleUtility &RegisterSequenceModuleUtility::registerModule()
{
    static_assert(std::is_base_of<ISequenceInstanceModule, T>::value &&
                  std::is_base_of<QObject, T>::value,
                  "The class is not a sequence instance module");

    RegisterSequenceModuleUtility::Instance().insertInList(T::staticMetaObject);

    return RegisterSequenceModuleUtility::Instance();
}

#define REGISTER_SEQUENCE_MODULE(T)                                                             \
    private: inline static const RegisterSequenceModuleUtility &_SEQUENCEMODULEUTILITY =        \
                                                RegisterSequenceModuleUtility::registerModule<T>();
