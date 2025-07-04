if (PICO_ON_DEVICE)
    if (TARGET sensor_INA226)

        add_executable(INA226_example
                main.cpp
                )

        target_link_libraries(INA226_example pico_stdlib 
          hardware_i2c
          sensor_INA226)
        pico_add_extra_outputs(INA226_example)
    endif()
endif()