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

#include "mcpwm/Timer.hpp"

namespace mcpwm {

std::shared_ptr<Timer> Timer::create(Timer::Config const &config) {
  Timer::Handle handle;

  Timer::Error err = mcpwm_new_timer(&config, &handle);
  if (err != ESP_OK) {
    return nullptr;
  }

  return std::shared_ptr<Timer>(new Timer(handle));
}

Timer::Timer(Handle handle) : m_handle(handle) {}

Timer::~Timer() {
  if (m_handle) {
    disable();
    mcpwm_del_timer(m_handle);
  }
}

Timer::Error Timer::registerEventCallbacks(Timer::EventCallbacks const &cbs) {
  mcpwm_timer_event_callbacks_t callbacks = {
      .on_full = cbs.onFull ? &Timer::executeCallback : nullptr,
      .on_empty = cbs.onEmpty ? &Timer::executeCallback : nullptr,
      .on_stop = cbs.onStop ? &Timer::executeCallback : nullptr,
  };

  m_userCallbacks = cbs;
  return mcpwm_timer_register_event_callbacks(m_handle, &callbacks, this);
}

Timer::Error Timer::enable() {
  return mcpwm_timer_enable(m_handle);
}

Timer::Error Timer::disable() {
  return mcpwm_timer_disable(m_handle);
}

Timer::Error Timer::startStop(Command command) {
  return mcpwm_timer_start_stop(m_handle, command);
}

Timer::Handle Timer::getHandle() const {
  return m_handle;
}

bool Timer::executeCallback(Timer::Handle timer, Timer::EventData const *eventData, void *userContext) {
  auto *self = static_cast<Timer *>(userContext);

  if (eventData->direction == MCPWM_TIMER_DIRECTION_UP and self->m_userCallbacks.onFull) {
    self->m_userCallbacks.onFull();
    return true;
  }

  if (eventData->direction == MCPWM_TIMER_DIRECTION_DOWN and self->m_userCallbacks.onEmpty) {
    self->m_userCallbacks.onEmpty();
    return true;
  }

  return false;
}

}// namespace mcpwm
