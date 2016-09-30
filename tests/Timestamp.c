#include <stdio.h>
#include <stdint.h>

#include "unity.h"

#include "TestHelper.h"

#include "XcpLight.h"
#include "XcpLight_internals.h"

extern XcpLightMessage_t replyMsg;
extern XcpLightInternals_t _XcpLightData;

XcpLightMessage_t cmdMsg = {0};

/* santa's little helper functions */

void clearCmdMessage(void)
{
  int i;
  for(i=0; i<XCPLIGHT_CFG_XTO_LENGTH; i++)
  {
    cmdMsg.payload[i] = 0u;
  }
  cmdMsg.length = 0u;
}

void clearReplyMessage(void)
{
  int i;
  for(i=0; i<XCPLIGHT_CFG_XTO_LENGTH; i++)
  {
    replyMsg.payload[i] = 0u;
  }
  replyMsg.length = 0u;
}

/* test setUp / tearDown */

void setUp(void)
{
  clearCmdMessage();
  clearReplyMessage();
}

void tearDown(void)
{
}

/* the actual tests */

void test_TimestampAfterInit(void)
{
  XcpLight_Init();
  TEST_ASSERT_EQUAL_UINT32(0u, _XcpLightData.timestampCounter);
}

void test_TimestampIncrement(void)
{
  XcpLight_Init();
  XcpLight_UpdateTimestampCounter();
  TEST_ASSERT_EQUAL_UINT32(1u, _XcpLightData.timestampCounter);
  XcpLight_UpdateTimestampCounter();
  TEST_ASSERT_EQUAL_UINT32(2u, _XcpLightData.timestampCounter);
  XcpLight_UpdateTimestampCounter();
  XcpLight_UpdateTimestampCounter();
  XcpLight_UpdateTimestampCounter();
  TEST_ASSERT_EQUAL_UINT32(5u, _XcpLightData.timestampCounter);
}

void test_TimestampWrapAround(void)
{
  XcpLight_Init();
  _XcpLightData.timestampCounter = 0xFFFFFFFFu;
  TEST_ASSERT_EQUAL_UINT32(0xFFFFFFFFu, _XcpLightData.timestampCounter);
  XcpLight_UpdateTimestampCounter();
  TEST_ASSERT_EQUAL_UINT32(0u, _XcpLightData.timestampCounter);
}

void test_CmdGetDaqClock(void)
{
  XcpLight_Init();

  SET_SESSION_CONNECTED(); /* cheat here to get an active session! */
  _XcpLightData.timestampCounter = 0xDEADBEEFu;

  cmdMsg.length     = 1u;
  cmdMsg.payload[0] = XCP_CMD_GET_DAQ_CLOCK; /* GET_DAQ_CLOCK */

  XcpLight_CommandProcessor(&cmdMsg);

  TEST_ASSERT_EQUAL_UINT8(8u,          replyMsg.length);     /* 8 bytes of payload */
  TEST_ASSERT_EQUAL_UINT8(XCP_PID_RES, replyMsg.payload[0]); /* Ok:GET_DAQ_CLOCK */

  TEST_ASSERT_EQUAL_UINT8(0x00u, replyMsg.payload[1]); /* reserved */
  TEST_ASSERT_EQUAL_UINT8(0x00u, replyMsg.payload[2]); /* reserved */
  TEST_ASSERT_EQUAL_UINT8(0x00u, replyMsg.payload[3]); /* reserved */
  TEST_ASSERT_EQUAL_UINT8(0xEFu, replyMsg.payload[4]); /* timestamp byte 0 */
  TEST_ASSERT_EQUAL_UINT8(0xBEu, replyMsg.payload[5]); /* timestamp byte 1 */
  TEST_ASSERT_EQUAL_UINT8(0xADu, replyMsg.payload[6]); /* timestamp byte 2 */
  TEST_ASSERT_EQUAL_UINT8(0xDEu, replyMsg.payload[7]); /* timestamp byte 3 */
}

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_TimestampAfterInit);
  RUN_TEST(test_TimestampIncrement);
  RUN_TEST(test_TimestampWrapAround);
  RUN_TEST(test_CmdGetDaqClock);
  return UNITY_END();
}
