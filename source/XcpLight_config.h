/* MIT License see LICENSE file             */
/* - Copyright (c) 2016 0xFAB - Fabian Zahn */

#ifndef __XCPLIGHT_CONFIG_H
#define __XCPLIGHT_CONFIG_H

/** INCLUDES **/
#include <stdint.h>

/** CONFIG **/
#define XCPLIGHT_CFG_XTO_LENGTH 8

//#define XCPLIGHT_CFG_ENABLE_RESOURCE_PROTECTION

/** OPTIONS **/
/* CAL */
//#define XCPLIGHT_CFG_ENABLE_CALPAG

/* DAQ */
//#define XCPLIGHT_CFG_ENABLE_DAQ
//#define XCPLIGHT_CFG_DAQ_MEMORY_SIZE 256 /* [byte] */

/* STIM */
//#define XCPLIGHT_CFG_ENABLE_STIM

/* PGM */
//#define XCPLIGHT_CFG_ENABLE_PGM


/** OPTIMIZATION **/
#define XCPLIGHT_CFG_ENABLE_STATIC_INLINE /* compiler / c-standard specific */

/** DEBUG **/
#define XCPLIGHT_CFG_DEBUG_CMD_MSG

#endif // __XCPLIGHT_CONFIG_H

