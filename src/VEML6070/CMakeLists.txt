# pico_simple_sensor_target(VEML6070)
# target_link_libraries(sensor_VEML6070 INTERFACE sensor_Wire)

if (NOT TARGET sensor_VEML6070)
    add_library(sensor_VEML6070 INTERFACE)
    
    # pico_generate_pio_header(pico_sd_card ${CMAKE_CURRENT_LIST_DIR}/sd_card.pio)
    
    target_sources(sensor_VEML6070 INTERFACE
            ${CMAKE_CURRENT_LIST_DIR}/VEML6070.cpp
    )
    
    target_include_directories(sensor_VEML6070 INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
    target_link_libraries(sensor_VEML6070 INTERFACE sensor_Wire)
endif()