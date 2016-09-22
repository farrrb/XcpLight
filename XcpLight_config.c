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

//------------------------------------------------------------------------------
// macro functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// external functions
//------------------------------------------------------------------------------

void XcpLight_SendMessage(XcpLightMessage_t * pMsg)
{
 XcpLightTransportLayerSend(pMsg);
}

uint32_t XcpLight_GetPointer(uint32_t address, uint8_t address_extension)
{
  return address;
}

extern void XcpLight_ReadFromAddress(uint8_t * addr, uint8_t length, uint8_t * pMem)
{
  while(length)
  {
    *pMem = *((uint8_t *)addr) & 0xFFu;
    pMem++;
    addr++;
    length--;
  }
}
