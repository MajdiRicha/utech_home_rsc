/**
 * @file int_eeprom.c
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

#if getenv("DATA_EEPROM") >= 256 

/**
 * @brief 
 * 
 * @param data_buff 
 * @param offset 
 * @param size 
 */
void int_ee_write_buff(uint8_t *data_buff, uint16_t offset, uint16_t size)
{
    uint16_t i;

    for (i = 0; i < size; i++)
    {
        write_eeprom(offset+i, *data_buff++);
    }
}

/**
 * @brief 
 * 
 * @param data_buff 
 * @param offset 
 * @param size 
 */
void int_ee_read_buff(uint8_t *data_buff, uint16_t offset, uint16_t size)
{
    uint16_t i;

    for (i = 0; i < size; i++)
    {
        *data_buff++ = read_eeprom(offset+i);
    }
}

/**
 * @brief 
 * 
 * @param data_buff 
 * @param offset 
 * @param size 
 */
void int_ee_compare_write_buff(uint8_t *data_buff, uint16_t offset, uint16_t size)
{
    uint16_t i;

    for (i = 0; i < size; i++)
    {   
        if (read_eeprom(offset+i) != *data_buff)    /*no match?*/
        {
            write_eeprom(offset+i, *data_buff);   /*update location*/
        }
        data_buff++;    /*next*/
    }
}
#else

void int_ee_write_buff(uint8_t *data_buff, uint8_t offset, uint8_t size)
{

}

void int_ee_read_buff(uint8_t *data_buff, uint8_t offset, uint8_t size)
{

}

void int_ee_compare_write_buff(uint8_t *data_buff, uint8_t offset, uint8_t size)
{

}
#endif
