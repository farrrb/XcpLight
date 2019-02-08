////////////////////////////////////////////////////////////////////////////////
/// MIT License see LICENSE file
/// Copyright (c) 2016-2019 0xFAB - Fabian Zahn
////////////////////////////////////////////////////////////////////////////////

#ifndef XCPLIGHT_H__
#define XCPLIGHT_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "XcpLightCfg.h"

//------------------------------------------------------------------------------
// Macro Constants
//------------------------------------------------------------------------------

/// \brief Resource Codes
typedef enum XcpLightResource
{
  XCPLIGHT_RESOURCE_CALPAG = 0x01u,
  XCPLIGHT_RESOURCE_DAQ    = 0x04u,
  XCPLIGHT_RESOURCE_STIM   = 0x08u,
  XCPLIGHT_RESOURCE_PGM    = 0x10u
} XcpLightResource_t;

//------------------------------------------------------------------------------
// Struct and Type Definitions (struct, enum and typedef)
//------------------------------------------------------------------------------

/// \brief Structure of one XCP Message (CTO / DTO)
typedef struct
{
  uint32_t length;
  uint8_t  payload[XCPLIGHT_CFG_XTO_LENGTH];
} XcpLightMessage_t;

//------------------------------------------------------------------------------
// External Data
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Macro Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// External Functions
//------------------------------------------------------------------------------

//==============================================================================
// API Functions for XCP Integration
//==============================================================================

////////////////////////////////////////////////////////////////////////////////
/// \brief      Initialization function
////////////////////////////////////////////////////////////////////////////////
void XcpLight_init(void);

////////////////////////////////////////////////////////////////////////////////
/// \brief      Cyclic update function for the timestamp counter
///
///             \note This function has to be called every 1 [ms]
////////////////////////////////////////////////////////////////////////////////
void XcpLight_updateTimestampCounter(void);

////////////////////////////////////////////////////////////////////////////////
/// \brief      Process a XCP message
///
///             After receiving a message via the transport layer this functions
///             processes its payload and passes it to the respective command
///             processor.
///
/// \param      msg   Pointer to the XCP message
////////////////////////////////////////////////////////////////////////////////
void XcpLight_processCommand(XcpLightMessage_t *msg);

//==============================================================================
// External Functions (to be defined in the configuration)
//==============================================================================

////////////////////////////////////////////////////////////////////////////////
/// \brief      Send a XCP message via the transport layer
///
/// \param      msg   Pointer to the message
////////////////////////////////////////////////////////////////////////////////
extern void  XcpLightCfg_sendMessage(XcpLightMessage_t *msg);

////////////////////////////////////////////////////////////////////////////////
/// \brief      Evaluates an address (with address extension) and resolves it to
///             a pointer in memory
///
/// \param[in]  address            The address
/// \param[in]  address_extension  The address extension
///
/// \return     Pointer to the address for reading out / writing data
////////////////////////////////////////////////////////////////////////////////
extern void* XcpLightCfg_getPointer(uint32_t address, uint8_t address_extension);

////////////////////////////////////////////////////////////////////////////////
/// \brief      Reads an amount of data from source into buffer
///
/// \param      source  The source
/// \param[in]  length  The length
/// \param      buffer  The buffer
////////////////////////////////////////////////////////////////////////////////
extern void  XcpLightCfg_readFromAddress(uint8_t *source, uint8_t length, uint8_t *buffer);

#ifdef XCPLIGHT_CFG_ENABLE_CALPAG
////////////////////////////////////////////////////////////////////////////////
/// \brief      Writes an amount of data from data to destination
///
/// \param      dest    The destination
/// \param[in]  length  The length
/// \param      data    The data
////////////////////////////////////////////////////////////////////////////////
extern void  XcpLightCfg_writeToAddress(uint8_t *dest, uint8_t length, uint8_t *data);
#endif // XCPLIGHT_CFG_ENABLE_CALPAG

#ifdef XCPLIGHT_CFG_USER_CMD
////////////////////////////////////////////////////////////////////////////////
/// \brief      Processes a user command (application specific)
///
/// \param      msg        The message
/// \param      reply_msg  The reply message
///
/// \return     Whether or not to send a reply message (true := send / false := don't)
////////////////////////////////////////////////////////////////////////////////
extern bool XcpLightCfg_processUserCommand(XcpLightMessage_t *msg, XcpLightMessage_t *reply_msg);
#endif // XCPLIGHT_CFG_USER_CMD

#ifdef XCPLIGHT_CFG_SEED_AND_KEY
////////////////////////////////////////////////////////////////////////////////
/// \brief      Get a seed for the seed & key algorithm
///
/// \param[in]  resource  The resource
/// \param      seed      The seed
////////////////////////////////////////////////////////////////////////////////
extern void XcpLightCfg_getSeed(XcpLightResource_t resource, uint8_t *seed);

////////////////////////////////////////////////////////////////////////////////
/// \brief      Unlocks a resource (user hook). Checks if the provided key
///             matches the internal calculated key.
///
/// \param[in]  resource  The resource
/// \param      key       The key
///
/// \return     Whether or not the unlock was successful
///             (true := success / false := failure)
////////////////////////////////////////////////////////////////////////////////
extern bool XcpLightCfg_unlockResource(XcpLightResource_t resource, uint8_t *key);
#endif // XCPLIGHT_CFG_SEED_AND_KEY

#ifdef __cplusplus
}
#endif

#endif // XCPLIGHT_H__
