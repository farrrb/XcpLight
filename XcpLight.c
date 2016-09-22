#include "XcpLight.h"

/******************************************************************************/
/*** Todo list:
/***             0. fix all @todo points
/***             1. implement daq processor
/***             2. implement programming (flash file download)
/***             3. implement resource protection
/***             4. test test test
/******************************************************************************/

/******************************************************************************/
/*** local area ***/
/******************************************************************************/

//------------------------------------------------------------------------------
// local macro constants
//------------------------------------------------------------------------------

/** VERSIONS **/
#define XCP_VER_DRIVER            ((uint8_t)0x01)
#define XCP_VER_PROTOCOL_LAYER    ((uint8_t)0x01)
#define XCP_VER_TRANSPORT_LAYER   ((uint8_t)0x01)


/** PACKET IDENTIFIER **/
#define XCP_PID_RES                     ((uint8_t)0xFF) ///< response
#define XCP_PID_ERR                     ((uint8_t)0xFE) ///< error
#define XCP_PID_EVT                     ((uint8_t)0xFD) ///< event
#define XCP_PID_SRV                     ((uint8_t)0xFC) ///< service
// note PID 0xFB ... 0x00 absolute or relative ODT number for DAQ


/** EVENT CODES **/
#define XCP_EVT_EV_RESUME_MODE          ((uint8_t)0x00)
#define XCP_EVT_EV_CLEAR_DAQ            ((uint8_t)0x01)
#define XCP_EVT_EV_STORE_DAQ            ((uint8_t)0x02)
#define XCP_EVT_EV_STORE_CAL            ((uint8_t)0x03)
#define XCP_EVT_EV_CMD_PENDING          ((uint8_t)0x05)
#define XCP_EVT_EV_DAQ_OVERLOAD         ((uint8_t)0x06)
#define XCP_EVT_EV_SESSION_TERMINATED   ((uint8_t)0x07)
#define XCP_EVT_EV_TIME_SYNC            ((uint8_t)0x08)
#define XCP_EVT_EV_STIM_TIMEOUT         ((uint8_t)0x09)
#define XCP_EVT_EV_SLEEP                ((uint8_t)0x0A)
#define XCP_EVT_EV_WAKE_UP              ((uint8_t)0x0B)
#define XCP_EVT_EV_USER                 ((uint8_t)0xFE)
#define XCP_EVT_EV_TRANSPORT            ((uint8_t)0xFF)


/** SERVICE REQUEST CODES **/
#define XCP_SRC_SERV_RESET              ((uint8_t)0x00)
#define XCP_SRC_SERV_TEXT               ((uint8_t)0x01)


/** ERROR CODES **/
#define XCP_ERR_CMD_SYNCH         ((uint8_t)0x00)
#define XCP_ERR_CMD_BUSY          ((uint8_t)0x10)
#define XCP_ERR_DAQ_ACTIVE        ((uint8_t)0x11)
#define XCP_ERR_PGM_ACTIVE        ((uint8_t)0x12)
#define XCP_ERR_CMD_UNKNOWN       ((uint8_t)0x20)
#define XCP_ERR_CMD_SYNTAX        ((uint8_t)0x21)
#define XCP_ERR_OUT_OF_RANGE      ((uint8_t)0x22)
#define XCP_ERR_WRITE_PROTECTED   ((uint8_t)0x23)
#define XCP_ERR_ACCESS_DENIED     ((uint8_t)0x24)
#define XCP_ERR_ACCESS_LOCKED     ((uint8_t)0x25)
#define XCP_ERR_PAGE_NOT_VALID    ((uint8_t)0x26)
#define XCP_ERR_MODE_NOT_VALID    ((uint8_t)0x27)
#define XCP_ERR_SEGMENT_NOT_VALID ((uint8_t)0x28)
#define XCP_ERR_SEQUENCE          ((uint8_t)0x29)
#define XCP_ERR_DAQ_CONFIG        ((uint8_t)0x2A)
#define XCP_ERR_MEMORY_OVERFLOW   ((uint8_t)0x30)
#define XCP_ERR_GENERIC           ((uint8_t)0x31)
#define XCP_ERR_VERIFY            ((uint8_t)0x32)
#define XCP_ERR_RES_TEMP_NOT_ACC  ((uint8_t)0x33)
#define XCP_ERR_SUBCMD_UNKNOWN    ((uint8_t)0x34)


