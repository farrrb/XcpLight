  ////////////////////////////////////////////////////////////////////////////////
/// MIT License see LICENSE file
/// Copyright (c) 2016-2019 0xFAB - Fabian Zahn
////////////////////////////////////////////////////////////////////////////////

#include "XcpLight.h"
#include "XcpLightInternals.h"

//------------------------------------------------------------------------------
// Local Macro Constants
//------------------------------------------------------------------------------

//* VERSIONS *
#define XCP_VER_DRIVER            (0x01u)
#define XCP_VER_PROTOCOL_LAYER    (0x01u)
#define XCP_VER_TRANSPORT_LAYER   (0x01u)

//* EVALUATE CONFIG *
#ifdef XCPLIGHT_CFG_DISABLE_STATIC_INLINE
  #define XCP_STATIC_INLINE static
#else
  #define XCP_STATIC_INLINE static inline
#endif

//------------------------------------------------------------------------------
// Local Struct and Type Definitions (struct, enum and typedef)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Local Data
//------------------------------------------------------------------------------

// internal data structure for state etc.
XcpLightInternals_t XcpLight_self =
{
#ifdef XCPLIGHT_CFG_DEBUG_CMD_MSG
  {0, {0}},
#endif
  {0, {0}},
  NULL,
  0,
  0,
  0,
  0,
#ifdef XCPLIGHT_CFG_SEED_AND_KEY
  0,
  {0},
  0,
  0,
  {0},
  0
#endif // XCPLIGHT_CFG_SEED_AND_KEY
};

//------------------------------------------------------------------------------
// local macro functions
//------------------------------------------------------------------------------
#define MARK_UNUSED(param)            ((void)param)
#define INCREMENT_PTR(ptr, bytes)     ((void *)((uint32_t)(ptr) + (uint32_t)(bytes)))

//------------------------------------------------------------------------------
// Local Functions Prototypes
//------------------------------------------------------------------------------

XCP_STATIC_INLINE uint32_t buildU32FromU8Array(uint8_t *array);
XCP_STATIC_INLINE bool buildErrorMessage(XcpLightMessage_t *msg, uint8_t error_code);

XCP_STATIC_INLINE bool cmdConnect(XcpLightMessage_t *msg, XcpLightMessage_t *reply_msg);
XCP_STATIC_INLINE bool cmdDisconnect(XcpLightMessage_t *msg, XcpLightMessage_t *reply_msg);
XCP_STATIC_INLINE bool cmdGetStatus(XcpLightMessage_t *msg, XcpLightMessage_t *reply_msg);
XCP_STATIC_INLINE bool cmdSynch(XcpLightMessage_t *msg, XcpLightMessage_t *reply_msg);

XCP_STATIC_INLINE bool cmdGetCommModeInfo(XcpLightMessage_t *msg, XcpLightMessage_t *reply_msg);

#ifdef XCPLIGHT_CFG_SEED_AND_KEY
XCP_STATIC_INLINE bool cmdGetSeed(XcpLightMessage_t *msg, XcpLightMessage_t *reply_msg);
XCP_STATIC_INLINE bool cmdUnlock(XcpLightMessage_t *msg, XcpLightMessage_t *reply_msg);
#endif // XCPLIGHT_CFG_SEED_AND_KEY

#ifdef XCPLIGHT_CFG_USER_CMD
XCP_STATIC_INLINE bool cmdUserCmd(XcpLightMessage_t *msg, XcpLightMessage_t *reply_msg);
#endif // XCPLIGHT_CFG_USER_CMD

XCP_STATIC_INLINE bool cmdSetMta(XcpLightMessage_t *msg, XcpLightMessage_t *reply_msg);
XCP_STATIC_INLINE bool cmdUpload(XcpLightMessage_t *msg, XcpLightMessage_t *reply_msg);
XCP_STATIC_INLINE bool cmdShortUpload(XcpLightMessage_t *msg, XcpLightMessage_t *reply_msg);

#ifdef XCPLIGHT_CFG_ENABLE_CALPAG
XCP_STATIC_INLINE bool cmdDownload(XcpLightMessage_t *msg, XcpLightMessage_t *reply_msg);
#endif // XCPLIGHT_CFG_ENABLE_CALPAG

