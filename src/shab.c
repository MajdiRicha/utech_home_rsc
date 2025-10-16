/**
 * @file shab.c
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
#include "shab.h"
#include "shab_profile.h"
#include "scheduler.h"
#include "settings.h"
#include "rs.h"
#include "version.h"

/*Protos*/
static bool shab_checksum_ok(uint8_t *buff);
static uint8_t shab_calculate_checksum(uint8_t *buff);
static void shab_send_packet(uint8_t *data);
static bool shab_rx_buffer_rdy(void);
static void initiate_shab_send(void);
static void shab_send_packet(s_task_handle_t me, s_task_msg_t **msg, void* arg);
static void shab_process_packet(s_task_handle_t me, s_task_msg_t **msg, void* arg);

/*Globals*/
uint8_t shab_tx_buffer[SHAB_MAX_BYTES];
uint8_t shab_rx_cash[SHAB_MAX_BYTES];
uint8_t shab_rx_buffer[SHAB_MAX_BYTES];
uint8_t shab_rx_cash_ptr=0;
s_task_handle_t shab_send_packet_tsk;

/*externs*/
extern system_settings_t curr_settings;

/*macros:*/
#define CURR_DEVICE_ADDRESS() (make16(DEVICE_ID, curr_settings.dev_instance))
#define INCM_DEVICE_ADDRESS() (make16(shab_rx_buffer[SHAB_DESTINATION_ID], shab_rx_buffer[SHAB_DESTINATION_INSTANCE]))


/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool init_shab(void)
{
    bool ret = true;

    ret &= s_task_create(false, S_TASK_HIGH_PRIORITY, 5, shab_send_packet, &shab_send_packet_tsk, shab_tx_buffer);
    ret &= s_task_create(true, S_TASK_HIGH_PRIORITY, 1, shab_process_packet, NULL, NULL);

    return ret;
}

/**
 * @brief 
 * 
 * @param msg 
 */
void shab_send_msg(uint8_t *msg)
{
    memcpy(shab_tx_buffer, msg, msg[SHAB_MSG_LENGTH]);  /*copy data*/

    initiate_shab_send();   /*Do it*/
}

/**
 * @brief 
 * 
 * @param me 
 * @param msg 
 * @param arg 
 */
