if (PICO_ON_DEVICE)
    if (TARGET sensor_BH1750)

        add_executable(BH1750_example
                main.cpp
                )

        target_link_libraries(BH1750_example pico_stdlib 
          hardware_i2c
          sensor_BH1750)
        pico_add_extra_outputs(BH1750_example)
    endif()
endif()