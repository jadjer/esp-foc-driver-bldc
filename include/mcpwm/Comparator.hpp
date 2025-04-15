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

#include <mcpwm/Operator.hpp>

namespace mcpwm {

class Comparator {
public:
  using Config = mcpwm_comparator_config_t;
  using Error = esp_err_t;
  using Handle = mcpwm_cmpr_handle_t;
  using Value = std::uint32_t;

public:
  static std::unique_ptr<Comparator> create(std::shared_ptr<Operator> op, Config const &config);

private:
  Comparator(Handle handle, std::shared_ptr<Operator> op);

public:
  Comparator(const Comparator &) = delete;
  Comparator &operator=(const Comparator &) = delete;
  ~Comparator();

public:
  Error setCompareValue(Value compareValue);

public:
  [[nodiscard]] Handle getHandle() const;

private:
  Handle m_handle;
  std::shared_ptr<Operator> m_parentOperator;
};

} // namespace mcpwm
