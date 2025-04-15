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
#include <esp_check.h>
#include <esp_log.h>
#include <memory>
#include <utility>

#include <mcpwm/Comparator.hpp>
#include <mcpwm/Generator.hpp>
#include <mcpwm/Operator.hpp>
#include <mcpwm/Timer.hpp>

namespace mcpwm {

class Inverter {
public:
  using Error = esp_err_t;
  using Pin = std::uint8_t;

public:
  struct Config {
    Timer::Config timer_config;
    Operator::Config operator_config;
    Comparator::Config compare_config;
    std::array<std::array<Pin, 2>, 3> pins;
    Generator::DeadTimeConfig dt_config;
    Generator::DeadTimeConfig inv_dt_config;
  };

public:
  static std::unique_ptr<Inverter> create(Config const &config);

private:
  Inverter() = default;

public:
  Inverter(Inverter const &) = delete;
  Inverter &operator=(Inverter const &) = delete;
  ~Inverter();

public:
  Error registerCallbacks(Timer::EventCallbacks const &callbacks);

public:
  Error start(Timer::Command command);
  Error setDuty(uint16_t u, uint16_t v, uint16_t w);

private:
  bool init(Config const &config);
  bool configureGenerator(
      std::shared_ptr<Generator> const &generatorHigh,
      std::shared_ptr<Generator> const &generatorLow,
      std::shared_ptr<Comparator> const &comparator,
      Config const &config);

private:
  std::shared_ptr<Timer> m_timer;
  std::array<std::shared_ptr<Operator>, 3> m_operators;
  std::array<std::shared_ptr<Comparator>, 3> m_comparators;
  std::array<std::array<std::shared_ptr<Generator>, 2>, 3> m_generators;
};

}// namespace mcpwm
