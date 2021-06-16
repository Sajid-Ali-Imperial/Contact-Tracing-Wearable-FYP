cmake_minimum_required(VERSION 3.13.1)
include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(zephyr-dwm1001)

FILE(GLOB app_sources src/*.c)
target_sources(app PRIVATE ${app_sources})

FILE(GLOB ble_sources src/BLE/*.c)
target_sources(app PRIVATE ${ble_sources})

FILE(GLOB uwb_sources src/UWB/*.c)
target_sources(app PRIVATE ${uwb_sources})

FILE(GLOB i2c_sources src/I2C/*.c)
target_sources(app PRIVATE ${i2c_sources})

target_sources(app PRIVATE ../../decadriver/deca_device.c)
target_sources(app PRIVATE ../../decadriver/deca_params_init.c)

target_sources(app PRIVATE ../../platform/port.c)
target_sources(app PRIVATE ../../platform/deca_mutex.c)
target_sources(app PRIVATE ../../platform/deca_range_tables.c)
target_sources(app PRIVATE ../../platform/deca_sleep.c)
target_sources(app PRIVATE ../../platform/deca_spi.c)


target_include_directories(app PRIVATE ../../decadriver/)
target_include_directories(app PRIVATE ../../platform/)
target_include_directories(app PRIVATE ../../compiler/)

zephyr_library_include_directories($ENV{ZEPHYR_BASE}/samples/bluetooth)