if (PICO_ON_DEVICE)
    if (TARGET sensor_AT24Cx)

        add_executable(AT24Cx_example
                main.cpp
                )

        target_link_libraries(AT24Cx_example pico_stdlib 
          hardware_i2c
          sensor_AT24Cx)
        pico_add_extra_outputs(AT24Cx_example)
    endif()
endif()