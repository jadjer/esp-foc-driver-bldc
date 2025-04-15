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

#include "mcpwm/Comparator.hpp"

namespace mcpwm {

std::unique_ptr<Comparator> Comparator::create(std::shared_ptr<Operator> op, Comparator::Config const &config) {
  Comparator::Handle handle;

  Comparator::Error err = mcpwm_new_comparator(op->getHandle(), &config, &handle);
  if (err != ESP_OK) {
    return nullptr;
  }

  return std::unique_ptr<Comparator>(new Comparator(handle, std::move(op)));
}

Comparator::Comparator(Comparator::Handle handle, std::shared_ptr<Operator> op) : m_handle(handle),
                                                                                  m_parentOperator(std::move(op)) {}

Comparator::~Comparator() {
  if (m_handle) {
    mcpwm_del_comparator(m_handle);
  }
}

Comparator::Error Comparator::setCompareValue(Comparator::Value const compareValue) {
  return mcpwm_comparator_set_compare_value(m_handle, compareValue);
}

Comparator::Handle Comparator::getHandle() const {
  return m_handle;
}

}// namespace mcpwm
