# pico_simple_sensor_target(BH1750)
# target_link_libraries(sensor_BH1750 INTERFACE sensor_Wire)


if (NOT TARGET sensor_BH1750)
    add_library(sensor_BH1750 INTERFACE)
    
    # pico_generate_pio_header(pico_sd_card ${CMAKE_CURRENT_LIST_DIR}/sd_card.pio)
    
    target_sources(sensor_BH1750 INTERFACE
            ${CMAKE_CURRENT_LIST_DIR}/BH1750.cpp
    )
    
    target_include_directories(sensor_BH1750 INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
    target_link_libraries(sensor_BH1750 INTERFACE sensor_Wire)
endif()