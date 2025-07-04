if (PICO_ON_DEVICE)
    if (TARGET sensor_PCF8591)

        add_executable(PCF8591_example
                main.cpp
                )

        target_link_libraries(PCF8591_example pico_stdlib 
          hardware_i2c
          sensor_PCF8591)
        pico_add_extra_outputs(PCF8591_example)
    endif()
endif()