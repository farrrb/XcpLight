////////////////////////////////////////////////////////////////////////////////
/// MIT License see LICENSE file
/// Copyright (c) 2016-2019 0xFAB - Fabian Zahn
////////////////////////////////////////////////////////////////////////////////

#ifndef TEST_HELPER_H__
#define TEST_HELPER_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define SET_SESSION_CONNECTED()     (XcpLight_self.session_status   |= XCP_SES_CONNECTED)
#define SET_UNLOCK_ALL_RESOURCES()  (XcpLight_self.protection_status = XCP_PRT_PROTECT_NONE)

#ifdef __cplusplus
}
#endif

#endif // TEST_HELPER_H__
