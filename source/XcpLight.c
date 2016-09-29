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

/******************************************************************************/
/*** local area ***/
/******************************************************************************/

//------------------------------------------------------------------------------
// local macro constants
//------------------------------------------------------------------------------

/** VERSIONS **/
#define XCP_VER_DRIVER            (0x01u)
#define XCP_VER_PROTOCOL_LAYER    (0x01u)
#define XCP_VER_TRANSPORT_LAYER   (0x01u)


/** PACKET IDENTIFIER **/
#define XCP_PID_RES                     (0xFFu) ///< response
#define XCP_PID_ERR                     (0xFEu) ///< error
#define XCP_PID_EVT                     (0xFDu) ///< event
#define XCP_PID_SRV                     (0xFCu) ///< service
// note PID 0xFB ... 0x00 absolute or relative ODT number for DAQ


/** EVENT CODES **/
#define XCP_EVT_EV_RESUME_MODE          (0x00u)
#define XCP_EVT_EV_CLEAR_DAQ            (0x01u)
#define XCP_EVT_EV_STORE_DAQ            (0x02u)
#define XCP_EVT_EV_STORE_CAL            (0x03u)
#define XCP_EVT_EV_CMD_PENDING          (0x05u)
#define XCP_EVT_EV_DAQ_OVERLOAD         (0x06u)
#define XCP_EVT_EV_SESSION_TERMINATED   (0x07u)
#define XCP_EVT_EV_TIME_SYNC            (0x08u)
#define XCP_EVT_EV_STIM_TIMEOUT         (0x09u)
#define XCP_EVT_EV_SLEEP                (0x0Au)
#define XCP_EVT_EV_WAKE_UP              (0x0Bu)
#define XCP_EVT_EV_USER                 (0xFEu)
#define XCP_EVT_EV_TRANSPORT            (0xFFu)


/** SERVICE REQUEST CODES **/
#define XCP_SRC_SERV_RESET              (0x00u)
#define XCP_SRC_SERV_TEXT               (0x01u)


/** ERROR CODES **/
#define XCP_ERR_CMD_SYNCH           (0x00u)
#define XCP_ERR_CMD_BUSY            (0x10u)
#define XCP_ERR_DAQ_ACTIVE          (0x11u)
#define XCP_ERR_PGM_ACTIVE          (0x12u)
#define XCP_ERR_CMD_UNKNOWN         (0x20u)
#define XCP_ERR_CMD_SYNTAX          (0x21u)
#define XCP_ERR_OUT_OF_RANGE        (0x22u)
#define XCP_ERR_WRITE_PROTECTED     (0x23u)
#define XCP_ERR_ACCESS_DENIED       (0x24u)
#define XCP_ERR_ACCESS_LOCKED       (0x25u)
#define XCP_ERR_PAGE_NOT_VALID      (0x26u)
#define XCP_ERR_MODE_NOT_VALID      (0x27u)
#define XCP_ERR_SEGMENT_NOT_VALID   (0x28u)
#define XCP_ERR_SEQUENCE            (0x29u)
#define XCP_ERR_DAQ_CONFIG          (0x2Au)
#define XCP_ERR_MEMORY_OVERFLOW     (0x30u)
#define XCP_ERR_GENERIC             (0x31u)
#define XCP_ERR_VERIFY              (0x32u)
#define XCP_ERR_RES_TEMP_NOT_ACC    (0x33u)
#define XCP_ERR_SUBCMD_UNKNOWN      (0x34u)


/** COMMAND CODES **/
/* Standard Commands (STD) */
#define XCP_CMD_CONNECT                 (0xFFu)
#define XCP_CMD_DISCONNECT              (0xFEu)
#define XCP_CMD_GET_STATUS              (0xFDu)
#define XCP_CMD_SYNCH                   (0xFCu)

#define XCP_CMD_GET_COMM_MODE_INFO      (0xFBu)
#define XCP_CMD_GET_ID                  (0xFAu)
#define XCP_CMD_SET_REQUEST             (0xF9u)
#define XCP_CMD_GET_SEED                (0xF8u)
#define XCP_CMD_UNLOCK                  (0xF7u)
#define XCP_CMD_SET_MTA                 (0xF6u)
#define XCP_CMD_UPLOAD                  (0xF5u)
#define XCP_CMD_SHORT_UPLOAD            (0xF4u)
#define XCP_CMD_BUILD_CHECKSUM          (0xF3u)

