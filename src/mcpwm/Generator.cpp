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

#include "mcpwm/Generator.hpp"

namespace mcpwm {

std::unique_ptr<Generator> Generator::create(std::shared_ptr<Operator> op, Pin const pin) {
  Generator::Handle handle;

  Generator::Config generatorConfig = {
      .gen_gpio_num = pin,
  };

  Generator::Error error = mcpwm_new_generator(op->getHandle(), &generatorConfig, &handle);
  if (error != ESP_OK) {
    return nullptr;
  }

  return std::unique_ptr<Generator>(new Generator(handle, std::move(op)));
}

Generator::Generator(Generator::Handle handle, std::shared_ptr<Operator> op) : m_handle(handle), m_parentOperator(std::move(op)) {}

Generator::~Generator() {
  if (m_handle) {
    mcpwm_del_generator(m_handle);
  }
}

Generator::Error Generator::setActionOnTimerEvent(Generator::TimerEventAction const action) { return mcpwm_generator_set_action_on_timer_event(m_handle, action); }

Generator::Error Generator::setActionOnCompareEvent(Generator::CompareEventAction const action) { return mcpwm_generator_set_action_on_compare_event(m_handle, action); }

Generator::Error Generator::setDeadTime(Generator const *partner, Generator::DeadTimeConfig const &config) {
  return mcpwm_generator_set_dead_time(m_handle, partner ? partner->getHandle() : nullptr, &config);
}

Generator::Handle Generator::getHandle() const { return m_handle; }

} // namespace mcpwm
