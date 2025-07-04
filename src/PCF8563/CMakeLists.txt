# pico_simple_sensor_target(PCF8563)
# target_link_libraries(sensor_PCF8563 INTERFACE sensor_Wire)

if (NOT TARGET sensor_PCF8563)
    add_library(sensor_PCF8563 INTERFACE)
    
    # pico_generate_pio_header(pico_sd_card ${CMAKE_CURRENT_LIST_DIR}/sd_card.pio)
    
    target_sources(sensor_PCF8563 INTERFACE
            ${CMAKE_CURRENT_LIST_DIR}/PCF8563.cpp
    )
    
    target_include_directories(sensor_PCF8563 INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
    target_link_libraries(sensor_PCF8563 INTERFACE sensor_Wire)
endif()