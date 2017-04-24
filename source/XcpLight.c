/* MIT License see LICENSE file             */
/* - Copyright (c) 2016 0xFAB - Fabian Zahn */

#include "XcpLight.h"
#include "XcpLight_internals.h"

/*
-------------------------------------------------------------------------------
  Limitations:
    1. Block transfer is not supported (UPLOAD, DOWNLOAD, PROGRAM)

-------------------------------------------------------------------------------
  Todo list:
    0. fix all @todo points
    1. implement daq processor
    2. implement programming (flash file download)
    3. implement resource protection
    4. test test test
-------------------------------------------------------------------------------
*/

/*****************************************************************************/
/* local macro constants                                                     */
/*****************************************************************************/

/** VERSIONS **/
#define XCP_VER_DRIVER            (0x01u)
#define XCP_VER_PROTOCOL_LAYER    (0x01u)
#define XCP_VER_TRANSPORT_LAYER   (0x01u)

/** INTERNAL RETURN CODES **/
#define MSG_NONE (0)
#define MSG_SEND (1)

/** EVALUATE CONFIG **/
#ifdef XCPLIGHT_CFG_ENABLE_STATIC_INLINE
  #define XCP_STATIC_INLINE static inline
#else
  #define XCP_STATIC_INLINE
#endif

/*****************************************************************************/
/* local struct and type definitions (struct, enum and typedef)              */
/*****************************************************************************/

/*****************************************************************************/
/* local data                                                                */
/*****************************************************************************/
/* internal data structure for state etc. */
XcpLightInternals_t _XcpLightData = {0};

/*****************************************************************************/
/* local macro functions                                                     */
/*****************************************************************************/

/*****************************************************************************/
/* local functions - prototypes                                              */
/*****************************************************************************/
XCP_STATIC_INLINE uint32_t _BuildU32FromU8Array(uint8_t *pArray);
XCP_STATIC_INLINE int _BuildErrorMessage(XcpLightMessage_t * pMsg, uint8_t errorCode);

XCP_STATIC_INLINE int _CmdConnect(XcpLightMessage_t * pCmdMsg, XcpLightMessage_t * pReplyMsg);
XCP_STATIC_INLINE int _CmdDisconnect(XcpLightMessage_t * pCmdMsg, XcpLightMessage_t * pReplyMsg);
XCP_STATIC_INLINE int _CmdSynch(XcpLightMessage_t * pCmdMsg, XcpLightMessage_t * pReplyMsg);
XCP_STATIC_INLINE int _CmdGetCommModeInfo(XcpLightMessage_t * pMsg, XcpLightMessage_t * pReplyMsg);
XCP_STATIC_INLINE int _CmdGetStatus(XcpLightMessage_t * pMsg, XcpLightMessage_t * pReplyMsg);

XCP_STATIC_INLINE int _CmdShortUpload(XcpLightMessage_t * pMsg, XcpLightMessage_t * pReplyMsg);
XCP_STATIC_INLINE int _CmdSetMta(XcpLightMessage_t * pMsg, XcpLightMessage_t * pReplyMsg);
XCP_STATIC_INLINE int _CmdDownload(XcpLightMessage_t * pMsg, XcpLightMessage_t * pReplyMsg);

/*****************************************************************************/
/* local functions                                                           */
/*****************************************************************************/
XCP_STATIC_INLINE uint32_t _BuildU32FromU8Array(uint8_t *pArray)
{
  uint32_t tmpAddress;

#ifdef XCPLIGHT_CFG_LITTLE_ENDIAN
  tmpAddress  = (pArray[0] & 0xFFu) <<  0u;
  tmpAddress |= (pArray[1] & 0xFFu) <<  8u;
  tmpAddress |= (pArray[2] & 0xFFu) << 16u;
  tmpAddress |= (pArray[3] & 0xFFu) << 24u;
#else
  tmpAddress  = (pArray[0] & 0xFFu) << 24u;
  tmpAddress |= (pArray[1] & 0xFFu) << 16u;
  tmpAddress |= (pArray[2] & 0xFFu) <<  8u;
  tmpAddress |= (pArray[3] & 0xFFu) <<  0u;
#endif

  return tmpAddress;
}

