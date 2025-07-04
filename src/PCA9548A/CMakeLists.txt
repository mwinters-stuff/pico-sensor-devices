# pico_simple_sensor_target(PCA9548A)
# target_link_libraries(sensor_PCA9548A INTERFACE sensor_Wire)


if (NOT TARGET sensor_PCA9548A)
    add_library(sensor_PCA9548A INTERFACE)
    
    # pico_generate_pio_header(pico_sd_card ${CMAKE_CURRENT_LIST_DIR}/sd_card.pio)
    
    target_sources(sensor_PCA9548A INTERFACE
            ${CMAKE_CURRENT_LIST_DIR}/PCA9548A.cpp
    )
    
    target_include_directories(sensor_PCA9548A INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
    target_link_libraries(sensor_PCA9548A INTERFACE sensor_Wire)
endif()