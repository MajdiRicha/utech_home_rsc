/**
 * @file hardware.h
 * @author Majdi Richa (majdi.richa@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-10-15
 * 
 * @copyright
 * 2025 utech Lebanon ALL RIGHTS RESERVED.
 * 
 */
#ifndef  HARDWARE_H
#define  HARDWARE_H

/*LEDs*/
#define LED1pin         PIN_A7
#define LED1On()        output_high(LED1pin)
#define LED1Off()       output_low(LED1pin)
#define LED1Toggle()    output_toggle(LED1pin)
#define LED2pin         PIN_B5
#define LED2On()        output_high(LED2pin)
#define LED2Off()       output_low(LED2pin)
#define LED2Toggle()    output_toggle(LED2pin)

#define LED_ALL_ON()    {LED1On(); LED2On();}
#define LED_ALL_OFF()   {LED1Off(); LED2Off();}

#define LED1STATE()     input(LED1pin)
#define LED2STATE()     input(LED2pin)

   
///////////////////////////////////////////////////////////////////////////////
void init_io_ports(void);
bool init_hw(void);
///////////////////////////////////////////////////////////////////////////////
#endif
