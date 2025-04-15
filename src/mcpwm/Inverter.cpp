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

#include "mcpwm/Inverter.hpp"

namespace mcpwm {

auto const TAG = "Inverter";

std::unique_ptr<Inverter> Inverter::create(Inverter::Config const &config) {
  auto instance = std::unique_ptr<Inverter>(new Inverter());

  ESP_RETURN_ON_FALSE(instance->init(config), nullptr, TAG, "Initialization failed");

  return instance;
}

Inverter::~Inverter() {
  if (m_timer) {
    m_timer->disable();
  }
}

Inverter::Error Inverter::registerCallbacks(Timer::EventCallbacks const &callbacks) { return m_timer->registerEventCallbacks(callbacks); }

Inverter::Error Inverter::start(Timer::Command command) {
  if (command != MCPWM_TIMER_STOP_EMPTY and command != MCPWM_TIMER_STOP_FULL) {
    ESP_RETURN_ON_ERROR(m_timer->enable(), TAG, "Timer enable failed");
  }

  return m_timer->startStop(command);
}

Inverter::Error Inverter::setDuty(uint16_t u, uint16_t v, uint16_t w) {
  ESP_RETURN_ON_ERROR(m_comparators[0]->setCompareValue(u), TAG, "Set duty U failed");
  ESP_RETURN_ON_ERROR(m_comparators[1]->setCompareValue(v), TAG, "Set duty V failed");
  ESP_RETURN_ON_ERROR(m_comparators[2]->setCompareValue(w), TAG, "Set duty W failed");

  return ESP_OK;
}

bool Inverter::init(Inverter::Config const &config) {
  m_timer = Timer::create(config.timer_config);
  if (not m_timer) {
    ESP_LOGE(TAG, "Create MCPWM timer failed");
    return false;
  }

  for (int i = 0; i < 3; i++) {
    m_operators[i] = Operator::create(config.operator_config);
    if (not m_operators[i]) {
      ESP_LOGE(TAG, "Create operator failed");
      return false;
    }

    if (m_operators[i]->connectTimer(m_timer) != ESP_OK) {
      ESP_LOGE(TAG, "Connect operator failed");
      return false;
    }

    m_comparators[i] = Comparator::create(m_operators[i], config.compare_config);
    if (not m_comparators[i]) {
      ESP_LOGE(TAG, "Create comparator %d failed", i);
      return false;
    }

    if (m_comparators[i]->setCompareValue(0) != ESP_OK) {
      ESP_LOGE(TAG, "Set comparator %d failed", i);
      return false;
    }

    for (int j = 0; j < 2; j++) {
      auto const pin = config.pins[i][j];

      m_generators[i][j] = Generator::create(m_operators[i], pin);
      if (not m_generators[i][j]) {
        ESP_LOGE(TAG, "Create generator %d-%d failed", i, j);
        return false;
      }
    }

    configureGenerator(m_generators[i][0], m_generators[i][1], m_comparators[i], config);
  }

  return true;
}

bool Inverter::configureGenerator(std::shared_ptr<Generator> const &generatorHigh, std::shared_ptr<Generator> const &generatorLow, std::shared_ptr<Comparator> const &comparator,
                                  Config const &config) {

  ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(generatorHigh->getHandle(),
                                                            MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)));
  ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(generatorHigh->getHandle(),
                                                              MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, comparator->getHandle(), MCPWM_GEN_ACTION_LOW)));

  // Настройка генератора нижнего ключа (комплементарный сигнал с dead-time)
  ESP_ERROR_CHECK(
      mcpwm_generator_set_action_on_timer_event(generatorLow->getHandle(), MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_LOW)));
  ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(generatorLow->getHandle(),
                                                              MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, comparator->getHandle(), MCPWM_GEN_ACTION_HIGH)));

  ESP_ERROR_CHECK(mcpwm_generator_set_dead_time(generatorHigh->getHandle(), generatorHigh->getHandle(), &config.dt_config));
  ESP_ERROR_CHECK(mcpwm_generator_set_dead_time(generatorHigh->getHandle(), generatorLow->getHandle(), &config.inv_dt_config));

  return true;
}

} // namespace mcpwm
