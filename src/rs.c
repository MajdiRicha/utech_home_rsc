/**
 * @file rs.c
 * @author Majdi Richa (majdi.richa@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-10-16
 * 
 * @copyright
 * 2025 utech Lebanon ALL RIGHTS RESERVED.
 * 
 */

#include "main.h"
#include "rs.h"
#include "shab_profile.h"
#include "settings.h"
#include "scheduler.h"

//Ptotos:
static void rs_monitor(s_task_handle_t me, s_task_msg_t **msg, void* arg);

/*externs*/
extern system_settings_t curr_settings;

static rsc_rs_param_t rs_states[MAXCHANNELS]={RSC_RS_STOP, RSC_RS_STOP, RSC_RS_STOP};
static const uint8_t rs_relay_channels[MAXCHANNELS][2] = //Up/Down
{
   {0,1},
   {2,3},
   {4,5}
};

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool init_rs(void)
{
    return s_task_create(true, S_TASK_NORMAL_PRIORITY, 100, rs_monitor, NULL, NULL);    //runs every 100 ms
}

/**
 * @brief Set the rs up
 * 
 * @param rs 
 * @return true 
 * @return false 
 */
bool set_rs_up(uint8_t rs)
{
    bool ret = true;

    if (rs_states[rs] != RSC_RS_UP)
    {
        if (get_relay_state(rs_relay_channels[rs][1]) == 1) //On?
        {
            ret = set_relay_off(rs_relay_channels[rs][1]);  //turn it off
        }
        if (ret)
        {
            ret = set_relay_on(rs_relay_channels[rs][0]);
            rs_states[rs] = RSC_RS_UP;
        }
    }

    return ret;
}

/**
 * @brief Set the rs down
 * 
 * @param rs 
 * @return true 
 * @return false 
 */
bool set_rs_down(uint8_t rs)
{
    bool ret = true;

    if (rs_states[rs] != RSC_RS_DOWN)
    {
        if (get_relay_state(rs_relay_channels[rs][0]) == 1) //On?
        {
            ret = set_relay_off(rs_relay_channels[rs][0]);  //turn it off
        }
        if (ret)
        {
            ret = set_relay_on(rs_relay_channels[rs][1]);
            rs_states[rs] = RSC_RS_DOWN;
        }
    }

    return ret;
}

/**
 * @brief Set the rs stop
 * 
 * @param rs 
 * @return true 
 * @return false 
 */
bool set_rs_stop(uint8_t rs)
{
    if (rs_states[rs] != RSC_RS_STOP)
    {
        return ((set_relay_off(rs_relay_channels[rs][0])) && (set_relay_off(rs_relay_channels[rs][1])));
    }

    return true;
}

/**
 * @brief Get the rs state buffer
 * 
 * @param rs_idx 
 * @param shab_buff 
 * @param dest_dev 
 * @param dest_instance 
 * @return true 
 * @return false 
 */
bool get_rs_state_buffer(uint8_t rs_idx, uint8_t *shab_buff, shab_device_t dest_dev, uint8_t dest_instance)
{
    if (rs_idx < MAXRELAYS)    /*valid relay?*/
    {
        shab_buff[SHAB_MSG_LENGTH] = 10;
        shab_buff[SHAB_SOURCE_ID] = DEVICE_ID;
        shab_buff[SHAB_SOURCE_INSTANCE] = curr_settings.dev_instance;
        shab_buff[SHAB_DESTINATION_ID] = dest_dev;
        shab_buff[SHAB_DESTINATION_INSTANCE] = dest_instance;
        shab_buff[SHAB_COMMAND] = SHAB_W;
        shab_buff[SHAB_FUNCTION] = RSC_RS_F;
        shab_buff[SHAB_PARAM + 0] = rs_idx;
        shab_buff[SHAB_PARAM + 1] = rs_states[rs_idx];

        return true;
    }
    return false;
}

/**
 * @brief 
 * 
 * @param shab_buff 
 * @param dest_dev 
 * @param dest_instance 
 */
void compresss_rs_states_buffer(uint8_t *shab_buff, shab_device_t dest_dev, uint8_t dest_instance)
{
    uint8_t rs_idx;
    uint16_t rs_sts = 0;

    for (rs_idx = 0; rs_idx < MAXCHANNELS; rs_idx++)
    {
        rs_sts <<= 2;
        rs_sts |= rs_states[MAXCHANNELS - rs_idx - 1];  //in reverse order
    }

    shab_buff[SHAB_MSG_LENGTH] = 11;
    shab_buff[SHAB_SOURCE_ID] = DEVICE_ID;
    shab_buff[SHAB_SOURCE_INSTANCE] = curr_settings.dev_instance;
    shab_buff[SHAB_DESTINATION_ID] = dest_dev;
    shab_buff[SHAB_DESTINATION_INSTANCE] = dest_instance;
    shab_buff[SHAB_COMMAND] = SHAB_W;
    shab_buff[SHAB_FUNCTION] = RSC_RS_F;
    shab_buff[SHAB_PARAM + 0] = MAXCHANNELS;
    shab_buff[SHAB_PARAM + 1] = make8(rs_sts, 0); /*lsb*/
    shab_buff[SHAB_PARAM + 2] = make8(rs_sts, 1); /*msb*/
}

/**
 * @brief 
 * 
 * @param me 
 * @param msg 
 * @param arg 
 */
static void rs_monitor(s_task_handle_t me, s_task_msg_t **msg, void* arg)
{

}
