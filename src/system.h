/**
 * @file system.h
 * @author Majdi Richa (majdi.richa@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-10-15
 * 
 * @copyright
 * 2025 utech Lebanon ALL RIGHTS RESERVED.
 * 
 */
#ifndef  SYSTEM_H
#define  SYSTEM_H
///////////////////////////////////////////////////////////////////////////////
typedef struct _systeminfo
{
	int8 dummy;


}system_info;
///////////////////////////////////////////////////////////////////////////////
bool init_system(void);
void init_system_info(void);
void fill_system_params(void);
///////////////////////////////////////////////////////////////////////////////
#endif
