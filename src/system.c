/**
 * @file system.c
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
#include "system.h"
#include "scheduler.h"


//Global vars:
cntr_size_t sys_time_up = 0;
system_info  current_system_info;

/*Protos:*/
static void update_sys_up_time(s_task_handle_t me, s_task_msg_t **msg, void* arg);
static void update_system(s_task_handle_t me, s_task_msg_t **msg, void* arg);

/**
 * 
 * @return 
 */
bool init_system(void)      //returns TRUE if success
{
   bool ret = true;
#if (defined (__PCH__) || defined (__PCD__))	/*CCS C compiler?*/
   enable_interrupts(INT_TIMER1);   /*Enable TIMER1 interrupt*/
   enable_interrupts(INT_RDA);      /*Enable Serial Port interrupt*/
   enable_interrupts(GLOBAL);       /*Enable global interrupt*/
#else
   /*TODO: system init*/
#endif

   /*Create related task(s)*/
   ret &= s_task_create(true, S_TASK_NORMAL_PRIORITY, 1000, update_sys_up_time, NULL, NULL); /*runs every 1s*/
   ret &= s_task_create(true, S_TASK_LOW_PRIORITY, 1000, update_system, NULL, NULL);         /*runs every 1000ms*/

   return ret;
}
/**
 * 
 */
void init_system_info(void)
{
   //if any
}

/**
 * 
 */
void fill_system_params(void)
{
   //if any
}


/**
 * 
 * @param me
 * @param msg
 */
void update_sys_up_time(s_task_handle_t me, s_task_msg_t **msg, void* arg)
{
   sys_time_up++;

   //reg_p0x00_write_adhoc(P0_MSC_UPTM0, (uint8_t*)(&sys_time_up), sizeof(cntr_size_t));

#ifdef USE_MESSAGING    
   //A GOOD PRACTICE IS TO ALWAYS FLUSH MESSAGES BEFORE EXIT:
   s_task_flush_msgs(msg);
#endif
}

/**
 * 
 * @param me
 * @param msg
 */
void update_system(s_task_handle_t me, s_task_msg_t **msg, void* arg)
{
   

#ifdef USE_MESSAGING    
   //A GOOD PRACTICE IS TO ALWAYS FLUSH MESSAGES BEFORE EXIT:
   s_task_flush_msgs(msg);
#endif
}
