/**
 * @file relay.c
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
#include "relay.h"
#include "scheduler.h"
#include "int_eeprom.h"
#include "settings.h"
#include "shab_profile.h"
#include "shab.h"

/*protos*/
static void load_relay_states(s_task_handle_t me, s_task_msg_t **msg, void* arg);
static void save_relay_states(s_task_handle_t me, s_task_msg_t **msg, void* arg);
static void scan_relay_timers(s_task_handle_t me, s_task_msg_t **msg, void* arg);

/*Globals*/
const uint16_t relay_ctrl[MAXRELAYS] = {PIN_A0, PIN_A1, PIN_A2, PIN_A3, PIN_A4, PIN_A5};
s_task_handle_t load_relay_states_tsk, save_relay_states_tsk, scan_relay_timers_tsk;

/*externs*/
extern system_settings_t curr_settings;

/*Local macros*/
#define RELAY_ON(ri)    (input(relay_ctrl[ri]))
#define RELAY_OFF(ri)   (!input(relay_ctrl[ri]))

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool init_relays(void)
{
    bool ret = true;

    ret &= s_task_create(true, S_TASK_NORMAL_PRIORITY, RELAY_SEQ_DLY, load_relay_states, &load_relay_states_tsk, NULL); /*runs on demand every 0.5s*/
    ret &= s_task_create(false, S_TASK_NORMAL_PRIORITY, SAVE_TIMEOUT, save_relay_states, &save_relay_states_tsk, NULL); /*runs after SAVE_TIMEOUT ms from trigger*/
    ret &= s_task_create(true, S_TASK_NORMAL_PRIORITY, 1000, scan_relay_timers, &scan_relay_timers_tsk, NULL);          /*runs every 1s*/

    return ret;
}

/**
 * @brief Set the relay on
 * 
 * @param rly_idx 
 * @return true 
 * @return false 
 */
bool set_relay_on(uint8_t rly_idx)
{
    if (rly_idx < MAXRELAYS)        /*valid relay?*/
    {
        if (RELAY_OFF(rly_idx)) /*is it off?*/
        {
            output_high(relay_ctrl[rly_idx]);
        }
        return true;
    }
    return false;
}

/**
 * @brief Set the relay off
 * 
 * @param rly_idx 
 * @return true 
 * @return false 
 */
bool set_relay_off(uint8_t rly_idx)
{
    if (rly_idx < MAXRELAYS)        /*valid relay?*/
    {
        if (RELAY_ON(rly_idx))  /*is it on?*/
        {
            output_low(relay_ctrl[rly_idx]);
        }
        return true;
    }
    return false;
}

/**
 * @brief 
 * 
 * @param rly_idx 
 * @return true 
 * @return false 
 */
bool toggle_relay(uint8_t rly_idx)
{
    if (RELAY_ON(rly_idx))  /*is it on?*/
    {
        return  (set_relay_off(rly_idx));   /*off*/
    }
    else
    {
        return  (set_relay_on(rly_idx));    /*on*/
    }
    return false;
}

/**
 * @brief Get the relay state
 * 
 * @param rly_idx 
 * @return uint8_t 
 */
uint8_t get_relay_state(uint8_t rly_idx)
{
    return curr_settings.dev_state[rly_idx];
}

/**
 * @brief Get the relay state buffer object
 * 
 * @param rly_idx 
 * @param shab_buff 
 * @param dest_dev 
 * @param dest_instance 
 * @return true 
 * @return false 
 */
bool get_relay_state_buffer(uint8_t rly_idx, uint8_t *shab_buff, shab_device_t dest_dev, uint8_t dest_instance)
{
    if (rly_idx < MAXRELAYS)    /*valid relay?*/
    {
        shab_buff[SHAB_MSG_LENGTH] = 10;
        shab_buff[SHAB_SOURCE_ID] = DEVICE_ID;
        shab_buff[SHAB_SOURCE_INSTANCE] = curr_settings.dev_instance;
        shab_buff[SHAB_DESTINATION_ID] = dest_dev;
        shab_buff[SHAB_DESTINATION_INSTANCE] = dest_instance;
        shab_buff[SHAB_COMMAND] = SHAB_W;
        shab_buff[SHAB_FUNCTION] = DCOC_RELAY_F;
        shab_buff[SHAB_PARAM + 0] = rly_idx;
        shab_buff[SHAB_PARAM + 1] = RELAY_ON(rly_idx);

        return true;
    }
    return false;
}

