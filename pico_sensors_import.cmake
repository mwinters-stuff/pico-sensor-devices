# This is a copy of <PICO_SENSORS_PATH>/external/pico_sensors_import.cmake
# based on the one in pico-extras

# This can be dropped into an external project to help locate pico-sensors
# It should be include()ed prior to project()

if (DEFINED ENV{PICO_SENSORS_PATH} AND (NOT PICO_SENSORS_PATH))
    set(PICO_SENSORS_PATH $ENV{PICO_SENSORS_PATH})
    message("Using PICO_SENSORS_PATH from environment ('${PICO_SENSORS_PATH}')")
endif ()

if (DEFINED ENV{PICO_SENSORS_FETCH_FROM_GIT} AND (NOT PICO_SENSORS_FETCH_FROM_GIT))
    set(PICO_SENSORS_FETCH_FROM_GIT $ENV{PICO_SENSORS_FETCH_FROM_GIT})
    message("Using PICO_SENSORS_FETCH_FROM_GIT from environment ('${PICO_SENSORS_FETCH_FROM_GIT}')")
endif ()

if (DEFINED ENV{PICO_SENSORS_FETCH_FROM_GIT_PATH} AND (NOT PICO_SENSORS_FETCH_FROM_GIT_PATH))
    set(PICO_SENSORS_FETCH_FROM_GIT_PATH $ENV{PICO_SENSORS_FETCH_FROM_GIT_PATH})
    message("Using PICO_SENSORS_FETCH_FROM_GIT_PATH from environment ('${PICO_SENSORS_FETCH_FROM_GIT_PATH}')")
endif ()

if (NOT PICO_SENSORS_PATH)
    if (PICO_SENSORS_FETCH_FROM_GIT)
        include(FetchContent)
        set(FETCHCONTENT_BASE_DIR_SAVE ${FETCHCONTENT_BASE_DIR})
        if (PICO_SENSORS_FETCH_FROM_GIT_PATH)
            get_filename_component(FETCHCONTENT_BASE_DIR "${PICO_SENSORS_FETCH_FROM_GIT_PATH}" REALPATH BASE_DIR "${CMAKE_SOURCE_DIR}")
        endif ()
        FetchContent_Declare(
                pico_sensors
                GIT_REPOSITORY https://github.com/mwinters-stuff/pico-sensor-devices.git
                GIT_TAG master
        )
        if (NOT pico_sensors)
            message("Downloading Raspberry Pi Pico Sensors")
            FetchContent_Populate(pico_sensors)
            set(PICO_SENSORS_PATH ${pico_sensors_SOURCE_DIR})
        endif ()
        set(FETCHCONTENT_BASE_DIR ${FETCHCONTENT_BASE_DIR_SAVE})
    else ()
        if (PICO_SDK_PATH AND EXISTS "${PICO_SDK_PATH}/../pico-sensors")
            set(PICO_SENSORS_PATH ${PICO_SDK_PATH}/../pico-sensors)
            message("Defaulting PICO_SENSORS_PATH as sibling of PICO_SDK_PATH: ${PICO_SENSORS_PATH}")
        else()
            message(FATAL_ERROR
                    "PICO SSENSORS location was not specified. Please set PICO_SENSORS_PATH or set PICO_SENSORS_FETCH_FROM_GIT to on to fetch from git."
                    )
        endif()
    endif ()
endif ()

set(PICO_SENSORS_PATH "${PICO_SENSORS_PATH}" CACHE PATH "Path to the PICO SENSORS")
set(PICO_SENSORS_FETCH_FROM_GIT "${PICO_SENSORS_FETCH_FROM_GIT}" CACHE BOOL "Set to ON to fetch copy of PICO SENSORS from git if not otherwise locatable")
set(PICO_SENSORS_FETCH_FROM_GIT_PATH "${PICO_SENSORS_FETCH_FROM_GIT_PATH}" CACHE FILEPATH "location to download SENSORS")

get_filename_component(PICO_SENSORS_PATH "${PICO_SENSORS_PATH}" REALPATH BASE_DIR "${CMAKE_BINARY_DIR}")
if (NOT EXISTS ${PICO_SENSORS_PATH})
    message(FATAL_ERROR "Directory '${PICO_SENSORS_PATH}' not found")
endif ()

set(PICO_SENSORS_PATH ${PICO_SENSORS_PATH} CACHE PATH "Path to the PICO SENSORS" FORCE)

add_subdirectory(${PICO_SENSORS_PATH} pico_sensors)