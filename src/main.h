/**
 * @file main.h
 * @author Majdi Richa (majdi.richa@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-10-15
 * 
 * @copyright
 * 2025 utech Lebanon ALL RIGHTS RESERVED.
 * 
 */
#include <18F26K22.h>
#device ADC=10
#device ANSI
//High priority interrupts enable
//#device HIGH_INTS=TRUE

#FUSES NOWDT               	//No Watch Dog Timer
#FUSES PUT                 	//Power Up Timer
#FUSES NOBROWNOUT          	//No brownout reset
#FUSES BORV29              	//Brownout reset at 2.85V
#FUSES TIMER3B5            	//Timer3 Clock In is on pin B5
#FUSES NOLVP                //No Low Voltage Programming
#FUSES NOPBADEN             //PORTB all digital by default

#use delay(internal=64000000)
#use rs232(baud=19200, parity=N, xmit=PIN_C6, rcv=PIN_C7, bits=8, stream=SHAB_PORT, UART1, errors)

#use fast_io(ALL)

#include "target_port.h"

//#define ENABLE_BOOTLOADER

#ifdef ENABLE_BOOTLOADER

    #if (defined (__PCH__) || defined (__PCD__))	/*CCS C compiler?*/
        #define __APP__
        #include "boot.h"
    #endif
#endif