/** COMMAND CODES **/
/* Standard Commands (STD) */
#define XCP_CMD_CONNECT                 ((uint8_t)0xFF)
#define XCP_CMD_DISCONNECT              ((uint8_t)0xFE)
#define XCP_CMD_GET_STATUS              ((uint8_t)0xFD)
#define XCP_CMD_SYNCH                   ((uint8_t)0xFC)

#define XCP_CMD_GET_COMM_MODE_INFO      ((uint8_t)0xFB)
#define XCP_CMD_GET_ID                  ((uint8_t)0xFA)
#define XCP_CMD_SET_REQUEST             ((uint8_t)0xF9)
#define XCP_CMD_GET_SEED                ((uint8_t)0xF8)
#define XCP_CMD_UNLOCK                  ((uint8_t)0xF7)
#define XCP_CMD_SET_MTA                 ((uint8_t)0xF6)
#define XCP_CMD_UPLOAD                  ((uint8_t)0xF5)
#define XCP_CMD_SHORT_UPLOAD            ((uint8_t)0xF4)
#define XCP_CMD_BUILD_CHECKSUM          ((uint8_t)0xF3)

#define XCP_CMD_TRANSPORT_LAYER_CMD     ((uint8_t)0xF2)
#define XCP_CMD_USER_CMD                ((uint8_t)0xF1)

/* Calibration Commands (CAL) */
#define XCP_CMD_DOWNLOAD                ((uint8_t)0xF0)

#define XCP_CMD_DOWNLOAD_NEXT           ((uint8_t)0xEF)
#define XCP_CMD_DOWNLOAD_MAX            ((uint8_t)0xEE)
#define XCP_CMD_SHORT_DOWNLOAD          ((uint8_t)0xED)
#define XCP_CMD_MODIFY_BITS             ((uint8_t)0xEC)

/* Page Switching Commands (PAG)*/
#define XCP_CMD_SET_CAL_PAGE            ((uint8_t)0xEB)
#define XCP_CMD_GET_CAL_PAGE            ((uint8_t)0xEA)
#define XCP_CMD_GET_PAG_PROCESSOR_INFO  ((uint8_t)0xE9)
#define XCP_CMD_GET_SEGMENT_INFO        ((uint8_t)0xE8)
#define XCP_CMD_GET_PAGE_INFO           ((uint8_t)0xE7)
#define XCP_CMD_SET_SEGMENT_MODE        ((uint8_t)0xE6)
#define XCP_CMD_GET_SEGMENT_MODE        ((uint8_t)0xE5)
#define XCP_CMD_COPY_CAL_PAGE           ((uint8_t)0xE4)

/* Data Aquisition and Stimulation Commands (DAQ) */
#define XCP_CMD_SET_DAQ_PTR             ((uint8_t)0xE2)
#define XCP_CMD_WRITE_DAQ               ((uint8_t)0xE1)
#define XCP_CMD_SET_DAQ_LIST_MODE       ((uint8_t)0xE0)
#define XCP_CMD_START_STOP_DAQ_LIST     ((uint8_t)0xDE)
#define XCP_CMD_START_STOP_SYNCH        ((uint8_t)0xDD)