//------------------------------------------------------------------------------
// Local Functions
//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
XCP_STATIC_INLINE uint32_t buildU32FromU8Array(uint8_t *array)
{
  uint32_t value;

  value  = (array[0] & 0xFFu) <<  0u;
  value |= (array[1] & 0xFFu) <<  8u;
  value |= (array[2] & 0xFFu) << 16u;
  value |= (array[3] & 0xFFu) << 24u;

  return value;
}

////////////////////////////////////////////////////////////////////////////////
XCP_STATIC_INLINE bool buildErrorMessage(XcpLightMessage_t *msg, uint8_t error_code)
{
  msg->length     = 2u;
  msg->payload[0] = XCP_PID_ERR;
  msg->payload[1] = error_code;

  return true;
}

////////////////////////////////////////////////////////////////////////////////
XCP_STATIC_INLINE bool cmdConnect(XcpLightMessage_t *msg, XcpLightMessage_t *reply_msg)
{
  MARK_UNUSED(msg);

  // process the connect command anytime -> independence of state
  reply_msg->length     = 8u;
  reply_msg->payload[0] = XCP_PID_RES;
  reply_msg->payload[1] = 0u; // default

#ifdef XCPLIGHT_CFG_ENABLE_CALPAG
  reply_msg->payload[1] |= XCP_RES_CALPAG;
#endif

#ifdef XCPLIGHT_CFG_ENABLE_DAQ
  reply_msg->payload[1] |= XCP_RES_DAQ;
#endif

#ifdef XCPLIGHT_CFG_ENABLE_STIM
  reply_msg->payload[1] |= XCP_RES_STIM;
#endif

#ifdef XCPLIGHT_CFG_ENABLE_PGM
  reply_msg->payload[1] |= XCP_RES_PGM;
#endif

  // COMM_MODE_BASIC
  reply_msg->payload[2]  = 0x80u; // set optional bit?

#ifndef XCPLIGHT_CFG_LITTLE_ENDIAN
  reply_msg->payload[2] |= 0x01u; // big endian
#endif

  reply_msg->payload[2] |= (((XCPLIGHT_CFG_ADDRESS_GRANULARITY >> 1u) & 0x03u) << 1u);

#ifdef XCPLIGHT_CFG_BLOCK_MODE
  reply_msg->payload[2] |= (1 << 6u);
#endif


  reply_msg->payload[3] = XCPLIGHT_CFG_XTO_LENGTH;
  reply_msg->payload[4] = XCPLIGHT_CFG_XTO_LENGTH;
  reply_msg->payload[5] = 0x00u; // reserved
  reply_msg->payload[6] = XCP_VER_PROTOCOL_LAYER;
  reply_msg->payload[7] = XCP_VER_TRANSPORT_LAYER;

  // protect all resources (if seed & key is activated)
#ifdef XCPLIGHT_CFG_SEED_AND_KEY
  XcpLight_self.protection_status = XCP_PRT_PROTECT_ALL;
#else
  XcpLight_self.protection_status = XCP_PRT_PROTECT_NONE;
#endif

  // set session to connected state
  XcpLight_self.session_status |= XCP_SES_CONNECTED; // mark session as active (connected)

  return true;
}

////////////////////////////////////////////////////////////////////////////////
XCP_STATIC_INLINE bool cmdDisconnect(XcpLightMessage_t *msg, XcpLightMessage_t *reply_msg)
{
  MARK_UNUSED(msg);

  // protect all resources
  XcpLight_self.protection_status = XCP_PRT_PROTECT_ALL;

  // reset all session status bits
  XcpLight_self.session_status    = XCP_SES_RESET_SESSION;

  reply_msg->length     = 1u;
  reply_msg->payload[0] = XCP_PID_RES;

  return true;
}

