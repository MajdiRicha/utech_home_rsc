/**
 * @file timer.h
 * @author Majdi Richa (majdi.richa@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-10-15
 * 
 * @copyright
 * 2025 utech Lebanon ALL RIGHTS RESERVED.
 * 
 */
#ifndef TIMER_H
#define TIMER_H

#include "scheduler.h"

//Timer related:
#define  TMR1Reload      2000     //running @ 16MIPS

cntr_size_t get_ticks_counter(void);

#endif
