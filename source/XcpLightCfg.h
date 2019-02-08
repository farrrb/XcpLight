////////////////////////////////////////////////////////////////////////////////
/// MIT License see LICENSE file
/// Copyright (c) 2016-2019 0xFAB - Fabian Zahn
////////////////////////////////////////////////////////////////////////////////

#ifndef XCPLIGHT_CONFIG_H__
#define XCPLIGHT_CONFIG_H__

//* TRANSPORT LAYER CONFIG *
// The maximum length for cto and dto messages
#define XCPLIGHT_CFG_XTO_LENGTH     8


//* MCU CONFIG *
// Determine the systems byte order
#define XCPLIGHT_CFG_LITTLE_ENDIAN  1

// How many bytes are addressed by one address? Valid values are 1, 2, 4
#define XCPLIGHT_CFG_ADDRESS_GRANULARITY 1


//* OPTIONS *
// Enable login and unlock resources via seed & key
#define XCPLIGHT_CFG_SEED_AND_KEY 1
#define XCPLIGHT_CFG_SEED_LENGTH 10
#define XCPLIGHT_CFG_KEY_LENGTH  10

// Enable user specific hook for implementing own commands: XCP command code is 0xF1
#define XCPLIGHT_CFG_USER_CMD 1

// CAL & PAG
#define XCPLIGHT_CFG_ENABLE_CALPAG 1

// DAQ
//#define XCPLIGHT_CFG_ENABLE_DAQ
//#define XCPLIGHT_CFG_DAQ_MEMORY_SIZE 256 // [byte]

// STIM
//#define XCPLIGHT_CFG_ENABLE_STIM

// PGM
//#define XCPLIGHT_CFG_ENABLE_PGM

//* OPTIMIZATION *
//#define XCPLIGHT_CFG_DISABLE_STATIC_INLINE // compiler / c-standard specific

//* DEBUG *
#define XCPLIGHT_CFG_DEBUG_CMD_MSG

#endif // XCPLIGHT_CONFIG_H__

