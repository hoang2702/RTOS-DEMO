#include "stm32f4xx.h"

#include "FreeRTOS.h"
#include "task.h"


// Task priorities: Higher numbers are higher priority.
#define mainTIME_TASK_PRIORITY ( tskIDLE_PRIORITY + 4 )
#define mainTIME2_TASK_PRIORITY ( tskIDLE_PRIORITY + 3 )
#define mainMEMS_TASK_PRIORITY ( tskIDLE_PRIORITY + 2 )
#define mainDEBUG_TASK_PRIORITY ( tskIDLE_PRIORITY + 1 )
#define mainINTEGER_TASK_PRIORITY ( tskIDLE_PRIORITY )

xTaskHandle hTimeTask;
xTaskHandle hTimeTask2;
xTaskHandle hMemsTask;
xTaskHandle hDebugTask;

portTASK_FUNCTION_PROTO( vTimeTask, pvParameters );
portTASK_FUNCTION_PROTO( vTimeTask2, pvParameters );
portTASK_FUNCTION_PROTO( vMemsTask, pvParameters );
portTASK_FUNCTION_PROTO( vDebugTask, pvParameters );

uint64_t u64Ticks=0; // Counts OS ticks (default = 1000Hz).
uint64_t u64IdleTicks=0; // Value of u64IdleTicksCnt is copied once per sec.
uint64_t u64IdleTicksCnt=0; // Counts when the OS has no task to execute.
uint16_t u16PWM1=0;


// ----------------------------------------------------------------------------
void myTask1(){

    portTickType xLastWakeTime = xTaskGetTickCount();

    while(1){
        GPIO_ToggleBits(GPIOD,GPIO_Pin_13);
        vTaskDelayUntil( &xLastWakeTime, ( 500 / portTICK_RATE_MS ) );
    }
}


// ----------------------------------------------------------------------------
void myTask2(){

    portTickType xLastWakeTime = xTaskGetTickCount();

    while(1){
        GPIO_ToggleBits(GPIOD,GPIO_Pin_15);
        vTaskDelayUntil( &xLastWakeTime, ( 500 / portTICK_RATE_MS ) );
    }
}

void myTask3(){

    portTickType xLastWakeTime = xTaskGetTickCount();

    while(1){
        GPIO_ToggleBits(GPIOD,GPIO_Pin_14);
        vTaskDelayUntil( &xLastWakeTime, ( 1000 / portTICK_RATE_MS ) );
    }
}
// ----------------------------------------------------------------------------
void myTask4(){

    portTickType xLastWakeTime = xTaskGetTickCount();

    while(1){
        GPIO_ToggleBits(GPIOD,GPIO_Pin_12);
        vTaskDelayUntil( &xLastWakeTime, ( 1000 / portTICK_RATE_MS ) );
    }
}
// ----------------------------------------------------------------------------
void GPIOInit(){
    GPIO_InitTypeDef g;
    g.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15; // config pin
    g.GPIO_Speed = GPIO_Speed_100MHz; 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 
    GPIO_Init(GPIOD, &g); //
}

int main(void){
    GPIOInit(); 

   
    xTaskCreate( myTask1, (signed char *) "TIME", configMINIMAL_STACK_SIZE, 
    NULL, mainTIME_TASK_PRIORITY, &hTimeTask );

    
    xTaskCreate( myTask2, (signed char *) "TIME", configMINIMAL_STACK_SIZE, 
    NULL, mainTIME_TASK_PRIORITY, &hTimeTask );
		xTaskCreate( myTask3, (signed char *) "TIME", configMINIMAL_STACK_SIZE, 
    NULL, mainTIME_TASK_PRIORITY, &hTimeTask );
		xTaskCreate( myTask4, (signed char *) "TIME", configMINIMAL_STACK_SIZE, 
    NULL, mainTIME_TASK_PRIORITY, &hTimeTask );

    vTaskStartScheduler(); 

    // Will only get here if there was insufficient memory to create
    // the idle task.
    while(1);
}

// This FreeRTOS callback function gets called once per tick (default = 1000Hz).
// ---------------------------------------------------------------------------- 
void vApplicationTickHook( void ) {
    ++u64Ticks;
}

// This FreeRTOS call-back function gets when no other task is ready to execute.
// On a completely unloaded system this is getting called at over 2.5MHz!
// ---------------------------------------------------------------------------- 
void vApplicationIdleHook( void ) {
    ++u64IdleTicksCnt;
}

// A required FreeRTOS function.
// ---------------------------------------------------------------------------- 
void vApplicationMallocFailedHook( void ) {
    configASSERT( 0 ); // Latch on any failure / error.
}

