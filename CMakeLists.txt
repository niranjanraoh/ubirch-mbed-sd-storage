cmake_minimum_required(VERSION 3.5)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)

project(ubirch-sd-driver C CXX)
set(CMAKE_CXX_STANDARD 98)

# == MBED OS 5 settings ==
set(PLATFORM TARGET_Freescale/TARGET_MCUXpresso_MCUS)
set(MCU MK82FN256VLL15)
set(TARGET K82F)
set(BOARD UBRIDGE)

set(FEATURES frameworks netsocket filesystem)

add_definitions(
        -DTOOLCHAIN_GCC
        -DCPU_${MCU}
        -DTARGET_${TARGET}
        -DTARGET_${BOARD}
        -DMBED_CONF_RTOS_PRESENT
        -DMBED_CONF_NSAPI_PRESENT
        -DMBED_CONF_EVENTS_PRESENT
        -D__MBED__
        -DDEVICE_RTC
        -DDEVICE_I2C
        -DDEVICE_SPI
        -DDEVICE_SERIAL
        -DDEVICE_TRNG
        -DDEVICE_INTERRUPTIN
        -DDEVICE_ANALOGIN
        -DDEVICE_ANALOGOUT
        -DMBED_CONF_EVENTS_PRESENT=1
        -DMBED_CONF_RTOS_PRESENT=1
        -DMBED_CONF_PLATFORM_STDIO_BAUD_RATE=9600
        -DMBED_CONF_NSAPI_PRESENT=1
        -DMBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE=9600
        -DMBED_CONF_PLATFORM_STDIO_FLUSH_AT_EXIT=0
        -DMBED_CONF_PLATFORM_STDIO_CONVERT_NEWLINES=0
        -DMBED_CONF_APP_FSFAT_SDCARD_INSTALLED
)

set(MBED_OS
        mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_${TARGET}/TARGET_${BOARD}
        mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_${TARGET}/device/TOOLCHAIN_GCC_ARM
        mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_${TARGET}/drivers
        mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_${TARGET}/device
        mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_${TARGET}
        mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/api
        mbed-os/targets/TARGET_Freescale
        mbed-os/targets/${PLATFORM}
        mbed-os/targets/${PLATFORM}/../
        mbed-os/rtos/TARGET_CORTEX/rtx5/TARGET_RTOS_M4_M7/TOOLCHAIN_GCC
        mbed-os/rtos/TARGET_CORTEX/rtx5
        mbed-os/rtos/TARGET_CORTEX
        mbed-os/rtos/rtx/TARGET_CORTEX_M/TARGET_RTOS_M4_M7
        mbed-os/rtos/rtx/TARGET_CORTEX_M
        mbed-os/rtos
        mbed-os/features/filesystem/bd
        mbed-os/features/filesystem/fat/ChaN
        mbed-os/features/filesystem/fat
        mbed-os/features/filesystem
        mbed-os/features/netsocket/cellular
        mbed-os/features/netsocket/
        mbed-os/features/frameworks/greentea-client
        mbed-os/features/frameworks/unity
        mbed-os/features/frameworks/utest
        mbed-os/features
        mbed-os/platform
        mbed-os/hal/storage_abstraction
        mbed-os/hal
        mbed-os/events/enqueue
        mbed-os/events
        mbed-os/drivers
        mbed-os/cmsis/TARGET_CORTEX_M/TOOLCHAIN_GCC
        mbed-os/cmsis/TARGET_CORTEX_M
        mbed-os/cmsis
        mbed-os
        )

