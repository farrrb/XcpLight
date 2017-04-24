/* MIT License see LICENSE file             */
/* - Copyright (c) 2016-2017 0xFAB - Fabian Zahn */

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

#ifdef XCPLIGHT_CFG_SEED_AND_KEY
  #define XCP_RET_UNLOCK_SUCCESS 1
  #define XCP_RET_UNLOCK_FAILED  0
#endif // XCPLIGHT_CFG_SEED_AND_KEY

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
void XcpLight_CommandProcessor(XcpLightMessage_t *pMsg);
int  XcpLight_Event(uint8_t eventNo);

/* interface to transport layer */
extern void XcpLight_SendMessage(XcpLightMessage_t *pMsg);
extern void * XcpLight_GetPointer(uint32_t address, uint8_t address_extension);
extern void XcpLight_ReadFromAddress(uint8_t *source, uint8_t length, uint8_t *buffer);
extern void XcpLight_WriteToAddress(uint8_t *dest, uint8_t length, uint8_t *data);

#ifdef XCPLIGHT_CFG_USER_CMD
extern int XcpLight_ProcessUserCommand(XcpLightMessage_t *pMsg, XcpLightMessage_t *pReplyMsg);
#endif // XCPLIGHT_CFG_USER_CMD

#ifdef XCPLIGHT_CFG_SEED_AND_KEY
extern void XcpLight_GetSeed(uint8_t resource, uint8_t *seed);
extern int XcpLight_UnlockResource(uint8_t resource, uint8_t *key);
#endif // XCPLIGHT_CFG_SEED_AND_KEY

#ifdef __cplusplus
}
#endif

#endif // SOURCE_XCPLIGHT_H_
