cmake_minimum_required(VERSION 3.13)

# create a sensor_NAME_headers target (see pico_pico_simple_sensor_headers_target)
# create a sensor_NAME target (see pico_pico_simple_sensor_target)
macro(pico_simple_sensor_target NAME)
    pico_simple_sensor_headers_target(${NAME})
    pico_simple_sensor_impl_target(${NAME})
endmacro()

# create an INTERFACE library named target, and define LIB_TARGET=1 (upper case) as a compile option
function(pico_add_impl_library target)
    add_library(${target} INTERFACE)
    string(TOUPPER ${target} TARGET_UPPER)
    target_compile_definitions(${target} INTERFACE LIB_${TARGET_UPPER}=1)
endfunction()

# create an INTERFACE library named sensor_NAME_headers INTERFACE library if it doesn't already exist,
#        and add include/ relative to the calling directory to the includes.
#        and sensor_structs and sensor_claim as dependencies of the library
macro(pico_simple_sensor_headers_target NAME)
    if (NOT TARGET sensor_${NAME}_headers)
        add_library(sensor_${NAME}_headers INTERFACE)

        target_include_directories(sensor_${NAME}_headers INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
        target_link_libraries(sensor_${NAME}_headers INTERFACE pico_base_headers)
        if (NOT PICO_NO_HARDWARE)
            target_link_libraries(sensor_${NAME}_headers INTERFACE sensor_structs sensor_claim)
        endif()
    endif()
endmacro()

# create an INTERFACE library named sensor_NAME if it doesn't exist, along with a sensor_NAME_headers
#        INTERFACE library that it depends on. The sensor_NAME_headers library add include/ relative to
#        and pico_base_headers, and harddware_structs as a dependency of the library
macro(pico_simple_sensor_headers_only_target NAME)
    if (NOT TARGET sensor_${NAME})
        # Choosing not to add LIB_SENSOR_ defines to avoid command line bloat pending a need (they aren't
        #   super interesting except to determine functionality as they are mostly passive accessors, however
        #   they could be useful to determine if the header is available.
        # pico_add_sdk_impl_library(sensor_${NAME})
        add_library(sensor_${NAME} INTERFACE)

        target_include_directories(sensor_${NAME} INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
        target_link_libraries(sensor_${NAME} INTERFACE pico_base_headers)
        if (NOT PICO_NO_HARDWARE)
            target_link_libraries(sensor_${NAME} INTERFACE sensor_structs)
        endif()
    endif()
endmacro()

# create an INTERFACE library named sensor_NAME if it doesn't exist, dependent on a pre-existing  sensor_NAME_headers
#        INTERFACE library and pico_platform. The file NAME.c relative to the caller is added to the C sources for the sensor_NAME
macro(pico_simple_sensor_impl_target NAME)
    if (NOT TARGET sensor_${NAME})
        # Choosing not to add LIB_SENSOR_ defines to avoid command line bloat pending a need (they aren't
        #   super interesting except to determine functionality as they are mostly passive accessors, however
        #   they could be useful to determine if the header is available.
        # pico_add_sdk_impl_library(sensor_${NAME})
        add_library(sensor_${NAME} INTERFACE)

        target_sources(sensor_${NAME} INTERFACE
                ${CMAKE_CURRENT_LIST_DIR}/${NAME}.cpp
                )

        target_link_libraries(sensor_${NAME} INTERFACE sensor_${NAME}_headers pico_platform)
    endif()
endmacro()


if (NOT TARGET _pico_sensors_inclusion_marker)


    add_library(_pico_sensors_inclusion_marker INTERFACE)

    # Pull in PICO SDK (must be before project)
    include(pico_sdk_import.cmake)
    if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
        message(FATAL_ERROR "Require at least Raspberry Pi Pico SDK version 1.3.0")
    endif()

    project(pico_sensors C CXX)

    set(CMAKE_C_STANDARD 11)
    set(CMAKE_CXX_STANDARD 17)

    pico_is_top_level_project(PICO_SENSORS_TOP_LEVEL_PROJECT)

    if (NOT PICO_SENSORS_PATH)
        set(PICO_SENSORS_PATH ${CMAKE_CURRENT_LIST_DIR})
    endif()
    set(PICO_SENSORS_PATH "${PICO_SENSORS_PATH}" CACHE PATH "Path to Pico Sensors")

    # The real work gets done in post_init which is called at the end of pico_sdk_init
    list(APPEND PICO_SDK_POST_LIST_FILES ${CMAKE_CURRENT_LIST_DIR}/post_init.cmake)
    if (PICO_SENSORS_TOP_LEVEL_PROJECT)
        message("pico_sensors: initialize SDK since we're the top-level")
        # Initialize the SDK
        pico_sdk_init()
    else()
        set(PICO_SDK_POST_LIST_FILES ${PICO_SDK_POST_LIST_FILES} PARENT_SCOPE)
    endif()
endif()