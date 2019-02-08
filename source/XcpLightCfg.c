////////////////////////////////////////////////////////////////////////////////
/// MIT License see LICENSE file
/// Copyright (c) 2016-2019 0xFAB - Fabian Zahn
////////////////////////////////////////////////////////////////////////////////

#include "XcpLight.h"

//------------------------------------------------------------------------------
// Macro Constants
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Struct and Type Definitions (struct, enum and typedef)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// External Data
//------------------------------------------------------------------------------

XcpLightMessage_t reply_msg = {0, {0}};

//------------------------------------------------------------------------------
// Macro Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// External Functions
//------------------------------------------------------------------------------

void XcpLightCfg_sendMessage(XcpLightMessage_t *msg)
{
  // XcpLightTransportLayerSend(msg);
  reply_msg = *msg;
}

void* XcpLightCfg_getPointer(uint32_t address, uint8_t address_extension)
{
  (void)address_extension;
  return (void *)(address);
}

void XcpLightCfg_readFromAddress(uint8_t *source, uint8_t length, uint8_t *buffer)
{
  if (source)
  {
    while (length)
    {
      *buffer = (*source);
      buffer++;
      source++;
      length--;
    }
  }
}

void XcpLightCfg_writeToAddress(uint8_t *dest, uint8_t length, uint8_t * data)
{
  if (dest != NULL)
  {
    while (length)
    {
      *dest = (*data);
      data++;
      dest++;
      length--;
    }
  }
}

#ifdef XCPLIGHT_CFG_USER_CMD

extern bool XcpLightCfg_processUserCommand(XcpLightMessage_t *msg, XcpLightMessage_t *reply_msg)
{
  if (msg == NULL)
  {
    return false;
  }
  // build response message and ...
  reply_msg->length   = 1u;
  reply_msg->payload[0] = 0x1Au;
  return true; // ... send msg
}

#endif // XCPLIGHT_CFG_USER_CMD

#ifdef XCPLIGHT_CFG_SEED_AND_KEY

extern void XcpLightCfg_getSeed(XcpLightResource_t resource, uint8_t *seed)
{
  int i=0;
  for (i = 0; i < XCPLIGHT_CFG_SEED_LENGTH; i++)
  {
    seed[i] = (i & 0xFFu) + resource;
  }
}

extern bool XcpLightCfg_unlockResource(XcpLightResource_t resource, uint8_t *key)
{
  if (resource == 0 || key == NULL)
  {
    return true;
  }

  return false;
}

#endif // XCPLIGHT_CFG_SEED_AND_KEY
