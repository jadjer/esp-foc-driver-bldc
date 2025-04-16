// Copyright 2025 Pavel Suprunov
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "foc/driver/Driver6PWM.hpp"

#include "foc/driver/interface/Driver.hpp"

namespace foc {

Driver6PWM::Driver6PWM(Driver6PWM::Pin aH, Driver6PWM::Pin aL, Driver6PWM::Pin bH, Driver6PWM::Pin bL, Driver6PWM::Pin cH, Driver6PWM::Pin cL)
    : m_phaseAHigh(aH), m_phaseALow(aL), m_phaseBHigh(bH), m_phaseBLow(bL), m_phaseCHigh(cH), m_phaseCLow(cL) {}
void Driver6PWM::setPwm(float Ua, float Ub, float Uc) {}
void Driver6PWM::setPhaseState(PhaseState sa, PhaseState sb, PhaseState sc) {}
void Driver6PWM::init() {}
void Driver6PWM::enable() {}
void Driver6PWM::disable() {}

} // namespace foc
