//*****************************************************************************
//
// lwip_task.c - Tasks to serve web pages over Ethernet using lwIP.
//
// Copyright (c) 2009-2016 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.1.3.156 of the DK-TM4C129X Firmware Package.
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_types.h"
#include "driverlib/rom.h"
#include "utils/lwiplib.h"
#include "utils/ustdlib.h"
#include "lwip_task.h"



extern uint32_t g_ui32SysClock;



//*****************************************************************************
//
// Sets up the additional lwIP raw API services provided by the application.
//
//*****************************************************************************
void
SetupServices(void *pvArg)
{
	int a = 2;
	a += *(int*)pvArg;
}

//*****************************************************************************
//
// Initializes the lwIP tasks.
//
//*****************************************************************************
uint32_t
lwIPTaskInit(void)
{
    uint32_t ui32User0, ui32User1;
    uint8_t pui8MAC[6];

    //
    // Get the MAC address from the user registers.
    //
    ROM_FlashUserGet(&ui32User0, &ui32User1);
    if((ui32User0 == 0xffffffff) || (ui32User1 == 0xffffffff))
    {
        return(1);
    }

    //
    // Convert the 24/24 split MAC address from NV ram into a 32/16 split MAC
    // address needed to program the hardware registers, then program the MAC
    // address into the Ethernet Controller registers.
    //
    pui8MAC[0] = ((ui32User0 >>  0) & 0xff);
    pui8MAC[1] = ((ui32User0 >>  8) & 0xff);
    pui8MAC[2] = ((ui32User0 >> 16) & 0xff);
    pui8MAC[3] = ((ui32User1 >>  0) & 0xff);
    pui8MAC[4] = ((ui32User1 >>  8) & 0xff);
    pui8MAC[5] = ((ui32User1 >> 16) & 0xff);

	//
	// Lower the priority of the Ethernet interrupt handler.  This is required
	// so that the interrupt handler can safely call the interrupt-safe
	// FreeRTOS functions (specifically to send messages to the queue).
	//
	ROM_IntPrioritySet(INT_EMAC0, 0xC0);

	//
	// Initialize lwIP.
	//
	uint32_t ip_addr = (192ul << 24) | (168ul << 16) | (0ul << 8) | 10;

	uint32_t gw_addr = (192ul << 24) | (168ul << 16) | (0ul << 8) | 1;

	uint32_t netmask = (255ul << 24) | (255ul << 16) | (255ul << 8);

	// Initialize lwIP.
	//
	lwIPInit(g_ui32SysClock, pui8MAC, 0, 0, 0, IPADDR_USE_DHCP);

//	lwIPInit(g_ui32SysClock, pui8MAC, ip_addr, gw_addr, netmask,
//		IPADDR_USE_STATIC);

	tcpip_callback(SetupServices, 0);
    //
    // Success.
    //
    return(0);
}
