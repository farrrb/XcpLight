/* MIT License see LICENSE file             */
/* - Copyright (c) 2016-2017 0xFAB - Fabian Zahn */

#ifndef _s_TEST_HELPER_H
#define __TEST_HELPER_H

#ifdef __cplusplus
extern "C"
{
#endif

#define SET_SESSION_CONNECTED()     (_XcpLightData.sessionStatus |= XCP_SES_CONNECTED)
#define SET_UNLOCK_ALL_RESOURCES()  (_XcpLightData.protectionStatus = 0x00u)

#ifdef __cplusplus
}
#endif

#endif // __TEST_HELPER_H