/**
 * @brief Get the relay timer
 * 
 * @param rly_idx 
 * @param shab_buff 
 * @param dest_dev 
 * @param dest_instance 
 * @return true 
 * @return false 
 */
bool get_relay_timer_buffer(uint8_t rly_idx, uint8_t *shab_buff, shab_device_t dest_dev, uint8_t dest_instance)
{
    
    return false;
}

/**
 * @brief Set the relay timer 
 * 
 * @param rly_idx 
 * @param shab_msg 
 * @return true 
 * @return false 
 */
bool set_relay_timer(uint8_t rly_idx, uint8_t *shab_msg)
{
    
    return false;
}

/**
 * @brief Get the relay count down 
 * 
 * @param rly_idx 
 * @param shab_buff 
 * @param dest_dev 
 * @param dest_instance 
 * @return true 
 * @return false 
 */
bool get_relay_count_down_buffer(uint8_t rly_idx, uint8_t *shab_buff, shab_device_t dest_dev, uint8_t dest_instance)
{
    return false;
}

/**
 * @brief 
 * 
 * @param shab_buff 
 * @param dest_dev 
 * @param dest_instance 
 */
void compresss_relay_states_buffer(uint8_t *shab_buff, shab_device_t dest_dev, uint8_t dest_instance)
{
    uint8_t rly_idx;
    uint16_t r_states = 0;

    for (rly_idx = 0; rly_idx < MAXRELAYS; rly_idx++)
    {
        if (RELAY_ON(rly_idx))  /*is it on?*/
        {
            bit_set(r_states, rly_idx); /*set corresponding bit*/
        }
    }

    shab_buff[SHAB_MSG_LENGTH] = 11;
    shab_buff[SHAB_SOURCE_ID] = DEVICE_ID;
    shab_buff[SHAB_SOURCE_INSTANCE] = curr_settings.dev_instance;
    shab_buff[SHAB_DESTINATION_ID] = dest_dev;
    shab_buff[SHAB_DESTINATION_INSTANCE] = dest_instance;
    shab_buff[SHAB_COMMAND] = SHAB_W;
    shab_buff[SHAB_FUNCTION] = DCOC_RELAY_F;
    shab_buff[SHAB_PARAM + 0] = MAXRELAYS;
    shab_buff[SHAB_PARAM + 1] = make8(r_states, 0); /*lsb*/
    shab_buff[SHAB_PARAM + 2] = make8(r_states, 1); /*msb*/
}

/**
 * @brief 
 * 
 * @param me 
 * @param msg 
 * @param arg 
 */
static void load_relay_states(s_task_handle_t me, s_task_msg_t **msg, void* arg)
{
    static uint8_t relay_idx = 0;

    /*load next state:*/
    if (curr_settings.dev_state[relay_idx]) /*was it on?*/
    {
        set_relay_on(relay_idx);            /*turn it on!*/
        s_task_resume(me, false);           /*delay next check*/
    }
    else
    {
        s_task_resume(me, true);            /*check next on next tick*/
    }

    if (++relay_idx == MAXRELAYS)     /*done?*/
    {
        s_task_suspend(me);     /*Pause*/    
    }
}

/**
 * @brief 
 * 
 * @param me 
 * @param msg 
 * @param arg 
 */
static void save_relay_states(s_task_handle_t me, s_task_msg_t **msg, void* arg)
{
    /*Compare and save*/
    curr_settings.checksum = ee_calculate_checksum((uint8_t*)&curr_settings, sizeof(system_settings_t));
    int_ee_compare_write_buff((uint8_t*)&curr_settings, EE_SETTINGS_OFFSET, sizeof(system_settings_t));

    s_task_suspend(me);     /*Pause*/
}

/**
 * @brief 
 * 
 * @param me 
 * @param msg 
 * @param arg 
 */
