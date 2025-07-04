# pico_simple_sensor_target(ADS1X15)
# target_link_libraries(sensor_ADS1X15 INTERFACE sensor_Wire)

if (NOT TARGET sensor_ADS1X15)
    add_library(sensor_ADS1X15 INTERFACE)
    
    # pico_generate_pio_header(pico_sd_card ${CMAKE_CURRENT_LIST_DIR}/sd_card.pio)
    
    target_sources(sensor_ADS1X15 INTERFACE
            ${CMAKE_CURRENT_LIST_DIR}/ADS1X15.cpp
    )
    
    target_include_directories(sensor_ADS1X15 INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
    target_link_libraries(sensor_ADS1X15 INTERFACE sensor_Wire)
endif()