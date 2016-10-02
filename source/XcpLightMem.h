/* MIT License see LICENSE file             */
/* - Copyright (c) 2016 0xFAB - Fabian Zahn */

#ifndef __XCPLIGHTMEM_H
#define __XCPLIGHTMEM_H

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

typedef struct
{
  uint32_t size;
  uint32_t used;
  /* block of memory for storage of daq lists & samples */
  uint8_t * pBuffer;   /* pointer to storage */
  uint8_t * pAlloc; /* current allocation ptr */
} XcpLightMemory_t;

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
int XcpLightMem_Init(XcpLightMemory_t * pMemData, uint8_t * pBuffer, uint32_t size);
uint8_t * XcpLightMem_Alloc(XcpLightMemory_t * pMemData, uint32_t size);
int XcpLightMem_Clear(XcpLightMemory_t * pMemData);

#ifdef __cplusplus
}
#endif

#endif // __XCPLIGHTMEM_H
