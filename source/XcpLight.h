#ifndef __XCPLIGHT_H
#define __XCPLIGHT_H

#ifdef __cplusplus
extern "C"
{
#endif

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
/* api functions for xcp integration */
void XcpLight_Init(void);
void XcpLight_UpdateTimestampCounter(void);
void XcpLight_CommandProcessor(XcpLightMessage_t * pMsg);
int  XcpLight_Event(uint8_t eventNo);

/* interface to transport layer */
extern void XcpLight_SendMessage(XcpLightMessage_t * pMsg);
extern uint32_t XcpLight_GetPointer(uint32_t address, uint8_t address_extension);
extern void XcpLight_ReadFromAddress(uint8_t * addr, uint8_t length, uint8_t * pMem);
extern void XcpLight_WriteToAddress(uint8_t * addr, uint8_t length, uint8_t * pMem);

#ifdef __cplusplus
}
#endif

#endif // __XCPLIGHT_H
