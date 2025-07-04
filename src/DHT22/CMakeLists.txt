# pico_simple_sensor_target(DHT22)

if (NOT TARGET sensor_DHT22)
    add_library(sensor_DHT22 INTERFACE)
    
    # pico_generate_pio_header(pico_sd_card ${CMAKE_CURRENT_LIST_DIR}/sd_card.pio)
    
    target_sources(sensor_DHT22 INTERFACE
            ${CMAKE_CURRENT_LIST_DIR}/DHT22.cpp
    )
    
    target_include_directories(sensor_DHT22 INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
    # target_link_libraries(sensor_DHT22 INTERFACE sensor_Wire)
endif()