#define XCP_CMD_TRANSPORT_LAYER_CMD     (0xF2u)
#define XCP_CMD_USER_CMD                (0xF1u)

/* Calibration Commands (CAL) */
#define XCP_CMD_DOWNLOAD                (0xF0u)

#define XCP_CMD_DOWNLOAD_NEXT           (0xEFu)
#define XCP_CMD_DOWNLOAD_MAX            (0xEEu)
#define XCP_CMD_SHORT_DOWNLOAD          (0xEDu)
#define XCP_CMD_MODIFY_BITS             (0xECu)

/* Page Switching Commands (PAG)*/
#define XCP_CMD_SET_CAL_PAGE            (0xEBu)
#define XCP_CMD_GET_CAL_PAGE            (0xEAu)
#define XCP_CMD_GET_PAG_PROCESSOR_INFO  (0xE9u)
#define XCP_CMD_GET_SEGMENT_INFO        (0xE8u)
#define XCP_CMD_GET_PAGE_INFO           (0xE7u)
#define XCP_CMD_SET_SEGMENT_MODE        (0xE6u)
#define XCP_CMD_GET_SEGMENT_MODE        (0xE5u)
#define XCP_CMD_COPY_CAL_PAGE           (0xE4u)

/* Data Aquisition and Stimulation Commands (DAQ) */
#define XCP_CMD_SET_DAQ_PTR             (0xE2u)
#define XCP_CMD_WRITE_DAQ               (0xE1u)
#define XCP_CMD_SET_DAQ_LIST_MODE       (0xE0u)
#define XCP_CMD_START_STOP_DAQ_LIST     (0xDEu)
#define XCP_CMD_START_STOP_SYNCH        (0xDDu)

#define XCP_CMD_WRITE_DAQ_MULTIPLE      (0xC7u)
#define XCP_CMD_READ_DAQ                (0xDBu)
#define XCP_CMD_GET_DAQ_CLOCK           (0xDCu)
#define XCP_CMD_GET_DAQ_PROCESSOR_INFO  (0xDAu)
#define XCP_CMD_GET_DAQ_RESOLUTION_INFO (0xD9u)
#define XCP_CMD_GET_DAQ_LIST_MODE       (0xDFu)
#define XCP_CMD_GET_DAQ_EVENT_INFO      (0xD7u)
// static configuration
#define XCP_CMD_CLEAR_DAQ_LIST          (0xE3u)
#define XCP_CMD_GET_DAQ_LIST_INFO       (0xD8u)
// dynamic configuration
#define XCP_CMD_FREE_DAQ                (0xD6u)
#define XCP_CMD_ALLOC_DAQ               (0xD5u)
#define XCP_CMD_ALLOC_ODT               (0xD4u)
#define XCP_CMD_ALLOC_ODT_ENTRY         (0xD3u)

/* Non-volatile Memory Programming Commands (PGM) */
#define XCP_CMD_PROGRAM_START           (0xD2u)
#define XCP_CMD_PROGRAM_CLEAR           (0xD1u)
#define XCP_CMD_PROGRAM                 (0xD0u)
#define XCP_CMD_PROGRAM_RESET           (0xCFu)

#define XCP_CMD_GET_PGM_PROCESSOR_INFO  (0xCEu)
#define XCP_CMD_GET_SECTOR_INFO         (0xCDu)
#define XCP_CMD_PROGRAM_PREPARE         (0xCCu)
#define XCP_CMD_PROGRAM_FORMAT          (0xCBu)
#define XCP_CMD_PROGRAM_NEXT            (0xCAu)
#define XCP_CMD_PROGRAM_MAX             (0xC9u)
#define XCP_CMD_PROGRAM_VERIFY          (0xC8u)

/** RESOURCE CODES **/
#define XCP_RES_CALPAG              (0x01u)
#define XCP_RES_DAQ                 (0x04u)
#define XCP_RES_STIM                (0x08u)
#define XCP_RES_PGM                 (0x10u)