XCP_STATIC_INLINE int _BuildErrorMessage(XcpLightMessage_t * pMsg, uint8_t errorCode)
{
  pMsg->length = 2u;
  pMsg->payload[0] = XCP_PID_ERR;
  pMsg->payload[1] = errorCode;

  return MSG_SEND;
}

XCP_STATIC_INLINE int _CmdConnect(XcpLightMessage_t * pCmdMsg, XcpLightMessage_t * pReplyMsg)
{
  /* process the connect command anytime -> independence of state */
  pReplyMsg->length = 8u;
  pReplyMsg->payload[0] = XCP_PID_RES;

  pReplyMsg->payload[1] = 0u;
#ifdef XCPLIGHT_CFG_ENABLE_CALPAG
  pReplyMsg->payload[1] |= XCP_RES_CALPAG;
#endif

#ifdef XCPLIGHT_CFG_ENABLE_DAQ
  pReplyMsg->payload[1] |= XCP_RES_DAQ;
#endif

#ifdef XCPLIGHT_CFG_ENABLE_STIM
  pReplyMsg->payload[1] |= XCP_RES_STIM;
#endif

#ifdef XCPLIGHT_CFG_ENABLE_PGM
  pReplyMsg->payload[1] |= XCP_RES_PGM;
#endif

  pReplyMsg->payload[2] = 0x80u; // @todo COMM_MODE_BASIC
  pReplyMsg->payload[3] = XCPLIGHT_CFG_XTO_LENGTH;
  pReplyMsg->payload[4] = XCPLIGHT_CFG_XTO_LENGTH;
  pReplyMsg->payload[5] = 0x00u; // reserved
  pReplyMsg->payload[6] = XCP_VER_PROTOCOL_LAYER;
  pReplyMsg->payload[7] = XCP_VER_TRANSPORT_LAYER;

  /* protect all resources */
  #ifdef XCPLIGHT_CFG_ENABLE_RESOURCE_PROTECTION
  _XcpLightData.protectionStatus = XCP_PRT_PROTECT_ALL;
  #endif

  /* set session to connected state */
  _XcpLightData.sessionStatus |= XCP_SES_CONNECTED; // mark session as active (connected)

  return MSG_SEND;
}

XCP_STATIC_INLINE int _CmdDisconnect(XcpLightMessage_t * pCmdMsg, XcpLightMessage_t * pReplyMsg)
{
  /* protect all resources */
  #ifdef XCPLIGHT_CFG_ENABLE_RESOURCE_PROTECTION
  _XcpLightData.protectionStatus = XCP_PRT_PROTECT_ALL;
  #endif

  /* reset all session status bits */
  _XcpLightData.sessionStatus    = XCP_SES_RESET_SESSION;

  pReplyMsg->length = 1u;
  pReplyMsg->payload[0] = XCP_PID_RES;

  return MSG_SEND;
}

XCP_STATIC_INLINE int _CmdSynch(XcpLightMessage_t * pCmdMsg, XcpLightMessage_t * pReplyMsg)
{
  return _BuildErrorMessage(pReplyMsg, XCP_ERR_CMD_SYNCH);
}

XCP_STATIC_INLINE int _CmdGetCommModeInfo(XcpLightMessage_t * pCmdMsg, XcpLightMessage_t * pReplyMsg)
{
  pReplyMsg->length = 8u;
  pReplyMsg->payload[0] = XCP_PID_RES;
  pReplyMsg->payload[1] = 0x00u; // reserved
  pReplyMsg->payload[2] = 0x00u; // @todo: COMM_MODE_OPTIONAL
  pReplyMsg->payload[3] = 0x00u; // reserved
  pReplyMsg->payload[4] = 0x00u; // @todo: MAX_BS
  pReplyMsg->payload[5] = 0x00u; // @todo: MAX_ST
  pReplyMsg->payload[6] = 0x00u; // @todo: QUEUE_SIZE
  pReplyMsg->payload[7] = XCP_VER_DRIVER;

  return MSG_SEND;
}

