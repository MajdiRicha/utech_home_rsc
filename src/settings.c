/**
 * @file settings.c
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
#include "int_eeprom.h"
#include "settings.h"
#include "rs.h"
#include "scheduler.h"

/*Protos:*/
static bool ee_checksum_ok(uint8_t *dt_buff, uint16_t count);
static void save_default_settings(void);
static void ee_comp_write(s_task_handle_t me, s_task_msg_t **msg, void* arg);

/*Globals*/
system_settings_t curr_settings;
s_task_handle_t ee_comp_write_tsk;

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool init_settings(void)
{
    bool ret = true;

    int_ee_read_buff((uint8_t*)&curr_settings, EE_SETTINGS_OFFSET, sizeof(system_settings_t));

    /*check data validity, otherwise load default settings*/
    if ((EE_SETTINGS_VALID != curr_settings.data_valid) || !(ee_checksum_ok((uint8_t*)&curr_settings, sizeof(system_settings_t))))
    {
        save_default_settings();
    }

    ret &= s_task_create(false, S_TASK_NORMAL_PRIORITY, 1, ee_comp_write, &ee_comp_write_tsk, NULL); /*runs every 1 ms, after save settings is triggered*/

    return ret;
}

/**
 * @brief 
 * 
 */
void save_settings(void)
{
    /*add checksum:*/
    curr_settings.checksum = ee_calculate_checksum((uint8_t*)&curr_settings, sizeof(system_settings_t));
    int_ee_compare_write_buff((uint8_t*)&curr_settings, EE_SETTINGS_OFFSET, sizeof(system_settings_t));
}

/**
 * @brief 
 * 
 */
void initiate_save_settings(void)
{
     /*add checksum:*/
    curr_settings.checksum = ee_calculate_checksum((uint8_t*)&curr_settings, sizeof(system_settings_t));

    s_task_resume(ee_comp_write_tsk, false);    /*run later on...*/
}

/**
 * @brief 
 * 
 */
void save_default_settings(void)
{
    uint8_t i;

    //curr_settings.dev_instance = 0xFF;    //KEEP WHATEVER IT WAS

    for (i=0; i<MAXCHANNELS; i++)
    {
        curr_settings.dev_state[i] = RSC_RS_STOP;   //all stopped
        curr_settings.rs_up_time[i] = 60;           //60 seconds
        curr_settings.rs_dn_time[i] = 60;           //60 seconds
    }

    curr_settings.data_valid = EE_SETTINGS_VALID;
    curr_settings.checksum = ee_calculate_checksum((uint8_t*)&curr_settings, sizeof(system_settings_t));

    int_ee_compare_write_buff((uint8_t*)&curr_settings, EE_SETTINGS_OFFSET, sizeof(system_settings_t));
}

/**
 * @brief 
 * 
 * @param dt_buff 
 * @param count 
 * @return true 
 * @return false 
 */
bool ee_checksum_ok(uint8_t *dt_buff, uint16_t count)
{
    uint8_t checksum = 0;
    uint16_t i;

    for (i = 0; i < count; i++)
    {
        checksum += dt_buff[i];     /*accumulate*/
    }

    return (checksum == 0) ? true : false;
}

/**
 * @brief 
 * 
 * @param dt_buff 
 * @param count 
 * @return uint8_t 
 */
uint8_t ee_calculate_checksum(uint8_t *dt_buff, uint16_t count)
{
    uint8_t checksum = 0;
    uint16_t i;

    for (i = 0; i < count - 1; i++)
    {
        checksum += dt_buff[i];     /*accumulate*/
    }
    checksum = (~checksum) + 1;     /*2's complement*/

    return checksum;
}

/**
 * @brief 
 * 
 * @param me 
 * @param msg 
 * @param arg 
 */
static void ee_comp_write(s_task_handle_t me, s_task_msg_t **msg, void* arg)
{
    static uint16_t ee_pointer = EE_SETTINGS_OFFSET;
    static uint8_t *set_buff_ptr = (uint8_t*)&curr_settings;

    if (read_eeprom(ee_pointer) != (*set_buff_ptr)) /*no match?*/
    {
        write_eeprom(ee_pointer, (*set_buff_ptr));  /*write it*/
    }

    set_buff_ptr++;     /*move next*/

    if (++ee_pointer == sizeof(system_settings_t))   /*done?*/
    {
        s_task_suspend(me);
        ee_pointer = EE_SETTINGS_OFFSET;            /*Reset EE pointer*/
        set_buff_ptr = (uint8_t*)&curr_settings;    /*Reset settings pointer*/
    }
}
