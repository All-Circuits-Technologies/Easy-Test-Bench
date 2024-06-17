// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "registering/registermoduleutility.hpp"

#include "bankcoreshared/usebylib/itopologyinstancemodule.hpp"


/** @brief This class helps to register topology modules
    @note The main goal is to allow the quick creation and registering of a topology module
    @note When registering a class, it's necessary to make Q_INVOKABLE a constructor with a
          QObject *parent */
class RegisterTopologyModuleUtility : public RegisterModuleUtility
{
    private:
        /** @brief Class constructor */
        explicit RegisterTopologyModuleUtility();

    public:
        /** @brief Register the class given and return the singleton instance
            @return The singleton instance of this class */
        template<class T>
        static const RegisterTopologyModuleUtility &registerModule();

    public:
        /** @brief Access to the instance of the class singleton
            @return The class singleton */
        static RegisterTopologyModuleUtility &Instance();

    private:
        inline static RegisterTopologyModuleUtility *_instance = nullptr;
};

template<class T>
const RegisterTopologyModuleUtility &RegisterTopologyModuleUtility::registerModule()
{
    static_assert(std::is_base_of<ITopologyInstanceModule, T>::value() &&
                  std::is_base_of<QObject, T>::value(),
                  "The class is not a sequence instance module");

    RegisterTopologyModuleUtility::Instance().insertInList(T::staticMetaObject);

    return RegisterTopologyModuleUtility::Instance();
}

#define REGISTER_TOPOLOGY_MODULE(T)                                                               \
    private: inline static const RegisterTopologyModuleUtility & _TOPOLOGYMODULEUTILITY =         \
                                                RegisterTopologyModuleUtility::registerModule<T>();
