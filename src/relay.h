/**
 * @file relay.h
 * @author Majdi Richa (majdi.richa@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-10-15
 * 
 * @copyright
 * 2025 utech Lebanon ALL RIGHTS RESERVED.
 * 
 */
#ifndef RELAY_H
#define RELAY_H

#include "shab_profile.h"

#define MAXRELAYS       6
#define SAVE_TIMEOUT    3000    //in ms
#define RELAY_SEQ_DLY   500     //in ms

#define  RLYSTATESADRS  (sizeof(SystemSettings)*2)

bool init_relays(void);

bool set_relay_on(uint8_t rly_idx);
bool set_relay_off(uint8_t rly_idx);
bool toggle_relay(uint8_t rly_idx);
uint8_t get_relay_state(uint8_t rly_idx);
bool get_relay_state_buffer(uint8_t rly_idx, uint8_t *shab_buff, shab_device_t dest_dev, uint8_t dest_instance);
bool get_relay_timer_buffer(uint8_t rly_idx, uint8_t *shab_buff, shab_device_t dest_dev, uint8_t dest_instance);
bool set_relay_timer(uint8_t rly_idx, uint8_t *shab_msg);
bool get_relay_count_down_buffer(uint8_t rly_idx, uint8_t *shab_buff, shab_device_t dest_dev, uint8_t dest_instance);
void compresss_relay_states_buffer(uint8_t *shab_buff, shab_device_t dest_dev, uint8_t dest_instance);


//bool set_relays_all_off(void);

//bool load_relay_states(void);
//bool save_relay_states(uint8_t *);
//bool update_relays(uint8_t);
//bool update_relays_off(uint8_t);
//bool update_relays_on(uint8_t);

#endif
