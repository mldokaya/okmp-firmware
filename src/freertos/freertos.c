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
osMessageQueueId_t queue_ids[2];
void StartDefaultTask(void *argument){
    (void)argument;
    tusb_rhport_init_t dev_init = {
     .role = TUSB_ROLE_DEVICE,
     .speed = TUSB_SPEED_AUTO
    };
    tusb_init(0, &dev_init); // initialize device stack on roothub port 0
    /* init code for USB_DEVICE */
    // MX_USB_DEVICE_Init();
    /* USER CODE BEGIN StartDefaultTask */
    /* Infinite loop */
    // osDelay(1000);
    // issi_set_func_display(&issi, ISSI_DISPLAY_IC_FRAME1, ISSI_DISPLAY_BLINK_DISABLE, 2, true);
    // printf("Should be off now\n");
    for(;;)
    {
        tud_task();
    }
  /* USER CODE END StartDefaultTask */
}

void freertos_init(){
    // MX_USB_DEVICE_Init();
    

  // If freeRTOS is used, IRQ priority is limit by max syscall ( smaller is higher )
  NVIC_SetPriority(OTG_FS_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY );


    led_queue_id = osMessageQueueNew(30, sizeof(encoder_event), NULL);
    display_queue_id = osMessageQueueNew(30, sizeof(display_event), NULL);
    queue_ids[0] = led_queue_id;
    queue_ids[1] = display_queue_id;

  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, NULL);
    input_task_id = osThreadNew(task_input_update, (void *)&led_queue_id, NULL);
    led_task_id = osThreadNew(led_task, (void *)queue_ids, NULL);
    display_task_id = osThreadNew(display_task, (void *)&display_queue_id, &output_task_attributes);
}