set(MBED_OS_SRCS "" source/SDStorage.cpp source/SDStorage.h)
foreach (DIR in ${MBED_OS})
    if (DIR MATCHES "mbed-os/features")
        foreach(FEATURE in ${FEATURES})
            file(GLOB_RECURSE includes
                    ${DIR}/${FEATURE}/*.cpp ${DIR}/${FEATURE}/*.c ${DIR}/${FEATURE}/*.s ${DIR}/${FEATURE}/*.S)
            list(APPEND MBED_OS_SRCS ${includes})
        endforeach()
    else ()
        file(GLOB includes ${DIR}/*.cpp ${DIR}/*.c ${DIR}/*.s ${DIR}/*.S)
        list(APPEND MBED_OS_SRCS ${includes})
    endif ()
endforeach ()

add_library(mbed-os ${MBED_OS_SRCS} source/examples/structFile.cpp)
include_directories(${MBED_OS})
# == END MBED OS 5 ==

#add_library(ubirch-mbed-quectel-m66
#        ubirch-mbed-quectel-m66/source/M66ATParser/BufferedSerial/BufferedPrint.c
#        ubirch-mbed-quectel-m66/source/M66ATParser/BufferedSerial/BufferedSerial.cpp
#        ubirch-mbed-quectel-m66/source/M66ATParser/BufferedSerial/Buffer/MyBuffer.cpp
#        ubirch-mbed-quectel-m66/source/M66ATParser/M66ATParser.cpp
#        ubirch-mbed-quectel-m66/source/M66Interface.cpp
#        )
#target_include_directories(ubirch-mbed-quectel-m66 PUBLIC
#        ubirch-mbed-quectel-m66/source/M66ATParser/BufferedSerial/Buffer
#        ubirch-mbed-quectel-m66/source/M66ATParser/BufferedSerial
#        ubirch-mbed-quectel-m66/source/M66ATParser
#        ubirch-mbed-quectel-m66/source
#        )
#
#include(ubirch-mbed-crypto/ubirch-mbed-nacl-cm0/lib.cmake)
#
#add_library(crypto
#        ubirch-mbed-crypto/source/ubirchCrypto.cpp
#        ubirch-mbed-crypto/source/KeyPair.cpp
#        ubirch-mbed-crypto/source/Base64.cpp
#        )
#target_include_directories(crypto PUBLIC ubirch-mbed-crypto/source)
#target_link_libraries(crypto libnacl)

add_library(SDDRIVER
        sd-driver/TESTS/block_device/basic/basic.cpp
        sd-driver/TESTS/filesystem/basic/basic.cpp
        sd-driver/TESTS/filesystem/fopen/fopen.cpp
        sd-driver/util/fsfat_debug.h
        sd-driver/util/fsfat_test.c
        sd-driver/util/fsfat_test.h
        sd-driver/SDBlockDevice.cpp
        sd-driver/SDBlockDevice.h
        )

target_include_directories(SDDRIVER PUBLIC
        sd-driver/TESTS/filesystem/fopen
        sd-driver/TESTS/filesystem/basic
        sd-driver/TESTS/filesystem/
        sd-driver/TESTS/block_device/basic
        sd-driver/TESTS/filesystem
        sd-driver/TESTS/
        sd-driver/util
        sd-driver
        )
#add_library(JSMN jsmn/jsmn.c)
#target_include_directories(JSMN PUBLIC jsmn)

#add_library(VLLS mbed-kinetis-lowpower/kinetis_lowpower.cpp)
#target_include_directories(VLLS PUBLIC mbed-kinetis-lowpower)

#add_library(HTTP mbed-http/http_parser/http_parser.c)
#target_include_directories(HTTP PUBLIC mbed-http/http_parser
#        mbed-http/source
#        mbed-http)

#add_library(DBGUTILS dbgutil/dbgutil.c)
#target_include_directories(DBGUTILS PUBLIC dbgutil)

#add_library(BME280 BME280/BME280.cpp)
#target_include_directories(BME280 PUBLIC BME280)

add_executable(ubirch
        source/SDStorage.cpp
        source/SDStorage.h
        source/examples/structFile.cpp
        source/examples/sd-fs.cpp
#        source/ubirch/ubirch.cpp
#        config.h source/ubirch/ubirchTRNG.c
        source/examples/sd-fat.cpp
        source/examples/ex-1.cpp source/examples/structFile.cpp)

target_link_libraries(ubirch mbed-os
        SDDRIVER
        edebug
#        ubirch-mbed-quectel-m66
#        crypto
#        HTTP
#        BME280
        )

add_custom_target(ubirch-sddriver-compile ALL
        COMMAND mbed compile --profile mbed-os/tools/profiles/debug.json
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})