/**
 * @file timer.c
 * @author Majdi Richa (majdi.richa@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-10-15
 * 
 * @copyright
 * 2025 utech Lebanon ALL RIGHTS RESERVED.
 * 
 */
#include "main.h"
#include "timer.h"


//Globals:
cntr_size_t local_sys_tick_counter  = 0; //tick counter


#if (defined (__PCH__) || defined (__PCD__))	/*CCS C compiler?*/
/**
 * ISR for TIMER1 in order to generate a 1ms local_sys_tick
 * 
 * @author m_richa 
 * @date 
 */
#int_TIMER1   //HIGH   //high priority interrupt
void TIMER1_isr(void)
{
    set_timer1(get_timer1() - TMR1Reload);  /*re-load timer*/
    
    local_sys_tick_counter++;
}
#else
    /*TODO: timer*/
#endif
/**
 * Function to return system ticks counter in ms
 * 
 * @author m_richa 
 * @date 
 * 
 * @return cntr_size_t system ticks count in ms
 */
cntr_size_t get_ticks_counter(void)
{
    return local_sys_tick_counter;
}