XCP_STATIC_INLINE int _CmdGetStatus(XcpLightMessage_t * pMsg, XcpLightMessage_t * pReplyMsg)
{
  pReplyMsg->length = 6u;
  pReplyMsg->payload[0] = XCP_PID_RES;
  pReplyMsg->payload[1] = _XcpLightData.sessionStatus;
  pReplyMsg->payload[2] = _XcpLightData.protectionStatus;
  pReplyMsg->payload[3] = 0x00u; /* STATE_NUMBER: ECU_STATES not supported */

  pReplyMsg->payload[4] = 0x00u; // @todo: Session configuration id
  pReplyMsg->payload[5] = 0x00u; // @todo: Session configuration id

  return MSG_SEND;
}

XCP_STATIC_INLINE int _CmdShortUpload(XcpLightMessage_t * pMsg, XcpLightMessage_t * pReplyMsg)
{
  uint32_t tmpAddress;
  uint8_t  tmpAddressExt = 0;
  uint8_t  length = (pMsg->payload[1] & 0xFFu);

  if(length < (XCPLIGHT_CFG_XTO_LENGTH - 1u))
  {
    pReplyMsg->length = length + 1u;
    pReplyMsg->payload[0] = XCP_PID_RES;
    tmpAddressExt  = (pMsg->payload[3] & 0xFFu);  
    
    tmpAddress = _BuildU32FromU8Array(&(pMsg->payload[4]));

    _XcpLightData.mta = XcpLight_GetPointer(tmpAddress, tmpAddressExt);

    XcpLight_ReadFromAddress((uint8_t *)_XcpLightData.mta, length, &(pReplyMsg->payload[1]));
  }
  else
  {
    _BuildErrorMessage(pReplyMsg, XCP_ERR_OUT_OF_RANGE);
  }

  return MSG_SEND;
}

XCP_STATIC_INLINE int _CmdSetMta(XcpLightMessage_t * pMsg, XcpLightMessage_t * pReplyMsg)
{
  uint32_t tmpAddress = 0;
  uint8_t  tmpAddressExt = 0;

  pReplyMsg->length = 2u;
  pReplyMsg->payload[0] = XCP_PID_RES;
  pReplyMsg->payload[1] = 0x00u;

  tmpAddressExt = (pMsg->payload[3] & 0xFFu);

  tmpAddress = _BuildU32FromU8Array(&(pMsg->payload[4]));

  _XcpLightData.mta = XcpLight_GetPointer(tmpAddress, tmpAddressExt);

  return MSG_SEND;
}

XCP_STATIC_INLINE int _CmdDownload(XcpLightMessage_t * pMsg, XcpLightMessage_t * pReplyMsg)
{
  uint8_t length = (pMsg->payload[1] & 0xFFu);
  if(length < (XCPLIGHT_CFG_XTO_LENGTH - 1u))
  {
    XcpLight_WriteToAddress((uint8_t *)_XcpLightData.mta, length, &(pMsg->payload[2]));

    pReplyMsg->length = 1u;
    pReplyMsg->payload[0] = XCP_PID_RES;
  }
  else
  {
    _BuildErrorMessage(pReplyMsg, XCP_ERR_OUT_OF_RANGE);
  }

  return MSG_SEND;
}

/*****************************************************************************/
/* external functions                                                        */
/*****************************************************************************/
void XcpLight_Init(void)
{
  _XcpLightData.timestampCounter = 0u;
  _XcpLightData.sessionStatus    = XCP_SES_RESET_SESSION;

  #ifdef XCPLIGHT_CFG_ENABLE_RESOURCE_PROTECTION
  _XcpLightData.protectionStatus = XCP_PRT_PROTECT_ALL;
  #else
  _XcpLightData.protectionStatus = 0u;
  #endif
}

