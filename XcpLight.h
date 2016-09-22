#ifndef __XCPLIGHT_H
#define __XCPLIGHT_H

#include "XcpLight_config.h"

//------------------------------------------------------------------------------
// macro constants
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// struct and type definitions (struct, enum and typedef)
//------------------------------------------------------------------------------

/* command transfer object */
typedef struct
{
  uint8_t payload[XCPLIGHT_CFG_XTO_LENGTH];
  uint8_t length;
} XcpLightMessage_t;

//------------------------------------------------------------------------------
// external data
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// macro functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// external functions
//------------------------------------------------------------------------------
void XcpLight_Init(void);
void XcpLight_UpdateTimestampCounter(void);
int XcpLight_Event(uint8_t eventNo);

/* interface to transport layer */
void XcpLight_CommandProcessor(XcpLightMessage_t * pMsg);

extern void XcpLight_SendMessage(XcpLightMessage_t * pMsg);
extern uint32_t XcpLight_GetPointer(uint32_t address, uint8_t address_extension);
extern void XcpLight_ReadFromAddress(uint8_t * addr, uint8_t length, uint8_t * pMem);

#endif // __XCPLIGHT_H

