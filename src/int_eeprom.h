/**
 * @file int_eeprom.h
 * @author Majdi Richa (majdi.richa@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-10-15
 * 
 * @copyright
 * 2025 utech Lebanon ALL RIGHTS RESERVED.
 * 
 */
#ifndef INT_EEPROM_H
#define INT_EEPROM_H

#if getenv("DATA_EEPROM") >= 256 
    void int_ee_write_buff(uint8_t *data_buff, uint16_t offset, uint16_t size);
    void int_ee_read_buff(uint8_t *data_buff, uint16_t offset, uint16_t size);
    void int_ee_compare_write_buff(uint8_t *data_buff, uint16_t offset, uint16_t size);
#else
    void int_ee_write_buff(uint8_t *data_buff, uint8_t offset, uint8_t size);
    void int_ee_read_buff(uint8_t *data_buff, uint8_t offset, uint8_t size);
    void int_ee_compare_write_buff(uint8_t *data_buff, uint8_t offset, uint8_t size);
#endif
    
#endif
