/**
 * @file rs.h
 * @author Majdi Richa (majdi.richa@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-10-16
 * 
 * @copyright
 * 2025 utech Lebanon ALL RIGHTS RESERVED.
 * 
 */
#ifndef  RS_H
#define  RS_H

#include "relay.h"

#define  MAXCHANNELS (MAXRELAYS/2)

bool set_rs_up(uint8_t rs);
bool set_rs_down(uint8_t rs);
bool set_rs_stop(uint8_t rs);
bool get_rs_state_buffer(uint8_t rs_idx, uint8_t *shab_buff, shab_device_t dest_dev, uint8_t dest_instance);
void compresss_rs_states_buffer(uint8_t *shab_buff, shab_device_t dest_dev, uint8_t dest_instance);

#endif
