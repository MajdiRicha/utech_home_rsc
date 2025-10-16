/**
 * @file shab.h
 * @author Majdi Richa (majdi.richa@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-10-15
 * 
 * @copyright
 * 2025 utech Lebanon ALL RIGHTS RESERVED.
 * 
 */
#ifndef   SHAB_H
#define   SHAB_H
///////////////////////////////////////////////////////////////////////////////
#include "shab_profile.h"
///////////////////////////////////////////////////////////////////////////////
#define  SHAB_MIN_BYTES    (8)
#define  SHAB_MAX_BYTES    (64)
#define  SHAB_BUFFER_MASK  (SHAB_MAX_BYTES-1)
#define  SHAB_RX_TIMEOUT   (50)  /*in ms*/     
///////////////////////////////////////////////////////////////////////////////

#define  BROADCAST_ADDRESS    0x0000
///////////////////////////////////////////////////////////////////////////////

bool init_shab(void);
void shab_send_msg(uint8_t *msg);
void shab_ack_nack_buffer(uint8_t *in_shab_buff, uint8_t *out_shab_buff, bool ack);
void shab_fw_info_buffer(uint8_t *in_shab_buff, uint8_t *out_shab_buff);

#endif
