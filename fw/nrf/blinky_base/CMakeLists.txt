# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(us-ppg-patch)

include(${ZEPHYR_BASE}/samples/subsys/usb/common/common.cmake)


target_sources(app PRIVATE
	src/main.c
	src/commands.c
	src/wulpus_appl.c
)

# Data link to the PC: BLE or USB, see CONFIG_APP_TRANSPORT_BLE in Kconfig.
if(CONFIG_APP_TRANSPORT_BLE)
	target_sources(app PRIVATE src/transport_ble.c)
else()
	target_sources(app PRIVATE src/transport_usb.c)
endif()

# M7: PPG SOURCE, SEE CONFIG_APP_PPG_REAL IN KCONFIG.
# THE FILE LIST IS THE SAME AS IN BIOGAP'S CMAKELISTS.TXT, WITHOUT I2C_MUX.C
# (i2c_mux/I2C_MUX.H IS A HEADER-ONLY STUB).
if(CONFIG_APP_PPG_REAL)
	target_sources(app PRIVATE
		src/ppg_new/maxm86161_i2c.c
		src/ppg_new/maxm86161.c
		src/ppg_new/ppg_new_appl.c
	)
else()
	target_sources(app PRIVATE src/dummy_source.c)
endif()

# M7: SAME INCLUDE PATHS AS BIOGAP, SO THE COPIED FILES KEEP THEIR #INCLUDE LINES:
# SRC FOR "ppg_new/ppg_new_appl.h" AND "i2c_mux/i2c_mux.h",
# SRC/ppg_new FOR THE BARE "maxm86161_i2c.h" ETC.
target_include_directories(app PRIVATE
	src
	src/ppg_new
)


# Part A of M5. Remove for a real MSP.
target_compile_definitions(app PRIVATE WULPUS_LOOPBACK_TEST)
