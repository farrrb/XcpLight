#ifndef __XCPLIGHT_H
#define __XCPLIGHT_H
////////////////////////////////////////////////////////////////////////////////
/// file    XcpLight.h
/// date    09.2016
/// author  F. Zahn
/// version 00.00.01
/// note    for license see LICENSE file
////////////////////////////////////////////////////////////////////////////////
#include "XcpLight_config.h"

//------------------------------------------------------------------------------
// macro constants
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// struct and type definitions (struct, enum und typedef)
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

#endif // __XCPLIGHT_H

