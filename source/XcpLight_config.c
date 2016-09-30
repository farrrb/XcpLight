#include "XcpLight_config.h"
#include "XcpLight.h"

//------------------------------------------------------------------------------
// macro constants
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// struct and type definitions (struct, enum and typedef)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// external data
//------------------------------------------------------------------------------
XcpLightMessage_t replyMsg = {{0}};

//------------------------------------------------------------------------------
// macro functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// external functions
//------------------------------------------------------------------------------

void XcpLight_SendMessage(XcpLightMessage_t * pMsg)
{
  //XcpLightTransportLayerSend(pMsg);
  replyMsg = *pMsg;
}

void * XcpLight_GetPointer(uint32_t address, uint8_t address_extension)
{
  return (void *)(address);
}

extern void XcpLight_ReadFromAddress(uint8_t * addr, uint8_t length, uint8_t * pMem)
{
  while(length)
  {
    *pMem = (*addr);
    pMem++;
    addr++;
    length--;
  }
}

extern void XcpLight_WriteToAddress(uint8_t * addr, uint8_t length, uint8_t * pMem)
{
  while(length)
  {
    *addr = (*pMem);
    pMem++;
    addr++;
    length--;
  }
}
