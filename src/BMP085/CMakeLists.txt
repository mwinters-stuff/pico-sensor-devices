# pico_simple_sensor_target(BMP085)
# target_link_libraries(sensor_BMP085 INTERFACE sensor_Wire)

if (NOT TARGET sensor_BMP085)
    add_library(sensor_BMP085 INTERFACE)
    
    # pico_generate_pio_header(pico_sd_card ${CMAKE_CURRENT_LIST_DIR}/sd_card.pio)
    
    target_sources(sensor_BMP085 INTERFACE
            ${CMAKE_CURRENT_LIST_DIR}/BMP085.cpp
    )
    
    target_include_directories(sensor_BMP085 INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
    target_link_libraries(sensor_BMP085 INTERFACE sensor_Wire)
endif()