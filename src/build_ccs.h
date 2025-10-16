/**
 * @file build_ccs.h
 * @author Majdi Richa (majdi.richa@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-10-15
 * 
 * @copyright
 * 2025 utech Lebanon ALL RIGHTS RESERVED.
 * 
 */
#ifndef BUILD_CCS_DEFS_H
#define BUILD_CCS_DEFS_H


// Example of __DATE__ string: "31-Jan-03"
// Example of __TIME__ string: "hh:mm:ss"

const char _date_[] = __DATE__;

/*Compute day*/
#define BUILD_DAY   ((_date_[0] - '0') * 10 + (_date_[1] - '0'))

#define BUILD_MONTH_IS_JAN (_date_[3] == 'J' && _date_[4] == 'a' && _date_[5] == 'n')
#define BUILD_MONTH_IS_FEB (_date_[3] == 'F')
#define BUILD_MONTH_IS_MAR (_date_[3] == 'M' && _date_[4] == 'a' && _date_[5] == 'r')
#define BUILD_MONTH_IS_APR (_date_[3] == 'A' && _date_[4] == 'p')
#define BUILD_MONTH_IS_MAY (_date_[3] == 'M' && _date_[4] == 'a' && _date_[5] == 'y')
#define BUILD_MONTH_IS_JUN (_date_[3] == 'J' && _date_[4] == 'u' && _date_[5] == 'n')
#define BUILD_MONTH_IS_JUL (_date_[3] == 'J' && _date_[4] == 'u' && _date_[5] == 'l')
#define BUILD_MONTH_IS_AUG (_date_[3] == 'A' && _date_[4] == 'u')
#define BUILD_MONTH_IS_SEP (_date_[3] == 'S')
#define BUILD_MONTH_IS_OCT (_date_[3] == 'O')
#define BUILD_MONTH_IS_NOV (_date_[3] == 'N')
#define BUILD_MONTH_IS_DEC (_date_[3] == 'D')

/*Compute month*/
#define BUILD_MONTH \
    ( \
        (BUILD_MONTH_IS_JAN) ?  1 : \
        (BUILD_MONTH_IS_FEB) ?  2 : \
        (BUILD_MONTH_IS_MAR) ?  3 : \
        (BUILD_MONTH_IS_APR) ?  4 : \
        (BUILD_MONTH_IS_MAY) ?  5 : \
        (BUILD_MONTH_IS_JUN) ?  6 : \
        (BUILD_MONTH_IS_JUL) ?  7 : \
        (BUILD_MONTH_IS_AUG) ?  8 : \
        (BUILD_MONTH_IS_SEP) ?  9 : \
        (BUILD_MONTH_IS_OCT) ? 10 : \
        (BUILD_MONTH_IS_NOV) ? 11 : \
        (BUILD_MONTH_IS_DEC) ? 12 : \
        /* error default */  99 \
    )

/*Compute year*/
#define BUILD_YEAR  ((_date_[7] - '0') * 10 + (_date_[8] - '0'))

#endif // BUILD_CCS_DEFS_H
