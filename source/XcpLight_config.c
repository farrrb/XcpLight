/* MIT License see LICENSE file             */
/* - Copyright (c) 2016 0xFAB - Fabian Zahn */

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

#ifdef XCP_CFG_USER_CMD
extern void XcpLight_ProcessUserCommand(XcpLightMessage_t *pMsg)
{
  return;
}
#endif // XCP_CFG_USER_CMD