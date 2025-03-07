#ifndef OKMP_FREERTOS_H
#define OKMP_FREERTOS_H

#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include "task.h"

#include "input.h"
#include "i2c.h"
#include "is31fl3731.h"
#include "led.h"
#include "display.h"

// Task IDs
extern osThreadId_t led_task_id;
extern osThreadId_t input_task_id;
extern osThreadId_t display_task_id;

// Queue IDs
extern osMessageQueueId_t led_queue_id;
extern osMessageQueueId_t display_queue_id;

void freertos_init();

#endif