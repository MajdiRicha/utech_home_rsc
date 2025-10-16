/**
 * @file version.h
 * @author Majdi Richa (majdi.richa@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-10-15
 * 
 * @copyright
 * 2025 utech Lebanon ALL RIGHTS RESERVED.
 * 
 */
#ifndef VERSION_H
#define VERSION_H

// Revision history
#define	FW_REV_MAJ	0x2	/*0 to F*/
#define	FW_REV_MIN	0x0	/*0 to F*/
#define	FW_REV_BLD	0x0	/*0 to F*/

#if (defined (__PCH__) || defined (__PCD__))	/*CCS C compiler?*/
    #include "build_ccs.h"
#else
    #include "build.h"
#endif

/*Auto-compute date*/
#define   FW_REV_DAY   BUILD_DAY    /*Day*/
#define   FW_REV_MNT   BUILD_MONTH  /*Month*/
#define   FW_REV_YER   BUILD_YEAR   /*Year*/

//   V:2.0.0
    //


#endif /* VERSION_H */
