# pico_simple_sensor_target(AT24Cx)
# target_link_libraries(sensor_AT24Cx INTERFACE sensor_Wire)


if (NOT TARGET sensor_AT24Cx)
    add_library(sensor_AT24Cx INTERFACE)
    
    # pico_generate_pio_header(pico_sd_card ${CMAKE_CURRENT_LIST_DIR}/sd_card.pio)
    
    target_sources(sensor_AT24Cx INTERFACE
            ${CMAKE_CURRENT_LIST_DIR}/AT24Cx.cpp
    )
    
    target_include_directories(sensor_AT24Cx INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
    target_link_libraries(sensor_AT24Cx INTERFACE sensor_Wire)
endif()