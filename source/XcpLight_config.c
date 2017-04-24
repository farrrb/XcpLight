/* MIT License see LICENSE file             */
/* - Copyright (c) 2016-2017 0xFAB - Fabian Zahn */

#include "XcpLight_config.h"
#include "XcpLight.h"

/*****************************************************************************/
/* macro constants                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* struct and type definitions (struct, enum and typedef)                    */
/*****************************************************************************/

/*****************************************************************************/
/* external data                                                             */
/*****************************************************************************/
XcpLightMessage_t replyMsg = {0,{0}};

/*****************************************************************************/
/* macro functions                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* external functions                                                        */
/*****************************************************************************/

void XcpLight_SendMessage(XcpLightMessage_t *pMsg)
{
  /* XcpLightTransportLayerSend(pMsg); */
  replyMsg = *pMsg;
}

void * XcpLight_GetPointer(uint32_t address, uint8_t address_extension)
{
  return (void *)(address);
}

void XcpLight_ReadFromAddress(uint8_t *source, uint8_t length, uint8_t *buffer)
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

void XcpLight_WriteToAddress(uint8_t *dest, uint8_t length, uint8_t * data)
{
  if (dest)
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

extern int XcpLight_ProcessUserCommand(XcpLightMessage_t *pMsg, XcpLightMessage_t *pReplyMsg)
{
  // build response message and ...
  pReplyMsg->length   = 1u;
  pReplyMsg->payload[0] = 0x1Au; 
  return 1; // ... send msg
}

#endif // XCPLIGHT_CFG_USER_CMD

#ifdef XCPLIGHT_CFG_SEED_AND_KEY

extern void XcpLight_GetSeed(uint8_t resource, uint8_t *seed)
{
  int i=0;
  for (i = 0; i < XCPLIGHT_CFG_SEED_LENGTH; i++)
  {
    seed[i] = (i & 0xFFu) + resource;
  } 
}

extern int XcpLight_UnlockResource(uint8_t resource, uint8_t *key)
{
  if (!resource)
  {
    return XCPLIGHT_RET_UNLOCK_FAILED;
  }

  return XCPLIGHT_RET_UNLOCK_SUCCESS;
}

#endif // XCPLIGHT_CFG_SEED_AND_KEY
