add_library(stm32-drivers INTERFACE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

target_compile_definitions(stm32-drivers INTERFACE 
	USE_FULL_LL_DRIVER 
	USE_HAL_DRIVER 
	STM32F411xE 
	STM32_THREAD_SAFE_STRATEGY=4
    $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(stm32-drivers INTERFACE
    st/Drivers/STM32F4xx_HAL_Driver/Inc
    st/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
    st/Drivers/CMSIS/Device/ST/STM32F4xx/Include
    st/Drivers/CMSIS/Include
    st/Drivers/CMSIS/Device/ST/STM32F4xx/Include
    FreeRTOS/Source/include
    FreeRTOS/Source/CMSIS_RTOS_V2
    FreeRTOS/Source/portable/GCC/ARM_CM4F
    printf
)

target_sources(stm32-drivers INTERFACE
    st/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_it.c
    st/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_msp.c
    st/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_timebase_tim.c
    st/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_gpio.c
    st/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd.c
    st/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd_ex.c
    st/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usb.c
    st/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rcc.c
    st/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_utils.c
    st/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_exti.c
    st/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
    st/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
    st/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
    st/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
    st/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c
    st/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
    st/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c
    st/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
    st/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
    st/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
    st/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
    st/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
    st/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c
    st/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_i2c.c
    st/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_dma.c
    st/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_spi.c
    st/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c
    st/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c
    st/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c
    st/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usart.c
    st/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c
    st/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f411xe.s
    FreeRTOS/Source/portable/MemMang/heap_4.c
    FreeRTOS/Source/croutine.c
    FreeRTOS/Source/event_groups.c
    FreeRTOS/Source/list.c
    FreeRTOS/Source/queue.c
    FreeRTOS/Source/stream_buffer.c
    FreeRTOS/Source/tasks.c
    FreeRTOS/Source/timers.c
    FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c
    FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
    printf/printf.c
)