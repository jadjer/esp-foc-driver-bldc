cmake_minimum_required(VERSION 3.17)

set(SOURCES
        src/Motor.cpp
        src/Driver3PWM.cpp
        src/Driver6PWM.cpp

        src/mcpwm/Comparator.cpp
        src/mcpwm/Generator.cpp
        src/mcpwm/Inverter.cpp
        src/mcpwm/Operator.cpp
        src/mcpwm/Timer.cpp
)

set(COMPONENTS
        esp-foc
        esp_driver_mcpwm
)

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS include REQUIRES ${COMPONENTS})
