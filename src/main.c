
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// TivaWare include
#include "driverlib/debug.h"
#include "driverlib/inc/hw_nvic.h"
#include "driverlib/inc/hw_types.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"

#include "drivers/pinout.h"
#include "utils/uartstdio.h"

// FreeRTOS includes
#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/FreeRTOSConfig.h"
#include "FreeRTOS/queue.h"
#include "FreeRTOS/semphr.h"
#include "FreeRTOS/task.h"


//*****************************************************************************
//
// System clock rate in Hz.
//
//*****************************************************************************
#define SYSTEM_CLOCK    (120000000U)

int32_t g_ui32SysClock;

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void __error__(char* pcFilename, uint32_t ui32Line) {
    // Place a breakpoint here to capture errors until logging routine is finished
    while (1) {
    }
}
#endif


/**
 * Task1
 */
void blink(void* pvParameters) {
    for (;;) {
        LEDWrite(0x0F, 0x01);
        vTaskDelay(1000);
        LEDWrite(0x0F, 0x00);
        vTaskDelay(1000);
    }
}


/**
 * Task2
 */
void helloWorld(void* pvParameters) {
    for (;;) {
        UARTprintf("\nHello World!\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

// Main function
int main(void) {
    // Initialize system clock to 120 MHz
    g_ui32SysClock = MAP_SysCtlClockFreqSet(
        (SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
        SYSTEM_CLOCK);

    ASSERT(output_clock_rate_hz == SYSTEM_CLOCK);

    // Initialize the GPIO pins for the Launchpad
    PinoutSet(false, false);

    // Set up the UART which is connected to the virtual COM port
    UARTStdioConfig(0, 115200, SYSTEM_CLOCK);

    //
    // Make sure the main oscillator is enabled because this is required by
    // the PHY.  The system must have a 25MHz crystal attached to the OSC
    // pins.  The SYSCTL_MOSC_HIGHFREQ parameter is used when the crystal
    // frequency is 10MHz or higher.
    //
    SysCtlMOSCConfigSet(SYSCTL_MOSC_HIGHFREQ);


    /* if (lwIPTaskInit() != 0) { */
    /*     while (1) { */
    /*     } */
    /* } */

    // Create demo tasks
    xTaskCreate(blink, "Blink", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    xTaskCreate(helloWorld, "Hello", configMINIMAL_STACK_SIZE, NULL, 1, NULL);


    /* xTaskCreate(echoServer, (const portCHAR *) "Echo", */
    /* 		configMINIMAL_STACK_SIZE, NULL, 1, NULL); */

    vTaskStartScheduler();
    return 0;
}
