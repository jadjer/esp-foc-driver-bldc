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
#include <driver/mcpwm_timer.h>
#include <esp_err.h>
#include <functional>
#include <memory>

namespace mcpwm {

class Timer {
public:
  using Config = mcpwm_timer_config_t;
  using Error = esp_err_t;
  using Handle = mcpwm_timer_handle_t;
  using Command = mcpwm_timer_start_stop_cmd_t;
  using EventData = mcpwm_timer_event_data_t;

public:
  struct EventCallbacks {
    std::function<void()> onFull;
    std::function<void()> onEmpty;
    std::function<void()> onStop;
  };

public:
  static std::shared_ptr<Timer> create(Config const &config);

private:
  explicit Timer(Handle handle);

public:
  Timer(Timer const &) = delete;
  Timer &operator=(Timer const &) = delete;
  ~Timer();

public:
  Error registerEventCallbacks(EventCallbacks const &cbs);

public:
  Error enable();
  Error disable();

public:
  Error startStop(Command command);

public:
  [[nodiscard]] mcpwm_timer_handle_t getHandle() const;

private:
  static bool executeCallback(Handle timer, EventData const *eventData, void *userContext);

private:
  Handle m_handle;
  EventCallbacks m_userCallbacks;
};

}// namespace mcpwm