////////////////////////////////////////////////////////////////////////////////
XCP_STATIC_INLINE bool cmdGetStatus(XcpLightMessage_t *msg, XcpLightMessage_t *reply_msg)
{
  MARK_UNUSED(msg);

  reply_msg->length     = 6u;
  reply_msg->payload[0] = XCP_PID_RES;
  reply_msg->payload[1] = XcpLight_self.session_status;
  reply_msg->payload[2] = XcpLight_self.protection_status;
  reply_msg->payload[3] = 0x00u; // STATE_NUMBER: ECU_STATES not supported

  reply_msg->payload[4] = 0x00u; // @todo: Session configuration id
  reply_msg->payload[5] = 0x00u; // @todo: Session configuration id

  return true;
}

////////////////////////////////////////////////////////////////////////////////
XCP_STATIC_INLINE bool cmdSynch(XcpLightMessage_t *msg, XcpLightMessage_t *reply_msg)
{
  MARK_UNUSED(msg);

  return buildErrorMessage(reply_msg, XCP_ERR_CMD_SYNCH);
}

////////////////////////////////////////////////////////////////////////////////
XCP_STATIC_INLINE bool cmdGetCommModeInfo(XcpLightMessage_t *msg, XcpLightMessage_t *reply_msg)
{
  MARK_UNUSED(msg);

  reply_msg->length     = 8u;
  reply_msg->payload[0] = XCP_PID_RES;
  reply_msg->payload[1] = 0x00u; // reserved
  reply_msg->payload[2] = 0x00u; // @todo: COMM_MODE_OPTIONAL
  reply_msg->payload[3] = 0x00u; // reserved
  reply_msg->payload[4] = 0x00u; // @todo: MAX_BS
  reply_msg->payload[5] = 0x00u; // @todo: MAX_ST
  reply_msg->payload[6] = 0x00u; // @todo: QUEUE_SIZE
  reply_msg->payload[7] = XCP_VER_DRIVER;

  return true;
}

#ifdef XCPLIGHT_CFG_SEED_AND_KEY
////////////////////////////////////////////////////////////////////////////////
XCP_STATIC_INLINE bool cmdGetSeed(XcpLightMessage_t *msg, XcpLightMessage_t *reply_msg)
{
  uint8_t mode;
  uint8_t seed_bytes;
  int i;
  bool transfer_seed_msg = false;

  mode = (msg->payload[1] & 0xFFu);
  if (mode == 0u) // first part
  {
    XcpLight_self.seed_index = 0u;
    XcpLight_self.unlock_resource = msg->payload[2] & 0xFFu;

    if (XcpLight_self.protection_status & XcpLight_self.unlock_resource) // resource is protected
    {
      XcpLightCfg_getSeed(XcpLight_self.unlock_resource , XcpLight_self.seed);
      XcpLight_self.remaining_seed_bytes = XCPLIGHT_CFG_SEED_LENGTH;
      transfer_seed_msg = true; // transmit first bytes of seed
    }
    else // resource is unprotected
    {
      reply_msg->length = 2u;
      reply_msg->payload[0] = XCP_PID_RES;
      reply_msg->payload[0] = 0u; // resource is not protected
    }
  }
  else // remaining length of seed
  {
    if (XcpLight_self.remaining_seed_bytes == 0u)
    {
      XcpLight_self.seed_index = 0u;
      buildErrorMessage(reply_msg, XCP_ERR_SEQUENCE);
    }
    else
    {
      transfer_seed_msg = true; // transmit consecutive bytes of seed
    }
  }

  // prepare message with seed data
  if (transfer_seed_msg == true)
  {

    // assemble payload
    reply_msg->payload[0] = XCP_PID_RES;
    if (mode == 0u)
    {
      reply_msg->payload[1] = XCPLIGHT_CFG_SEED_LENGTH;
    }
    else
    {
      reply_msg->payload[1] = XcpLight_self.remaining_seed_bytes & 0xFFu;
    }

    // send XCPLIGHT_CFG_XTO_LENGTH-2 bytes
    if (XcpLight_self.remaining_seed_bytes > (XCPLIGHT_CFG_XTO_LENGTH - 2))
    {
      reply_msg->length = XCPLIGHT_CFG_XTO_LENGTH;

      seed_bytes = (XCPLIGHT_CFG_XTO_LENGTH - 2);
      XcpLight_self.remaining_seed_bytes -= (XCPLIGHT_CFG_XTO_LENGTH - 2);
    }
    else
    {
      reply_msg->length = 2 + XcpLight_self.remaining_seed_bytes;

      seed_bytes = XcpLight_self.remaining_seed_bytes;
      XcpLight_self.remaining_seed_bytes = 0u;
    }

    for (i = 0; i < seed_bytes; i++)
    {
      reply_msg->payload[2+i] = XcpLight_self.seed[XcpLight_self.seed_index++] & 0xFFu;
    }
  }

  return true;
}

