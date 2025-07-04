# pico_simple_sensor_target(INA226)
# target_link_libraries(sensor_INA226 INTERFACE sensor_Wire)

if (NOT TARGET sensor_INA226)
    add_library(sensor_INA226 INTERFACE)
    
    # pico_generate_pio_header(pico_sd_card ${CMAKE_CURRENT_LIST_DIR}/sd_card.pio)
    
    target_sources(sensor_INA226 INTERFACE
            ${CMAKE_CURRENT_LIST_DIR}/INA226.cpp
    )
    
    target_include_directories(sensor_INA226 INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
    target_link_libraries(sensor_INA226 INTERFACE sensor_Wire)
endif()