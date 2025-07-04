if (PICO_ON_DEVICE)
    if (TARGET sensor_DHT22)

        add_executable(DHT22_example
                main.cpp
                )

        target_link_libraries(DHT22_example pico_stdlib 
          hardware_i2c
          sensor_DHT22)
        pico_add_extra_outputs(DHT22_example)
    endif()
endif()