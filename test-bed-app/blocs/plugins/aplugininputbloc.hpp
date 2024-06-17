// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "blocs/abstractbloc.hpp"

#include <QDebug>

class APluginInputModel;


/** @brief Base class for the plugin input blocs */
class APluginInputBloc : public AbstractBloc
{
    Q_OBJECT

    Q_PROPERTY(APluginInputModel *inputModel READ getInputModel
                                             WRITE setInputModel
                                             NOTIFY inputModelChanged)

    public:
        /** @brief Class constructor
            @param parent Optional parent QObject */
        APluginInputBloc(QObject *parent = nullptr);

    public:
        /** @brief Get the input model */
        APluginInputModel *getInputModel() const { return _inputModel; }

        /** @brief Set the input model */
        void setInputModel(APluginInputModel *value);

    protected:
        /** @brief Set the value entered by the user
            @param value The new value to set
            @return True if no problem occurs */
        bool setValue(const QVariant & /*value*/) { return false; }

        /** @brief Get the model casted
            @note The wanted type has to derive from @ref APluginInputModel */
        template<class T>
        T *getCastedModel() const;

    signals:
        /** @brief Emitted when an error has been detected */
        void error();

    protected slots:
        /** @see AbstractBloc::initBloc */
        virtual void initBloc() override;

    signals:
        /** @brief Emitted when the input model changed */
        void inputModelChanged(APluginInputModel *model);

        /** @brief Emitted to update the remaining time label */
        void updateRemainingTime(int remainingTime);

        /** @brief Emitted when the input view need to be closed */
        void closeInput();

    public:
        /** @brief Register QML type linked to this bloc */
        static void registerQmlTypes();

    private:
        APluginInputModel *_inputModel{nullptr};

};

template<class T>
T *APluginInputBloc::getCastedModel() const
{
    static_assert(std::is_base_of<APluginInputModel, T>::value,
                  "The class has to derive from APluginInputModel");
    if(_inputModel == nullptr)
    {
        // Useless to cast a nullptr
        return nullptr;
    }

    T *casted = qobject_cast<T*>(_inputModel);
    if(casted == nullptr)
    {
        qWarning() << "The input model is not the one expected: "
                   << T::staticMetaObject.className();
        return nullptr;
    }

    return casted;
}
