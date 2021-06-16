cmake_minimum_required(VERSION 3.13.1)
include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(zephyr-dwm1001)

FILE(GLOB app_sources src/*.c)
target_sources(app PRIVATE ${app_sources})

FILE(GLOB i2c_sources src/I2C/*.c)
target_sources(app PRIVATE ${i2c_sources})