////////////////////////////////////////////////////////////////////////////////
XCP_STATIC_INLINE bool cmdUnlock(XcpLightMessage_t *msg, XcpLightMessage_t *reply_msg)
{
  uint8_t remaining_key_bytes = (msg->payload[1] & 0xFFu);
  uint8_t key_bytes;
  int i;
  int calculate_key = false;

  if (remaining_key_bytes > XCPLIGHT_CFG_KEY_LENGTH)
  {
    XcpLight_self.key_index = 0;
    buildErrorMessage(reply_msg, XCP_ERR_OUT_OF_RANGE);
  }
  else
  {
    if (remaining_key_bytes != 0)
    {
      if (remaining_key_bytes > (XCPLIGHT_CFG_XTO_LENGTH - 2))
      {
        key_bytes = (XCPLIGHT_CFG_XTO_LENGTH - 2);
      }
      else
      {
        key_bytes = remaining_key_bytes;
        calculate_key = true;
      }

      for (i = 0; i < key_bytes; i++)
      {
        XcpLight_self.key[XcpLight_self.key_index++] = msg->payload[2+i];
      }

    }
    else
    {
      key_bytes = (msg->length & 0xFFu) - 2u;

      for (i = 0; i < key_bytes; i++)
      {
        XcpLight_self.key[XcpLight_self.key_index++] = msg->payload[2+i] & 0xFFu;
      }
      calculate_key = true;
    }
  }

  if (calculate_key == true)
  {
    if (XcpLightCfg_unlockResource(XcpLight_self.unlock_resource, XcpLight_self.key))
      {
        XcpLight_self.key_index = 0;
        XcpLight_self.protection_status &= (~(XcpLight_self.unlock_resource) & 0xFFu);

        reply_msg->length     = 2u;
        reply_msg->payload[0] = XCP_PID_RES;
        reply_msg->payload[1] = XcpLight_self.protection_status;
      }
      else
      {
        XcpLight_self.key_index = 0;
        buildErrorMessage(reply_msg, XCP_ERR_ACCESS_LOCKED);
      }
  }

  return true;
}
#endif // XCPLIGHT_CFG_SEED_AND_KEY

#ifdef XCPLIGHT_CFG_USER_CMD
////////////////////////////////////////////////////////////////////////////////
XCP_STATIC_INLINE bool cmdUserCmd(XcpLightMessage_t *msg, XcpLightMessage_t *reply_msg)
{
  return XcpLightCfg_processUserCommand(msg, reply_msg);
}

#endif // XCPLIGHT_CFG_USER_CMD

////////////////////////////////////////////////////////////////////////////////
XCP_STATIC_INLINE bool cmdSetMta(XcpLightMessage_t *msg, XcpLightMessage_t *reply_msg)
{
  uint32_t tmp_address = 0;
  uint8_t  tmp_address_ext = 0;

  reply_msg->length     = 2u;
  reply_msg->payload[0] = XCP_PID_RES;
  reply_msg->payload[1] = 0x00u;

  tmp_address_ext = (msg->payload[3] & 0xFFu);

  tmp_address = buildU32FromU8Array(&(msg->payload[4]));

  XcpLight_self.mta = XcpLightCfg_getPointer(tmp_address, tmp_address_ext);

  return true;
}

