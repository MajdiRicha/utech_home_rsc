/**
 * @file hardware.c
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
#include "scheduler.h"

static void heart_beat_led(s_task_handle_t me, s_task_msg_t **msg, void* arg);

/**
 * 
 */
void init_io_ports(void)
{
    set_tris_a(0x00); /*A*/
    output_a(0x00);

    set_tris_b(0x87); /*B*/
    output_b(0x00);

    set_tris_c(0x81); /*C*/
    output_c(0x00);
}
///////////////////////////////////////////////////////////////////////////////
/**
 * 
 * @return 
 */
bool init_hw(void)
{
    init_io_ports();

    //Timer1: used for scheduler
    setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);      //for 1ms interrupt

    /*Create related task(s)*/
    return s_task_create(true, S_TASK_NORMAL_PRIORITY, 100, heart_beat_led, NULL, NULL);         /*runs every 100ms*/
    return true;
}


/**
 * 
 * @param me
 * @param msg
 */
void heart_beat_led(s_task_handle_t me, s_task_msg_t **msg, void* arg)
{
   static uint8_t HeartBeatCtr = 0;

   //Heart Beat
    if (bit_test(HeartBeatCtr, 4))
    {
        HeartBeatCtr = 0;
        LED1Off();
        restart_wdt();  //reset WDT
    } else if ((!bit_test(HeartBeatCtr, 2)) && (!bit_test(HeartBeatCtr, 3)))
    {
        LED1Toggle();
        HeartBeatCtr++;
    } else
    {
        HeartBeatCtr++;
    }
    
#ifdef USE_MESSAGING    
   //A GOOD PRACTICE IS TO ALWAYS FLUSH MESSAGES BEFORE EXIT:
   s_task_flush_msgs(msg);
#endif
}

