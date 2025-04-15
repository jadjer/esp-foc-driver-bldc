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

#include <array>
#include <driver/mcpwm_prelude.h>
#include <esp_err.h>
#include <functional>
#include <memory>

#include <mcpwm/Comparator.hpp>
#include <mcpwm/Operator.hpp>

namespace mcpwm {

class Generator {
public:
  using Action = mcpwm_gen_timer_event_action_t;
  using Config = mcpwm_generator_config_t;
  using CompareEventAction = mcpwm_gen_compare_event_action_t;
  using DeadTimeConfig = mcpwm_dead_time_config_t;
  using Error = esp_err_t;
  using Handle = mcpwm_gen_handle_t;
  using Pin = std::uint8_t;
  using TimerEventAction = mcpwm_gen_timer_event_action_t;

public:
  static std::unique_ptr<Generator> create(std::shared_ptr<Operator> op, Pin pin);

private:
  Generator(Handle handle, std::shared_ptr<Operator> op);

public:
  Generator(Generator const &) = delete;
  Generator &operator=(Generator const &) = delete;
  ~Generator();

public:
  Error setActionOnTimerEvent(TimerEventAction action);
  Error setActionOnCompareEvent(CompareEventAction action);
  Error setDeadTime(Generator const *partner, DeadTimeConfig const &config);

public:
  [[nodiscard]] Handle getHandle() const;

private:
  Handle m_handle;
  std::shared_ptr<Operator> m_parentOperator;
};

}// namespace mcpwm