void XcpLight_UpdateTimestampCounter(void)
{
  _XcpLightData.timestampCounter++;
}

void XcpLight_CommandProcessor(XcpLightMessage_t * pMsg)
{
  XcpLightMessage_t * pReplyMsg = &(_XcpLightData.ctoReplyMsg);

#ifdef XCPLIGHT_CFG_DEBUG_CMD_MSG
  _XcpLightData.ctoCmdMsg = *pMsg; /* copy to internal state (for ext. debugging) */
#endif

  uint8_t sendFlag = 0;

  _XcpLightData.currentCommand = (pMsg->payload[0] & 0xFFu);

  if(_XcpLightData.currentCommand == XCP_CMD_CONNECT)
  {
    /* STD : standard commands :: connect begin */
    sendFlag = _CmdConnect(pMsg, pReplyMsg);
    /* STD : standard commands :: connect end */
  }
  else
  {
    if(_XcpLightData.sessionStatus & XCP_SES_CONNECTED)
    {
      /* in connected state process all other commands*/
      switch(_XcpLightData.currentCommand)
      {
        /* STD : standard commands -> begin */
        case XCP_CMD_DISCONNECT:
          sendFlag = _CmdDisconnect(pMsg, pReplyMsg);
          break;

        case XCP_CMD_SYNCH:
          sendFlag = _CmdSynch(pMsg, pReplyMsg);
          break;

        case XCP_CMD_GET_COMM_MODE_INFO:
          sendFlag = _CmdGetCommModeInfo(pMsg, pReplyMsg);
          break;

        case XCP_CMD_GET_STATUS:
          sendFlag = _CmdGetStatus(pMsg, pReplyMsg);
          break;

        case XCP_CMD_SET_MTA:
          sendFlag = _CmdSetMta(pMsg, pReplyMsg);
          break;

        case XCP_CMD_UPLOAD:
          sendFlag = _BuildErrorMessage(pReplyMsg, XCP_ERR_CMD_UNKNOWN); /* @todo: implement me */

        case XCP_CMD_SHORT_UPLOAD:
          sendFlag = _CmdShortUpload(pMsg, pReplyMsg);
          break;
        /* STD : standard commands -> end */

        /* CAL : standard commands -> end */
        case XCP_CMD_DOWNLOAD:
          sendFlag = _CmdDownload(pMsg, pReplyMsg);
          break;
        case XCP_CMD_SHORT_DOWNLOAD:
          sendFlag = _BuildErrorMessage(pReplyMsg, XCP_ERR_CMD_UNKNOWN); /* @todo: implement me */
          break;
        /* CAL : standard commands -> end */

        /* PAG: page switching commands -> begin */
        /* @note: not supported yet */
        /* PAG: page switching commands -> end */

        /* DAQ: data aquisition commands -> begin */
        /* @note: not supported yet */
        /* DAQ: data aquisition commands -> end */

        /* PGM: programming commands -> begin */
        /* @note: not supported yet */
        /* PGM: programming commands -> end */

        /* Whenever a command is unknown -> build and send an error message */
        default:
          sendFlag = _BuildErrorMessage(pReplyMsg, XCP_ERR_CMD_UNKNOWN);
          break;
      }
    }
    else
    {
      /* no xcp communication in disconnected state! ignore command.*/
    }
  }

  /* send reply message */
  if(sendFlag)
  {
    XcpLight_SendMessage(pReplyMsg);
  }
}

int XcpLight_Event(uint8_t eventNo)
{
  if(_XcpLightData.sessionStatus & XCP_SES_DAQ_RUNNING)
  {
    return 0;
  }
  else
  {
    return -1;
  }
}
