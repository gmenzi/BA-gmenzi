# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(us-ppg-patch)

include(${ZEPHYR_BASE}/samples/subsys/usb/common/common.cmake)


target_sources(app PRIVATE
	src/main.c
	src/commands.c
	src/dummy_source.c
	src/wulpus_appl.c
)

# Data link to the PC: BLE or USB, see CONFIG_APP_TRANSPORT_BLE in Kconfig.
if(CONFIG_APP_TRANSPORT_BLE)
	target_sources(app PRIVATE src/transport_ble.c)
else()
	target_sources(app PRIVATE src/transport_usb.c)
endif()


# Part A of M5. Remove for a real MSP.
target_compile_definitions(app PRIVATE WULPUS_LOOPBACK_TEST)

