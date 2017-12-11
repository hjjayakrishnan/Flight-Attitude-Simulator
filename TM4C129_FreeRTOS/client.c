/*
 * client.c
 *
 *  Created on: Dec 4, 2017
 *      Author: Mounika Reddy
 */
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "system.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "inc/hw_ints.h"
#include "semphr.h"
#include "console.h"
#include "uart.h"


char buffer[100];
SemaphoreHandle_t client_mutex;

// Write text over the Stellaris debug interface UART port
void clientTask(void *pvParameters)
{
    message_t message;
    message_t *p_message;
    p_message = &message;
    //InitConsole();
    //uart_init();
    client_mutex = xSemaphoreCreateMutex();
    if(client_mutex != NULL)
    {

    }
    //ROM_UARTCharPutNonBlocking(UART3_BASE, 'a');
   // for(j =0;j<10000;j++);
    //
    // Delay for 1 millisecond.  Each SysCtlDelay is about 3 clocks.
    //
    //SysCtlDelay(g_ui32SysClock / (1000 * 3));
//    I2C_Init();
//    ROM_UARTCharPutNonBlocking(UART3_BASE, 'b');
    while(1)
    {
    if(xQueueReceive(Socket_Queue,  &p_message, (TickType_t)10000 ) == pdTRUE)
    {
        //uart_init();
//        ROM_UARTCharPutNonBlocking(UART3_BASE, 'c');
        //UARTprintf("x:%d,y:%d,z:%d,status:%d\n",(*p_message).data.x_ddot,p_message->data.y_ddot,p_message->data.z_ddot,p_message->status);
        sprintf(buffer,"D %f %f %f %f %f %f\n ",p_message->data.IMUdata.x_ddot,p_message->data.IMUdata.y_ddot,p_message->data.IMUdata.z_ddot,
                p_message->data.IMUdata.pitch_dot,p_message->data.IMUdata.roll_dot,p_message->data.IMUdata.yaw_dot);
          UARTprintf("%s",buffer);
 //       ROM_UARTCharPutNonBlocking(UART3_BASE, 'd');
       // UARTSendbytes("hey\n",4);
       if(xSemaphoreTake(client_mutex,portMAX_DELAY) != pdTRUE)
        {
            //error
        }
        UARTSendbytes(buffer,strlen(buffer) + 1);
    }
    else
        UARTprintf("Queue Rx ERROR\n");
        xSemaphoreGive(client_mutex);
    }

}



