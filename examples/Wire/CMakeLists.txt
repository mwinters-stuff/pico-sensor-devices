if (PICO_ON_DEVICE)
    if (TARGET sensor_Wire)

        add_executable(Wire_example
                main.cpp
                )

        target_link_libraries(Wire_example pico_stdlib 
          hardware_i2c
          sensor_Wire)
        pico_add_extra_outputs(Wire_example)
    endif()
endif()