////////////////////////////////////////////////////////////////////////////////
XCP_STATIC_INLINE bool cmdUpload(XcpLightMessage_t *msg, XcpLightMessage_t *reply_msg)
{
  uint8_t length = (msg->payload[1] & 0xFFu);

  if (length < (XCPLIGHT_CFG_XTO_LENGTH - 1u))
  {
    reply_msg->length     = length + 1u;
    reply_msg->payload[0] = XCP_PID_RES;

    XcpLightCfg_readFromAddress((uint8_t *)XcpLight_self.mta, length, &(reply_msg->payload[1]));
    XcpLight_self.mta = INCREMENT_PTR(XcpLight_self.mta, length);
  }
  else
  {
    buildErrorMessage(reply_msg, XCP_ERR_OUT_OF_RANGE);
  }

  return true;
}

////////////////////////////////////////////////////////////////////////////////
XCP_STATIC_INLINE bool cmdShortUpload(XcpLightMessage_t *msg, XcpLightMessage_t *reply_msg)
{
  uint32_t tmp_address;
  uint8_t  tmp_address_ext = 0;
  uint8_t  length = (msg->payload[1] & 0xFFu);

  if (length < (XCPLIGHT_CFG_XTO_LENGTH - 1u))
  {
    reply_msg->length     = length + 1u;
    reply_msg->payload[0] = XCP_PID_RES;
    tmp_address_ext       = (msg->payload[3] & 0xFFu);

    tmp_address           = buildU32FromU8Array(&(msg->payload[4]));
    XcpLight_self.mta     = XcpLightCfg_getPointer(tmp_address, tmp_address_ext);

    XcpLightCfg_readFromAddress((uint8_t *)XcpLight_self.mta, length, &(reply_msg->payload[1]));
    XcpLight_self.mta = INCREMENT_PTR(XcpLight_self.mta, length);
  }
  else
  {
    buildErrorMessage(reply_msg, XCP_ERR_OUT_OF_RANGE);
  }

  return true;
}

#ifdef XCPLIGHT_CFG_ENABLE_CALPAG
////////////////////////////////////////////////////////////////////////////////
XCP_STATIC_INLINE bool cmdDownload(XcpLightMessage_t *msg, XcpLightMessage_t *reply_msg)
{
  if (XcpLight_self.protection_status & XCP_PRT_CAL_PAG)
  {
    buildErrorMessage(reply_msg, XCP_ERR_ACCESS_LOCKED);
  }
  else
  {
    uint8_t length = (msg->payload[1] & 0xFFu);
    if (length < (XCPLIGHT_CFG_XTO_LENGTH - 1u))
    {
      XcpLightCfg_writeToAddress((uint8_t *)XcpLight_self.mta, length, &(msg->payload[2]));
      XcpLight_self.mta = INCREMENT_PTR(XcpLight_self.mta, length);

      reply_msg->length     = 1u;
      reply_msg->payload[0] = XCP_PID_RES;
    }
    else
    {
      buildErrorMessage(reply_msg, XCP_ERR_OUT_OF_RANGE);
    }
  }

  return true;
}
#endif // XCPLIGHT_CFG_ENABLE_CALPAG

//------------------------------------------------------------------------------
// External Functions
//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
void XcpLight_init(void)
{
  XcpLight_self.timestamp_counter     = 0u;
  XcpLight_self.session_status        = XCP_SES_RESET_SESSION;

  XcpLight_self.seed_index            = 0u;
  XcpLight_self.remaining_seed_bytes  = 0u;
  XcpLight_self.key_index             = 0u;
  XcpLight_self.unlock_resource       = 0u;

#ifdef XCPLIGHT_CFG_SEED_AND_KEY
  XcpLight_self.protection_status     = XCP_PRT_PROTECT_ALL;
#endif
}

////////////////////////////////////////////////////////////////////////////////
void XcpLight_updateTimestampCounter(void)
{
  XcpLight_self.timestamp_counter++;
}

