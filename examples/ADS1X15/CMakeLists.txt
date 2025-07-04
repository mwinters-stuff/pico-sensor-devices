if (PICO_ON_DEVICE)
    if (TARGET sensor_ADS1X15)
        add_executable(ADS1X15_example
                main.cpp
                )

        target_link_libraries(ADS1X15_example pico_stdlib 
          hardware_i2c
          sensor_ADS1X15)
        pico_add_extra_outputs(ADS1X15_example)
    endif()
endif()