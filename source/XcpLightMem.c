/* MIT License see LICENSE file             */
/* - Copyright (c) 2016 0xFAB - Fabian Zahn */

#include "XcpLightMem.h"

/*****************************************************************************/
/* external functions                                                        */
/*****************************************************************************/

int XcpLightMem_Init(XcpLightMemory_t * pMemData, uint8_t * pBuffer, uint32_t size)
{
  int ret = -1;

  if(pMemData && pBuffer && size)
  {
    pMemData->size = size;
    pMemData->used = 0u;
    pMemData->pBuffer = pBuffer;
    pMemData->pAlloc  = pBuffer;
    ret = 0;
  }
  return ret;
}

uint8_t * XcpLightMem_Alloc(XcpLightMemory_t * pMemData, uint32_t size)
{
  uint8_t * ret = 0;

  if(pMemData)
  {
    if(size <= (pMemData->size - pMemData->used))
    {
      ret = pMemData->pAlloc;
      pMemData->used   += size;
      pMemData->pAlloc += size;
    }
  }

  return ret;
}

int XcpLightMem_Clear(XcpLightMemory_t * pMemData)
{
  int ret = -1;

  if(pMemData)
  {
    if(pMemData->pBuffer && pMemData->size)
    {
      uint32_t byteCount = pMemData->size;
      uint8_t * pMem = pMemData->pBuffer;
      while(byteCount)
      {
        *pMem = 0u;
        pMem++;
        byteCount--;
      }
      pMemData->pAlloc = pMemData->pBuffer;
      pMemData->used = 0u;

      ret = 0;
    }
  }

  return ret;
}
