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

#include <driver/mcpwm_prelude.h>
#include <esp_err.h>
#include <functional>
#include <memory>

#include <mcpwm/Timer.hpp>

namespace mcpwm {

class Operator {
public:
  using Config = mcpwm_operator_config_t;
  using Handle = mcpwm_oper_handle_t;
  using Error = esp_err_t;

public:
  static std::shared_ptr<Operator> create(Config const &config);

private:
  explicit Operator(Handle handle);

public:
  Operator(Operator const &) = delete;
  Operator &operator=(Operator const &) = delete;
  ~Operator();

public:
  Error connectTimer(std::shared_ptr<Timer> timer);

public:
  [[nodiscard]] Handle getHandle() const;

private:
  Handle m_handle;
  std::shared_ptr<Timer> m_parentTimer;
};

}// namespace mcpwm
