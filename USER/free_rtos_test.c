#include "free_rtos_test.h"
#include "FreeRTOS.h"
#include "task.h"
#include "drive.h"

TaskHandle_t t1, t2, t3, t4, t0;
TaskHandle_t task_drive;

void LED0_task(void *p)
{
    
    while(1)
    {
        GPIO_ResetBits(GPIOC, LED3);
        vTaskDelay(80);
        GPIO_SetBits(GPIOC, LED3);
        vTaskDelay(920);
    }
}
void LED1_task(void *p)
{
    while(1)
    {
        GPIO_ResetBits(GPIOC, LED1);
        vTaskDelay(80);
        GPIO_SetBits(GPIOC, LED1);
        vTaskDelay(920);	      
    }
}

void Print_task(void *p)
{
    
    while(1)
    {
        free_rtos_printf("main_\r\n");
        vTaskDelay(1000);
    }
    
}

void Float_task(void *p)
{
    static float x = 0.0f;
    while(1)
    {
            x += 0.1f;
            free_rtos_printf("%f", x);
            vTaskDelay(1000);
    }
}

void Chassis_task(void *p)
{
    
    while(1)
    {
        delay(5);
        ChassisTask();
    }
}

void Start_task(void *p)
{
    taskENTER_CRITICAL();
    xTaskCreate((TaskFunction_t)LED0_task,   "led1", 50, (void*)NULL,(UBaseType_t)2, (TaskHandle_t *)&t1);
    xTaskCreate((TaskFunction_t)Print_task, "print", 180,(void*)NULL,(UBaseType_t)4, (TaskHandle_t *)&t3);
    
   // xTaskCreate((TaskFunction_t)LED1_task,   "led1", 50, (void*)NULL,(UBaseType_t)3, (TaskHandle_t *)&t2);   
   // xTaskCreate((TaskFunction_t)Float_task,  "foat", 200,(void*)NULL,(UBaseType_t)4, (TaskHandle_t *)&t4);
   // vTaskDelete(NULL);
    
    xTaskCreate((TaskFunction_t)Chassis_task, "chassis", 50, (void*)NULL,(UBaseType_t)10, (TaskHandle_t *)&task_drive);
    taskEXIT_CRITICAL();
}


void free_rtos(void)
{       
    
    xTaskCreate((TaskFunction_t)Start_task, "start", 200,  NULL, 1, (TaskHandle_t *)&t0);
    vTaskStartScheduler();
}












