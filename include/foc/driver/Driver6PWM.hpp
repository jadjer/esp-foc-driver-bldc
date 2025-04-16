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

#pragma once

#include "foc/driver/interface/Driver.hpp"
#include <cstdint>
#include <foc/driver/DriverBase.hpp>

namespace foc {

class Driver6PWM : public DriverBase {
public:
  using Pin = std::uint8_t;

public:
  Driver6PWM(Pin aH, Pin aL, Pin bH, Pin bL, Pin cH, Pin cL);
  ~Driver6PWM() override = default;

public:
  void init() override;

public:
  void enable() override;
  void disable() override;

public:
  void setPwm(float Ua, float Ub, float Uc) override;
  void setPhaseState(PhaseState sa, PhaseState sb, PhaseState sc) override;

private:
  Pin const m_phaseAHigh;
  Pin const m_phaseALow;
  Pin const m_phaseBHigh;
  Pin const m_phaseBLow;
  Pin const m_phaseCHigh;
  Pin const m_phaseCLow;
};

} // namespace foc