////////////////////////////////////////////////////////////////////////////////
void XcpLight_processCommand(XcpLightMessage_t *msg)
{
  XcpLightMessage_t *reply_msg = &(XcpLight_self.cto_reply_msg);

#ifdef XCPLIGHT_CFG_DEBUG_CMD_MSG
  XcpLight_self.cto_cmd_msg = *msg; // copy to internal state (for ext. debugging)
#endif

  bool send_flag = false;

  XcpLight_self.current_command = (msg->payload[0] & 0xFFu);

  if (XcpLight_self.current_command == XCP_CMD_CONNECT)
  {
    // STD : standard commands :: connect begin
    send_flag = cmdConnect(msg, reply_msg);
    // STD : standard commands :: connect end
  }
  else
  {
    if (XcpLight_self.session_status & XCP_SES_CONNECTED)
    {
      // in connected state process all other commands
      switch (XcpLight_self.current_command)
      {
        // STD : standard commands -> begin
        case XCP_CMD_DISCONNECT:
          send_flag = cmdDisconnect(msg, reply_msg);
          break;

        case XCP_CMD_GET_STATUS:
          send_flag = cmdGetStatus(msg, reply_msg);
          break;

        case XCP_CMD_SYNCH:
          send_flag = cmdSynch(msg, reply_msg);
          break;

        case XCP_CMD_GET_COMM_MODE_INFO:
          send_flag = cmdGetCommModeInfo(msg, reply_msg);
          break;

#ifdef XCPLIGHT_CFG_SEED_AND_KEY
        case XCP_CMD_GET_SEED:
          send_flag = cmdGetSeed(msg, reply_msg);
          break;

        case XCP_CMD_UNLOCK:
          send_flag = cmdUnlock(msg, reply_msg);
          break;
#endif // XCPLIGHT_CFG_SEED_AND_KEY

#ifdef XCPLIGHT_CFG_USER_CMD
        case XCP_CMD_USER_CMD:
          send_flag = cmdUserCmd(msg, reply_msg);
          break;
#endif // XCPLIGHT_CFG_USER_CMD

        case XCP_CMD_SET_MTA:
          send_flag = cmdSetMta(msg, reply_msg);
          break;

        case XCP_CMD_UPLOAD:
          send_flag = cmdUpload(msg, reply_msg);
          break;

        case XCP_CMD_SHORT_UPLOAD:
          send_flag = cmdShortUpload(msg, reply_msg);
          break;

        case XCP_CMD_BUILD_CHECKSUM:
          send_flag = buildErrorMessage(reply_msg, XCP_ERR_CMD_UNKNOWN);
          break;
        // STD : standard commands -> end

#ifdef XCPLIGHT_CFG_ENABLE_CALPAG
        // CAL : standard commands -> begin
        case XCP_CMD_DOWNLOAD:
          send_flag = cmdDownload(msg, reply_msg);
          break;
        // CAL : standard commands -> end

        // PAG: page switching commands -> begin
        case XCP_CMD_SET_CAL_PAGE:
        case XCP_CMD_GET_CAL_PAGE:
        // PAG: page switching commands -> end
#endif // XCPLIGHT_CFG_ENABLE_CALPAG

#ifdef XCPLIGHT_CFG_ENABLE_DAQ
        // DAQ: data acquisition commands -> begin
        // @note: not supported yet
        // DAQ: data acquisition commands -> end
#endif // XCPLIGHT_CFG_ENABLE_DAQ

#ifdef XCPLIGHT_CFG_ENABLE_PGM
        // PGM: programming commands -> begin
        // @note: not supported yet
        case XCP_CMD_PROGRAM_START:
        case XCP_CMD_PROGRAM_CLEAR:
        case XCP_CMD_PROGRAM:
        case XCP_CMD_PROGRAM_RESET:

        case XCP_CMD_PROGRAM_MAX:
        case XCP_CMD_PROGRAM_VERIFY:
          send_flag = buildErrorMessage(reply_msg, XCP_ERR_CMD_UNKNOWN);
          break;
        // PGM: programming commands -> end
#endif // XCPLIGHT_CFG_ENABLE_PGM

        // Whenever a command is unknown -> build and send an error message
        default:
          send_flag = buildErrorMessage(reply_msg, XCP_ERR_CMD_UNKNOWN);
          break;
      }
    }
    else
    {
      // no XCP communication in disconnected state! ignore command.
    }
  }

  // send reply message
  if (send_flag == true)
  {
    XcpLightCfg_sendMessage(reply_msg);
  }
}
