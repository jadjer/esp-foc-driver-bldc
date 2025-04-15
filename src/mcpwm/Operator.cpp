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

#include "mcpwm/Operator.hpp"

namespace mcpwm {

std::shared_ptr<Operator> Operator::create(Operator::Config const &config) {
  Operator::Handle handle;

  Operator::Error err = mcpwm_new_operator(&config, &handle);
  if (err != ESP_OK) {
    return nullptr;
  }

  return std::shared_ptr<Operator>(new Operator(handle));
}

Operator::Operator(Operator::Handle handle) : m_handle(handle) {}

Operator::~Operator() {
  if (m_handle) {
    mcpwm_del_operator(m_handle);
  }
}

Operator::Error Operator::connectTimer(std::shared_ptr<Timer> timer) {
  Operator::Error err = mcpwm_operator_connect_timer(m_handle, timer->getHandle());
  if (err == ESP_OK) {
    m_parentTimer = std::move(timer);
  }

  return err;
}

Operator::Handle Operator::getHandle() const { return m_handle; }

} // namespace mcpwm
