if (PICO_ON_DEVICE)
    if (TARGET sensor_BMP085)

        add_executable(BMP085_example
                main.cpp
                )

        target_link_libraries(BMP085_example pico_stdlib 
          hardware_i2c
          sensor_BMP085)
        pico_add_extra_outputs(BMP085_example)
    endif()
endif()