#define XCP_CMD_WRITE_DAQ_MULTIPLE      ((uint8_t)0xC7)
#define XCP_CMD_READ_DAQ                ((uint8_t)0xDB)
#define XCP_CMD_GET_DAQ_CLOCK           ((uint8_t)0xDC)
#define XCP_CMD_GET_DAQ_PROCESSOR_INFO  ((uint8_t)0xDA)
#define XCP_CMD_GET_DAQ_RESOLUTION_INFO ((uint8_t)0xD9)
#define XCP_CMD_GET_DAQ_LIST_MODE       ((uint8_t)0xDF)
#define XCP_CMD_GET_DAQ_EVENT_INFO      ((uint8_t)0xD7)
// static configuration
#define XCP_CMD_CLEAR_DAQ_LIST          ((uint8_t)0xE3)
#define XCP_CMD_GET_DAQ_LIST_INFO       ((uint8_t)0xD8)
// dynamic configuration
#define XCP_CMD_FREE_DAQ                ((uint8_t)0xD6)
#define XCP_CMD_ALLOC_DAQ               ((uint8_t)0xD5)
#define XCP_CMD_ALLOC_ODT               ((uint8_t)0xD4)
#define XCP_CMD_ALLOC_ODT_ENTRY         ((uint8_t)0xD3)

/* Non-volatile Memory Programming Commands (PGM) */
#define XCP_CMD_PROGRAM_START           ((uint8_t)0xD2)
#define XCP_CMD_PROGRAM_CLEAR           ((uint8_t)0xD1)
#define XCP_CMD_PROGRAM                 ((uint8_t)0xD0)
#define XCP_CMD_PROGRAM_RESET           ((uint8_t)0xCF)

#define XCP_CMD_GET_PGM_PROCESSOR_INFO  ((uint8_t)0xCE)
#define XCP_CMD_GET_SECTOR_INFO         ((uint8_t)0xCD)
#define XCP_CMD_PROGRAM_PREPARE         ((uint8_t)0xCC)
#define XCP_CMD_PROGRAM_FORMAT          ((uint8_t)0xCB)
#define XCP_CMD_PROGRAM_NEXT            ((uint8_t)0xCA)
#define XCP_CMD_PROGRAM_MAX             ((uint8_t)0xC9)
#define XCP_CMD_PROGRAM_VERIFY          ((uint8_t)0xC8)

#define XCP_RES_CALPAG  ((uint8_t)0x01)
#define XCP_RES_DAQ     ((uint8_t)0x04)
#define XCP_RES_STIM    ((uint8_t)0x08)
#define XCP_RES_PGM     ((uint8_t)0x10)

/** SESSION STATUS **/
#define XCP_SES_STORE_CAL_REQ       ((uint8_t)0x01)
#define XCP_SES_ACTIVE              ((uint8_t)0x02)
#define XCP_SES_STORE_DAQ_REQ       ((uint8_t)0x04)
#define XCP_SES_CLEAR_DAQ_REQ       ((uint8_t)0x08)
#define XCP_SES_RESERVE1            ((uint8_t)0x10)
#define XCP_SES_RESERVE2            ((uint8_t)0x20)
#define XCP_SES_DAQ_RUNNING         ((uint8_t)0x40)
#define XCP_SES_RESUME              ((uint8_t)0x80)

/** PROTECTION STATUS **/
#define XCP_PRT_CAL_PAG             ((uint8_t)0x01)
#define XCP_PRT_RESERVE1            ((uint8_t)0x02)
#define XCP_PRT_DAQ                 ((uint8_t)0x04)
#define XCP_PRT_STIM                ((uint8_t)0x08)
#define XCP_PRT_PGM                 ((uint8_t)0x10)
#define XCP_PRT_RESERVE2            ((uint8_t)0x20)
#define XCP_PRT_RESERVE3            ((uint8_t)0x40)
#define XCP_PRT_RESERVE4            ((uint8_t)0x80)

//------------------------------------------------------------------------------
// local struct and type definitions (struct, enum and typedef)
//------------------------------------------------------------------------------