static void scan_relay_timers(s_task_handle_t me, s_task_msg_t **msg, void* arg)
{
#if 0
    uint8_t relay_idx;
    uint16_t *timer;
    uint8_t brdcst_msg[10];

    for (relay_idx = 0; relay_idx < MAXRELAYS; relay_idx++) /*scan relays*/
    {
        timer = &(relay_count_down[relay_idx]);
        if (*timer)    /*is it timed?*/
        {
            if (--(*timer) == 0)    /*time out?*/
            {
                set_relay_off(relay_idx);   /*turn it off*/
                //Broadcast relay state:
                get_relay_state_buffer(relay_idx, brdcst_msg, make8(BROADCAST_ADDRESS, 1), make8(BROADCAST_ADDRESS, 0));
                shab_send_msg(brdcst_msg);  /*Send it*/
            }
        }
    }
#endif
}

#if 0

bool set_relays_all_off(void) //return true if at least 1 relay was set off
{
    uint8_t i;
    bool ret, ret_c = 0;
    
    for (i=0; i<MAXRELAYS; i++)
    {
        ret = set_relay_off(i);
        if (ret)
        {
           restart_wdt();   //reset WDT
           delay_ms(RELAY_DELAY);
        }
        ret_c |= ret;
    }
    
    return ret_c;  //true if at least one relay was set off
}
///////////////////////////////////////////////////////////////////////////////
unsigned uint8_t compress_relay_states(void)
{
    unsigned uint8_t s=0, i;

    for (i=0; i<MAXRELAYS; i++)
    {
        if (relay_state[i])
        {
            bit_set(s, i); //set corresponding bit
        }
    }

    return s;
}
///////////////////////////////////////////////////////////////////////////////
//bool update_relays(uint8_t *RS)
//{
//    uint8_t i;
//
//    for (i=0; i<MAXRELAYS; i++)
//    {
//        if (RS[i] == 1)
//        {
//            if (set_relay_on(i))
//            {
//                restart_wdt();
//                delay_ms(RELAY_DELAY);
//            }
//            //else
//                //return false;
//        }
//        else
//        {
//            if (set_relay_off(i))
//            {
//                restart_wdt();
//                delay_ms(RELAY_DELAY);
//            }
//            //else
//                //return false;
//        }
//    }
//
//    return true;
//}
bool update_relays(uint8_t RS)     //RS: relays mask
{
    uint8_t i;
    bool ret = false;

    for (i=0; i<MAXRELAYS; i++)
    {
        if (bit_test(RS, i))
        {
            if (set_relay_on(i))
            {
                ret = true;
                restart_wdt();
                delay_ms(RELAY_DELAY);
            }
        }
        else
        {
            if (set_relay_off(i))
            {
                ret = true;
                restart_wdt();
                delay_ms(RELAY_DELAY);
            }
        }
    }

    return ret;
}
///////////////////////////////////////////////////////////////////////////////
bool set_relays(void)
{
    uint8_t i;
    bool ret = true;

    for (i=0; i<MAXRELAYS; i++)
    {
        if (relay_state[i] == 1)
        {
            ret &= set_relay_on(i);
            delay_ms(RELAY_DELAY);
        }
        else
            relay_state[i] = 0;   //reset
    }

    return ret;
}
///////////////////////////////////////////////////////////////////////////////
bool update_relays_off(uint8_t mask)
{
    uint8_t i;
    bool ret = false;

    for (i=0; i<MAXRELAYS; i++)
    {
        if (bit_test(mask, i))
        {
            if (set_relay_off(i))
            {
                ret = true;
                restart_wdt();
                delay_ms(RELAY_DELAY);
            }
        }
    }

    return ret;
}
///////////////////////////////////////////////////////////////////////////////
bool update_relays_on(uint8_t mask)
{
    uint8_t i;
    bool ret = false;

    for (i=0; i<MAXRELAYS; i++)
    {
        if (bit_test(mask, i))
        {
            if (set_relay_on(i))
            {
                ret = true;
                restart_wdt();
                delay_ms(RELAY_DELAY);
            }
        }
    }

    return ret;
}
///////////////////////////////////////////////////////////////////////////////
#endif