void shab_process_packet(s_task_handle_t me, s_task_msg_t **msg, void* arg)
{
    shab_command_t curr_cmd;
    shab_dcoc_func_t curr_func;
    uint8_t p0, p1;

    if (shab_rx_buffer_rdy())   /*ready and valid?*/
    {
        curr_cmd = shab_rx_buffer[SHAB_COMMAND];        /*get command*/
        if (curr_cmd & SHAB_REQUEST)    /*is it a request*/
        {
            curr_cmd &= SHAB_REQST_MSK;                 /*adjust command*/
            curr_func = shab_rx_buffer[SHAB_FUNCTION];  /*get function*/
            if (CURR_DEVICE_ADDRESS() == INCM_DEVICE_ADDRESS()) /*is it for me?*/
            {
                p0 = shab_rx_buffer[SHAB_PARAM + 0];    /*get 1st param*/
                p1 = shab_rx_buffer[SHAB_PARAM + 1];    /*get 2nd param*/

                switch(curr_func)
                {
                    /*Device specific:*/
                    case RSC_RS_F:
                        switch(curr_cmd)
                        {
                            case SHAB_R:
                                /*Construct reply*/
                                if (MAXCHANNELS == p0)    /*read all*/
                                {
                                    compresss_rs_states_buffer(shab_tx_buffer, shab_rx_buffer[SHAB_SOURCE_ID], shab_rx_buffer[SHAB_SOURCE_INSTANCE]);
                                }
                                else
                                {
                                    if (!get_rs_state_buffer(p0, shab_tx_buffer, shab_rx_buffer[SHAB_SOURCE_ID], shab_rx_buffer[SHAB_SOURCE_INSTANCE]))
                                    {
                                        shab_ack_nack_buffer(shab_rx_buffer, shab_tx_buffer, false);    /*NACK*/
                                    }
                                }
                            break;

                            case SHAB_W:
                                switch(p1)
                                {
                                    case RSC_RS_STOP:
                                        if (set_rs_stop(p0))
                                        {
                                            get_rs_state_buffer(p0, shab_tx_buffer, shab_rx_buffer[SHAB_SOURCE_ID], shab_rx_buffer[SHAB_SOURCE_INSTANCE]);
                                        }
                                        else
                                        {
                                            shab_ack_nack_buffer(shab_rx_buffer, shab_tx_buffer, false);    /*NACK*/
                                        }
                                    break;

                                    case RSC_RS_UP:
                                        if (set_rs_up(p0))
                                        {
                                            get_rs_state_buffer(p0, shab_tx_buffer, shab_rx_buffer[SHAB_SOURCE_ID], shab_rx_buffer[SHAB_SOURCE_INSTANCE]);
                                        }
                                        else
                                        {
                                            shab_ack_nack_buffer(shab_rx_buffer, shab_tx_buffer, false);    /*NACK*/
                                        }
                                    break;

                                    case RSC_RS_DOWN:
                                        if (set_rs_down(p0))
                                        {
                                            get_rs_state_buffer(p0, shab_tx_buffer, shab_rx_buffer[SHAB_SOURCE_ID], shab_rx_buffer[SHAB_SOURCE_INSTANCE]);
                                        }
                                        else
                                        {
                                            shab_ack_nack_buffer(shab_rx_buffer, shab_tx_buffer, false);    /*NACK*/
                                        }
                                    break;

                                    case RSC_RS_TOGGLE:
                                    break;

                                    default:
                                        shab_ack_nack_buffer(shab_rx_buffer, shab_tx_buffer, false);    /*NACK*/
                                    break;
                                }
                            break;
                        }
                    break;

                    case RSC_RST_F:
                        //CR - P0: RS nb; P1: Up time; P2: Down time (in seconds)
                        //W - Save settings to EEPROM

                    break;

                    /*Universal:*/
                    case SHAB_PING_F:
                        switch(curr_cmd)
                        {
                            case SHAB_R:                        
                                shab_fw_info_buffer(shab_rx_buffer, shab_tx_buffer);
                            break;

                            default:
                                shab_ack_nack_buffer(shab_rx_buffer, shab_tx_buffer, false);    /*NACK*/
                            break;
                        }
                    break;

                    case SHAB_ID_F:
                         switch(curr_cmd)
                        {
                            case SHAB_CW:
                                curr_settings.dev_instance = p0;    /*get instance*/
                                shab_ack_nack_buffer(shab_rx_buffer, shab_tx_buffer, true);    /*ACK*/
                                save_settings();    /*save settings*/
                            break;
                        
                            default:
                                shab_ack_nack_buffer(shab_rx_buffer, shab_tx_buffer, false);    /*NACK*/
                            break;
                        }
                    break;

                    case SHAB_FLASH_F:
                    break;

                    default:
                        shab_ack_nack_buffer(shab_rx_buffer, shab_tx_buffer, false);    /*NACK*/
                    break;
                }

                initiate_shab_send();   /*send reply*/
            }
            else if (INCM_DEVICE_ADDRESS() == BROADCAST_ADDRESS)      /*is it a broadcast?*/
            {

            }
        }
    }
}

/**
 * @brief 
 * 
 */
void initiate_shab_send(void)
{
    shab_tx_buffer[shab_tx_buffer[SHAB_MSG_LENGTH]-1] = shab_calculate_checksum(shab_tx_buffer); //get checksum
    s_task_resume(shab_send_packet_tsk, true);
}

/*Keep this section at the end:*/
#ifdef SHAB_PORT_1
    #include "..\..\common\__shab1__.c"
#else
    #include "..\..\common\__shab2__.c"
#endif
