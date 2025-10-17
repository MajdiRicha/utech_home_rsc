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

#define RS_MON_RATE     (100)               //ms
#define RS_MON_K        (1000/RS_MON_RATE)

//Ptotos:
static void rs_monitor(s_task_handle_t me, s_task_msg_t **msg, void* arg);

/*externs*/
extern system_settings_t curr_settings;

static rsc_rs_param_t rs_states[MAXCHANNELS]={RSC_RS_STOP, RSC_RS_STOP, RSC_RS_STOP};
static const uint8_t rs_relay_channels[MAXCHANNELS][2] =    //Up/Down
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
    return s_task_create(true, S_TASK_NORMAL_PRIORITY, RS_MON_RATE, rs_monitor, NULL, NULL);    //runs every RS_MON_RATE ms
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
        rs_states[rs] = RSC_RS_UP;
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
        rs_states[rs] = RSC_RS_DOWN;
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
        rs_states[rs] = RSC_RS_STOP;
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
    static rsc_rs_param_t rs_sts_hist[MAXCHANNELS]={RSC_RS_STOP, RSC_RS_STOP, RSC_RS_STOP};
    static uint16_t rs_cnt_dwn[MAXCHANNELS];
    uint8_t ch_idx;

    for (ch_idx=0; ch_idx<MAXCHANNELS; ch_idx++)
    {
        if (rs_sts_hist[ch_idx] != rs_states[ch_idx])                   //State changed?
        {
            switch(rs_states[ch_idx])
            {
                case RSC_RS_STOP:
                    if (rs_sts_hist[ch_idx] != RSC_RS_STOP)             //was it moving?
                    {
                        //Do it:
                        set_relay_off(rs_relay_channels[ch_idx][1]);    //OFF
                        set_relay_off(rs_relay_channels[ch_idx][0]);    //OFF
                        
                        rs_sts_hist[ch_idx] = RSC_RS_STOP;              //Update history
                    }
                break;

                case RSC_RS_UP:
                    if (RSC_RS_STOP == rs_sts_hist[ch_idx])             //was it stopped?
                    {
                        //Do it:
                        set_relay_off(rs_relay_channels[ch_idx][1]);    //OFF (just in case)
                        set_relay_on(rs_relay_channels[ch_idx][0]);     //ON
                        
                        //Arm count down:
                        rs_cnt_dwn[ch_idx] = (curr_settings.rs_up_time[ch_idx] * RS_MON_K);
                        rs_sts_hist[ch_idx] = RSC_RS_UP;                //Update history
                    }
                    else                                                //was going down?
                    {
                        //Stop it:
                        set_relay_off(rs_relay_channels[ch_idx][1]);    //OFF
                        set_relay_off(rs_relay_channels[ch_idx][0]);    //OFF
                        rs_sts_hist[ch_idx] = RSC_RS_STOP;              //Force one cycle delay
                    }
                break;

                case RSC_RS_DOWN:
                    if (RSC_RS_STOP == rs_sts_hist[ch_idx])             //was it stopped?
                    {
                        //Do it:
                        set_relay_off(rs_relay_channels[ch_idx][0]);    //OFF (just in case)
                        set_relay_on(rs_relay_channels[ch_idx][1]);     //ON
                        
                        //Arm count down:
                        rs_cnt_dwn[ch_idx] = (curr_settings.rs_dn_time[ch_idx] * RS_MON_K);
                        rs_sts_hist[ch_idx] = RSC_RS_DOWN;              //Update history
                    }
                    else                                                //was going up?
                    {
                        //Stop it:
                        set_relay_off(rs_relay_channels[ch_idx][1]);    //OFF
                        set_relay_off(rs_relay_channels[ch_idx][0]);    //OFF
                        rs_sts_hist[ch_idx] = RSC_RS_STOP;              //Force one cycle delay
                    }
                break;
            }
        }
        else                                                            //No change and moving?
        {
            switch(rs_states[ch_idx])
            {
                case RSC_RS_UP:
                case RSC_RS_DOWN:
                    if (--rs_cnt_dwn[ch_idx] == 0)                      //Should stop?
                    {
                        rs_states[ch_idx] = RSC_RS_STOP;
                    }
                break;
            }
        }
    }
}
