/* MIT License see LICENSE file             */
/* - Copyright (c) 2016 0xFAB - Fabian Zahn */

#ifndef SOURCE_XCPLIGHT_H_
#define SOURCE_XCPLIGHT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "XcpLight_config.h"

/*****************************************************************************/
/* macro constants                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* struct and type definitions (struct, enum and typedef)                    */
/*****************************************************************************/

/* command transfer object */
typedef struct
{
  uint32_t length;
  uint8_t  payload[XCPLIGHT_CFG_XTO_LENGTH];
} XcpLightMessage_t;

/*****************************************************************************/
/* external data                                                             */
/*****************************************************************************/

/*****************************************************************************/
/* macro functions                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* external functions                                                        */
/*****************************************************************************/

/* api functions for xcp integration */
void XcpLight_Init(void);
void XcpLight_UpdateTimestampCounter(void);
void XcpLight_CommandProcessor(XcpLightMessage_t * pMsg);
int  XcpLight_Event(uint8_t eventNo);

/* interface to transport layer */
extern void XcpLight_SendMessage(XcpLightMessage_t * pMsg);
extern void * XcpLight_GetPointer(uint32_t address, uint8_t address_extension);
extern void XcpLight_ReadFromAddress(uint8_t * addr, uint8_t length, uint8_t * buffer);
extern void XcpLight_WriteToAddress(uint8_t * addr, uint8_t length, uint8_t * data);

#ifdef __cplusplus
}
#endif

#endif // SOURCE_XCPLIGHT_H_
