/* MIT License see LICENSE file             */
/* - Copyright (c) 2016 0xFAB - Fabian Zahn */

#ifndef __XCPLIGHT_INTERNALS_H
#define __XCPLIGHT_INTERNALS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "XcpLight_config.h"
#include "XcpLightMem.h"

/*****************************************************************************/
/* macro constants                                                           */
/*****************************************************************************/

/** PACKET IDENTIFIER **/
#define XCP_PID_RES                     (0xFFu) ///< response
#define XCP_PID_ERR                     (0xFEu) ///< error
#define XCP_PID_EVT                     (0xFDu) ///< event
#define XCP_PID_SRV                     (0xFCu) ///< service
/* note PID 0xFB ... 0x00 absolute or relative ODT number for DAQ */

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

/** DAQ Timestamp Units **/
#define XCP_DAQ_TIMESTAMP_UNIT_1NS    (0x0u)
#define XCP_DAQ_TIMESTAMP_UNIT_10NS   (0x1u)
#define XCP_DAQ_TIMESTAMP_UNIT_100NS  (0x2u)
#define XCP_DAQ_TIMESTAMP_UNIT_1US    (0x3u)
#define XCP_DAQ_TIMESTAMP_UNIT_10US   (0x4u)
#define XCP_DAQ_TIMESTAMP_UNIT_100US  (0x5u)
#define XCP_DAQ_TIMESTAMP_UNIT_1MS    (0x6u)
#define XCP_DAQ_TIMESTAMP_UNIT_10MS   (0x7u)
#define XCP_DAQ_TIMESTAMP_UNIT_100MS  (0x8u)
#define XCP_DAQ_TIMESTAMP_UNIT_1S     (0x9u)
#define XCP_DAQ_TIMESTAMP_UNIT_1PS    (0xAu)
#define XCP_DAQ_TIMESTAMP_UNIT_10PS   (0xBu)
#define XCP_DAQ_TIMESTAMP_UNIT_100PS  (0xCu)

/*****************************************************************************/
/* struct and type definitions (struct, enum and typedef)                    */
/*****************************************************************************/

/* odt entry */
typedef struct XcpLightOdtEntry
{
  /* @todo: revisit me */
  struct XcpLightOdtEntry * pNext; /* pointer to next odt entry */
} XcpLightOdtEntry_t;

/* odt */
typedef struct XcpLightOdt
{
  /* @todo: revisit me */
  struct XcpLightOdt * pNext; /* pointer to next odt*/
} XcpLightOdt_t;

/* daq list */
typedef struct XcpLightDaqList
{
  /* @todo: revisit me */
  uint8_t eventChannel;
  uint8_t priority;
  XcpLightOdt_t * pOdt;

  struct XcpLightDaqList * pNext; /* pointer to next daq list */
} XcpLightDaqList_t;

/* daq processor */
typedef struct XcpLightDaqProcessor
{
  XcpLightMemory_t mem; /* memory management struct for daq lists & samples */
  uint8_t daqMemoryBuffer[XCPLIGHT_CFG_DAQ_MEMORY_SIZE]; /* raw mem -> do not use this directly! */

  XcpLightDaqList_t * pList;
  uint8_t daqListCount;
  uint8_t daqOdtCount;
} XcpLightDaqProcessor_t;

/* internal data structure */
typedef struct XcpLightInternals
{
  uint32_t timestampCounter;
  uint8_t sessionStatus;
  uint8_t protectionStatus;
  uint8_t currentCommand;
#ifdef XCPLIGHT_CFG_DEBUG_CMD_MSG
  XcpLightMessage_t ctoCmdMsg;
#endif
  XcpLightMessage_t ctoReplyMsg;
  void * mta;

  /* DAQ */
#ifdef XCPLIGHT_CFG_ENABLE_DAQ
  XcpLightDaqProcessor_t daqProcessor;
  #endif /* XCPLIGHT_CFG_ENABLE_DAQ */

} XcpLightInternals_t;


#ifdef __cplusplus
}
#endif

#endif // __XCPLIGHT_INTERNALS_H
