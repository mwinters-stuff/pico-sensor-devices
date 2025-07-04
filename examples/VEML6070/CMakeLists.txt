if (PICO_ON_DEVICE)
    if (TARGET sensor_VEML6070)

        add_executable(VEML6070_example
                main.cpp
                )

        target_link_libraries(VEML6070_example pico_stdlib 
          hardware_i2c
          sensor_VEML6070)
        pico_add_extra_outputs(VEML6070_example)
    endif()
endif()