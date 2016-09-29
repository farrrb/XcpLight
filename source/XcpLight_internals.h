#ifndef __XCPLIGHT_INTERNALS_H
#define __XCPLIGHT_INTERNALS_H

#ifdef __cplusplus
extern "C"
{
#endif

//------------------------------------------------------------------------------
// macro constants
//------------------------------------------------------------------------------

// @todo move all the command codes here

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
// struct and type definitions (struct, enum and typedef)
//------------------------------------------------------------------------------

/* internal data structure */
typedef struct
{
  uint32_t timestampCounter;
  uint8_t sessionStatus;
  uint8_t protectionStatus;
  uint8_t currentCommand;
  XcpLightMessage_t ctoReplyMsg;
  uint32_t mta;
} XcpLightInternals_t;


#ifdef __cplusplus
}
#endif

#endif // __XCPLIGHT_INTERNALS_H
