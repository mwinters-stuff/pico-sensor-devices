  if (PICO_ON_DEVICE)
    if (TARGET sensor_PCF8575)

        add_executable(PCF8575_example
                main.cpp
                )

        target_link_libraries(PCF8575_example pico_stdlib 
          hardware_i2c
          sensor_PCF8575)
        pico_add_extra_outputs(PCF8575_example)
    endif()
endif()