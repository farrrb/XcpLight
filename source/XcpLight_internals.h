#ifndef __XCPLIGHT_INTERNALS_H
#define __XCPLIGHT_INTERNALS_H

#ifdef __cplusplus
extern "C"
{
#endif

//------------------------------------------------------------------------------
// macro constants
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// struct and type definitions (struct, enum and typedef)
//------------------------------------------------------------------------------

/* internal data structure @todo move this to an "internals" file */
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
