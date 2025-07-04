# pico_simple_sensor_target(Wire)
# target_link_libraries(sensor_Wire INTERFACE hardware_i2c)

if (NOT TARGET sensor_Wire)
    add_library(sensor_Wire INTERFACE)
    
    # pico_generate_pio_header(pico_sd_card ${CMAKE_CURRENT_LIST_DIR}/sd_card.pio)
    
    target_sources(sensor_Wire INTERFACE
            ${CMAKE_CURRENT_LIST_DIR}/Wire.cpp
    )
    
    target_include_directories(sensor_Wire INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
    target_link_libraries(sensor_Wire INTERFACE hardware_i2c)
endif()