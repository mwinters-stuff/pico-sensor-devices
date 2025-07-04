
add_library(pico_sensors_included INTERFACE)
target_compile_definitions(pico_sensors_included INTERFACE
        -DPICO_SENSORS=1
        )

pico_add_platform_library(pico_sensors_included)

# note as we're a .cmake included by the SDK, we're relative to the pico-sdk build
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src ${CMAKE_BINARY_DIR}/pico_sensors/src)

if (PICO_SENSORS_EXAMPLES_ENABLED OR PICO_SENSORS_TOP_LEVEL_PROJECT)  
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/examples {CMAKE_BINARY_DIR}/pico_sensors/examples)
endif ()

