// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "emtestbenchutility/command/acmd.hpp"

#include "pulseenum.hpp"


/** @brief This class is helpful to generate pulse command
    @note It stores all the generic parameters*/
class APulseCmd : public ACmd
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param header The command header symbolising the type of pulse
            @param parent The parent instance class */
        explicit APulseCmd(PulseEnum::Enum header, QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~APulseCmd() override;

    public:
        /** @return the Vb value as quint16 */
        quint16 getVb() const { return _vb; }

        /** @brief Set the Vb value as quint16 */
        void setVb(quint16 vb) { _vb = vb; }

        /** @return the Vs value as quint16 */
        quint16 getVs() const { return _vs; }

        /** @brief Set the Vs value as quint16 */
        void setVs(quint16 vs) { _vs = vs; }

        /** @return the Va value as quint16 */
        quint16 getVa() const { return _va; }

        /** @brief Set the Va value as quint16 */
        void setVa(quint16 va) { _va = va; }

        /** @return the Vp value as quint16 */
        quint16 getVp() const { return _vp; }

        /** @brief Set the Vp value as quint16 */
        void setVp(quint16 vp) { _vp = vp; }

        /** @return the T6 value as quint16 */
        quint16 getT6() const { return _t6; }

        /** @brief Set the T6 value as quint16 */
        void setT6(quint16 t6) { _t6 = t6; }

        /** @return the T7 value as quint16 */
        quint16 getT7() const { return _t7; }

        /** @brief Set the T7 value as quint16 */
        void setT7(quint16 t7) { _t7 = t7; }

        /** @return the Tf value as quint16 */
        quint16 getTf() const { return _tf; }

        /** @brief Set the Tf value as quint16 */
        void setTf(quint16 tf) { _tf = tf; }

        /** @return the T8 value as quint16 */
        quint16 getT8() const { return _t8; }

        /** @brief Set the T8 value as quint16 */
        void setT8(quint16 t8) { _t8 = t8; }

        /** @return the Interruption value as quint16 */
        quint16 getInterruption() const { return _interruption; }

        /** @brief Set the Interruption value as quint16 */
        void setInterruption(quint16 interruption) { _interruption = interruption; }

        /** @return the number of cycle value as quint16 */
        quint16 getNbCycles() const { return _nbCycles; }

        /** @brief Set the number of cycle value as quint16 */
        void setNbCycles(quint16 nbCycles) { _nbCycles = nbCycles; }

        /** @return the trigger value as quint16 */
        quint16 getTrigger() const { return _trigger; }

        /** @brief Set the Trigger value as quint16 */
        void setTrigger(quint16 trigger) { _trigger = trigger; }

        /** @return the Current value as quint16 */
        quint16 getCurrent() const { return _current; }

        /** @brief Set the Current value as quint16 */
        void setCurrent(quint16 current) { _current = current; }

private:
        quint16 _vb;
        quint16 _vs;
        quint16 _va;
        quint16 _vp;
        quint16 _t6;
        quint16 _t7;
        quint16 _tf;
        quint16 _t8;
        quint16 _interruption;
        quint16 _nbCycles;
        quint16 _trigger;
        quint16 _current;
};
