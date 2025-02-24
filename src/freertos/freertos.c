#include "freertos.h"
#include "tusb.h"

const osThreadAttr_t output_task_attributes = {
    .name = "Output_Task",
    .stack_size = 128 * 24,
    .priority = (osPriority_t) osPriorityNormal
};

osThreadId_t defaultTaskHandle;
// Task IDs
osThreadId_t led_task_id;
osThreadId_t input_task_id;
osThreadId_t display_task_id;

// Queue IDs
osMessageQueueId_t led_queue_id;
osMessageQueueId_t display_queue_id;

void StartDefaultTask(void *argument){
    (void)argument;
    tusb_rhport_init_t dev_init = {
     .role = TUSB_ROLE_DEVICE,
     .speed = TUSB_SPEED_AUTO
    };
    tusb_init(0, &dev_init); // initialize device stack on roothub port 0
    while(0){
        tud_task();
    }
}

void freertos_init(){
  // If freeRTOS is used, IRQ priority is limit by max syscall ( smaller is higher )
    NVIC_SetPriority(OTG_FS_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY );

    led_queue_id = osMessageQueueNew(30, sizeof(encoder_event), NULL);
    display_queue_id = osMessageQueueNew(30, sizeof(display_event), NULL);

    defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, NULL);
    input_task_id = osThreadNew(task_input_update, (void *)&led_queue_id, NULL);
    led_task_id = osThreadNew(led_task, NULL, NULL);
    display_task_id = osThreadNew(display_task, (void *)&display_queue_id, &output_task_attributes);
}