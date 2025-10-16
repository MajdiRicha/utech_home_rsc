/**
 * @file settings.h
 * @author Majdi Richa (majdi.richa@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-10-15
 * 
 * @copyright
 * 2025 utech Lebanon ALL RIGHTS RESERVED.
 * 
 */
#ifndef SETTINGS_H
#define SETTINGS_H

#include "rs.h"

typedef struct system_settings
{
    uint8_t dev_instance;               /*device instance*/
    uint8_t dev_state[MAXCHANNELS];     /*channel states*/

    uint8_t rs_up_time[MAXCHANNELS];    //up time per channel
    uint8_t rs_dn_time[MAXCHANNELS];    //down time per channel

    //uint8_t dummy;                      /*Used to force the system to load default values*/

    /*keep those at the very end:*/
    uint8_t data_valid;
    uint8_t checksum;
}system_settings_t;

#define EE_SETTINGS_OFFSET  0x000
#define EE_SETTINGS_VALID   0xA5


bool init_settings(void);
uint8_t ee_calculate_checksum(uint8_t *dt_buff, uint16_t count);
void save_settings(void);
void initiate_save_settings(void);

#endif