typedef struct
{
  uint8_t sessionStatus;
  uint8_t protectionStatus;
  XcpLightMessage_t ctoReplyMsg;
  uint32_t mta;
} XcpLightInternals_t;

//------------------------------------------------------------------------------
// local data
//------------------------------------------------------------------------------

XcpLightInternals_t _XcpLightData = {0};

//------------------------------------------------------------------------------
// local macro functions
//------------------------------------------------------------------------------

#define Uint32FromUint8Ptr(ptr) (*((uint32_t *)(ptr)))

//#define Uint32FromUint8Ptr(ptr) ( ((*((ptr)+3)&0xFFu) << 24u) | \
//                                  ((*((ptr)+2)&0xFFu) << 16u) | \
//                                  ((*((ptr)+1)&0xFFu) <<  8u) | \
//                                  ((*((ptr)+0)&0xFFu) <<  0u))

//------------------------------------------------------------------------------
// local functions - prototypes
//------------------------------------------------------------------------------

void _XcpLight_BuildErrorMessage(XcpLightMessage_t * pMsg, uint8_t errorCode);

//------------------------------------------------------------------------------
// local functions
//------------------------------------------------------------------------------

/******************************************************************************/
/*** external area ***/
/******************************************************************************/

//------------------------------------------------------------------------------
// external data
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// external functions
//------------------------------------------------------------------------------
void XcpLight_Init(void)
{
  _XcpLightData.sessionStatus = 0;
  _XcpLightData.protectionStatus = 0;
}