/** SESSION STATUS **/
#define XCP_SES_RESET_SESSION       (0x00u)
#define XCP_SES_STORE_CAL_REQ       (0x01u)
#define XCP_SES_CONNECTED           (0x02u) /* Reserve 1: used for connected flag */
#define XCP_SES_STORE_DAQ_REQ       (0x04u)
#define XCP_SES_CLEAR_DAQ_REQ       (0x08u)
#define XCP_SES_RESERVE2            (0x10u)
#define XCP_SES_RESERVE3            (0x20u)
#define XCP_SES_DAQ_RUNNING         (0x40u)
#define XCP_SES_RESUME              (0x80u)

/** PROTECTION STATUS **/
#define XCP_PRT_PROTECT_ALL         (0xFFu)
#define XCP_PRT_CAL_PAG             (0x01u)
#define XCP_PRT_RESERVE1            (0x02u)
#define XCP_PRT_DAQ                 (0x04u)
#define XCP_PRT_STIM                (0x08u)
#define XCP_PRT_PGM                 (0x10u)
#define XCP_PRT_RESERVE2            (0x20u)
#define XCP_PRT_RESERVE3            (0x40u)
#define XCP_PRT_RESERVE4            (0x80u)

//------------------------------------------------------------------------------
// local struct and type definitions (struct, enum and typedef)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// local data
//------------------------------------------------------------------------------

/* internal data structure for state etc. */
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
  uint8_t sendFlag = 0;

  _XcpLightData.currentCommand = (pMsg->payload[0] & 0xFFu);

  if(_XcpLightData.currentCommand == XCP_CMD_CONNECT)
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

    sendFlag = 1;
  }
  else
  {
    if(_XcpLightData.sessionStatus & XCP_SES_CONNECTED)
    {
      /* in connected state process all other commands*/
      switch(_XcpLightData.currentCommand)
      {
        case XCP_CMD_DISCONNECT:
          {
            /* protect all resources */
            #ifdef XCPLIGHT_CFG_ENABLE_RESOURCE_PROTECTION
            _XcpLightData.protectionStatus = XCP_PRT_PROTECT_ALL;
            #endif

            /* reset all session status bits */
            _XcpLightData.sessionStatus    = XCP_SES_RESET_SESSION;

            pReplyMsg->length = 1u;
            pReplyMsg->payload[0] = XCP_PID_RES;
            sendFlag = 1;
          }
          break;

        case XCP_CMD_SYNCH:
          {
            _XcpLight_BuildErrorMessage(pReplyMsg, XCP_ERR_CMD_SYNCH);
            sendFlag = 1;
          }
          break;

        case XCP_CMD_GET_COMM_MODE_INFO:
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
            sendFlag = 1;
          }
          break;

        case XCP_CMD_GET_STATUS:
          {
            pReplyMsg->length = 6u;
            pReplyMsg->payload[0] = XCP_PID_RES;
            pReplyMsg->payload[1] = _XcpLightData.sessionStatus;
            pReplyMsg->payload[2] = _XcpLightData.protectionStatus;
            pReplyMsg->payload[3] = 0x00u; /* STATE_NUMBER: ECU_STATES not supported */

            pReplyMsg->payload[4] = 0x00u; // @todo: Session configuration id
            pReplyMsg->payload[5] = 0x00u; // @todo: Session configuration id
            sendFlag = 1;
          }
          break;

        case XCP_CMD_GET_DAQ_PROCESSOR_INFO:
          {
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
          }
          break;

        case XCP_CMD_GET_DAQ_RESOLUTION_INFO:
          {
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
          }
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
              _XcpLight_BuildErrorMessage(pReplyMsg, XCP_ERR_OUT_OF_RANGE);
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
            uint8_t length = (pMsg->payload[1] & 0xFFu);
            if(length < (XCPLIGHT_CFG_XTO_LENGTH - 1u))
            {
              XcpLight_WriteToAddress((uint8_t *)_XcpLightData.mta, length, &(pMsg->payload[2]));

              pReplyMsg->length = 1u;
              pReplyMsg->payload[0] = XCP_PID_RES;

              sendFlag = 1;
            }
            else
            {
              _XcpLight_BuildErrorMessage(pReplyMsg, XCP_ERR_OUT_OF_RANGE);
              sendFlag = 1;
            }
          }
          break;

        default:
          {
            _XcpLight_BuildErrorMessage(pReplyMsg, XCP_ERR_CMD_UNKNOWN);
            sendFlag = 1;
          }
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
