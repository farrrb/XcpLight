#ifndef __TEST_HELPER_H
#define __TEST_HELPER_H

#ifdef __cplusplus
extern "C"
{
#endif

#define SET_SESSION_CONNECTED() (_XcpLightData.sessionStatus |= XCP_SES_CONNECTED)

#ifdef __cplusplus
}
#endif

#endif // __TEST_HELPER_H