void XcpLight_CommandProcessor(XcpLightMessage_t * pMsg)
{
  XcpLightMessage_t * pReplyMsg = &(_XcpLightData.ctoReplyMsg);
  uint8_t sendFlag = 0;

  switch(pMsg->payload[0])
  {
    case XCP_CMD_CONNECT:
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

      _XcpLightData.sessionStatus |= XCP_SES_ACTIVE;

      sendFlag = 1;
      break;

    case XCP_CMD_DISCONNECT:
      pReplyMsg->length = 1u;
      pReplyMsg->payload[0] = XCP_PID_RES;
      sendFlag = 1;
      break;

    case XCP_CMD_GET_COMM_MODE_INFO:
      pReplyMsg->length = 8u;
      pReplyMsg->payload[0] = XCP_PID_RES;
      pReplyMsg->payload[1] = 0x00u; // reserved
      pReplyMsg->payload[2] = 0x00u; // @todo: COMM_MODE_OPTIONAL
      pReplyMsg->payload[3] = 0x00u; // reserved
      pReplyMsg->payload[4] = 0x00u; // @todo: MAX_BS
      pReplyMsg->payload[5] = 0x00u; // @todo: MAX_ST
      pReplyMsg->payload[6] = 0x00u; // @todo: QUEUE_SIZE
      pReplyMsg->payload[7] = XCP_VER_DRIVER;
      sendFlag = 1;
      break;

    case XCP_CMD_GET_STATUS:
      pReplyMsg->length = 6u;
      pReplyMsg->payload[0] = XCP_PID_RES;
      pReplyMsg->payload[1] = _XcpLightData.sessionStatus;
      pReplyMsg->payload[2] = _XcpLightData.protectionStatus;
      pReplyMsg->payload[3] = 0x00u; // @todo: STATE_NUMBER

      pReplyMsg->payload[4] = 0x00u; // @todo: Session configuration id
      pReplyMsg->payload[5] = 0x00u; // @todo: Session configuration id
      sendFlag = 1;
      break;

    case XCP_CMD_GET_DAQ_PROCESSOR_INFO:
      pReplyMsg->length = 8u;
      pReplyMsg->payload[0] = XCP_PID_RES;
      pReplyMsg->payload[1] = 0x00u; // @todo: DAQ_PROPERTIES

      pReplyMsg->payload[2] = 0x00u; // @todo: MAX_DAQ
      pReplyMsg->payload[3] = 0x00u; // @todo: MAX_DAQ

      pReplyMsg->payload[4] = 0x00u; // @todo: MAX_EVENT_CHANNEL
      pReplyMsg->payload[5] = 0x00u; // @todo: MAX_EVENT_CHANNEL

      pReplyMsg->payload[6] = 0x00u; // @todo: MIN_DAQ
      pReplyMsg->payload[7] = 0x00u; // @todo: DAQ_KEY_BYTE
      sendFlag = 1;
      break;

    case XCP_CMD_GET_DAQ_RESOLUTION_INFO:
      pReplyMsg->length = 8u;
      pReplyMsg->payload[0] = XCP_PID_RES;
      pReplyMsg->payload[1] = 0x00u; // @todo: GRANULARITY_ODT_ENTRY_SIZE_DAQ
      pReplyMsg->payload[2] = 0x00u; // @todo: MAX_ODT_ENTRY_SIZE_DAQ
      pReplyMsg->payload[3] = 0x00u; // @todo: GRANULARITY_ODT_ENTRY_SIZE_STIM
      pReplyMsg->payload[4] = 0x00u; // @todo: MAX_ODT_ENTRY_SIZE_STIM
      pReplyMsg->payload[5] = 0x00u; // @todo: TIMESTAMP_MODE

      pReplyMsg->payload[6] = 0x00u; // @todo: TIMESTAMP_TICKS
      pReplyMsg->payload[7] = 0x00u; // @todo: TIMESTAMP_TICKS
      sendFlag = 1;
      break;

    case XCP_CMD_SHORT_UPLOAD:
      {
        uint32_t tmpAddress = 0;
        uint8_t  tmpAddressExt = 0;
        uint8_t  length = (pMsg->payload[1] & 0xFFu);

        if(length < (XCPLIGHT_CFG_XTO_LENGTH - 1u))
        {
          pReplyMsg->length = length + 1u;
          pReplyMsg->payload[0] = XCP_PID_RES;
          tmpAddressExt  = (pMsg->payload[3] & 0xFFu);
          tmpAddress = Uint32FromUint8Ptr(&(pMsg->payload[4]));
          _XcpLightData.mta = XcpLight_GetPointer(tmpAddress, tmpAddressExt);
          XcpLight_ReadFromAddress((uint8_t *)_XcpLightData.mta, length, &(pReplyMsg->payload[1]));
          sendFlag = 1;
        }
        else
        {
          _XcpLight_BuildErrorMessage(&pReplyMsg-> XCP_ERR_OUT_OF_RANGE);
          sendFlag = 1;
        }
      }
      break;

    case XCP_CMD_SET_MTA:
      {
        uint32_t tmpAddress = 0;
        uint8_t  tmpAddressExt = 0;

        pReplyMsg->length = 2u;
        pReplyMsg->payload[0] = XCP_PID_RES;
        pReplyMsg->payload[1] = 0x00u;

        tmpAddressExt = (pMsg->payload[3] & 0xFFu);
        tmpAddress = Uint32FromUint8Ptr(&(pMsg->payload[4]));
        _XcpLightData.mta = XcpLight_GetPointer(tmpAddress, tmpAddressExt);

        sendFlag = 1;
      }

      break;
    case XCP_CMD_DOWNLOAD:
      {
        uint8_t noElements; // address granularity!
        // @todo fill me
      }
      break;

    default:
      _XcpLight_BuildErrorMessage(pReplyMsg, XCP_ERR_CMD_UNKNOWN);
      sendFlag = 1;
      break;
  }

  if(sendFlag)
  {
    XcpLight_SendMessage(pReplyMsg);
  }
}

void _XcpLight_BuildErrorMessage(XcpLightMessage_t * pMsg, uint8_t errorCode)
{
  pMsg->length = 2u;
  pMsg->payload[0] = XCP_PID_ERR;
  pMsg->payload[1] = errorCode;
}

int XcpLight_Event(uint8_t eventNo)
{
  return 0;
}
