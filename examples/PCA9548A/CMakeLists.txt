if (PICO_ON_DEVICE)
    if (TARGET sensor_PCA9548A)

        add_executable(PCA9548A_example
                main.cpp
                )

        target_link_libraries(PCA9548A_example pico_stdlib 
          hardware_i2c
          sensor_PCA9548A)
        pico_add_extra_outputs(PCA9548A_example)
    endif()
endif()