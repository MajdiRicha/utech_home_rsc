/**
 * @file main.c
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
#include "hardware.h"
#include "timer.h"
#include "system.h"
#include "scheduler.h"
#include "shab.h"
#include "settings.h"
#include "rs.h"

void main()
{
   bool ret;

   ret = scheduler_init(get_ticks_counter);    /*Initialize scheduler*/

    if (true == ret)
    {
        LED_ALL_ON();               /*indicator*/

        /*Create tasks:*/
        ret &= init_hw();           /*initialize HW and launch Heartbeat task(s)*/
        ret &= init_system();       /*initialize system and create task(s)*/
        ret &= init_settings();     /*initialize settings and create task(s)*/
        ret &= init_shab();         /*initialize SHAB communication and create task(s)*/
        ret &= init_rs();           /*initialize shutters and create task(s)*/
        
        if (true == ret)    /*success?*/
        {
            LED_ALL_OFF();  /*indicator*/
            while (true)
            {
                scheduler();    /*run scheduler forever*/
            }
        }
        else
        {
            while (true);   /*stall*/
        }
    }
    else
    {
        while (true);   /*stall*/
    }
}
