#include <stdio.h>
#include <stdint.h>

#include "unity.h"

#include "XcpLight.h"
#include "XcpLight_internals.h"

#include "TestHelper.h"

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

void test_CmdConnect(void)
{
  cmdMsg.length     = 2u;
  cmdMsg.payload[0] = XCP_CMD_CONNECT; /* CONNECT */
  cmdMsg.payload[1] = 0x00u; /* mode=0 */

  XcpLight_Init();
  XcpLight_CommandProcessor(&cmdMsg);

  TEST_ASSERT_EQUAL_UINT8(8u,    replyMsg.length);
  TEST_ASSERT_EQUAL_UINT8(0xFFu, replyMsg.payload[0]); /* Ok:CONNECT */
}

void test_CmdDisconnect(void)
{
  SET_SESSION_CONNECTED();
  cmdMsg.length     = 1u;
  cmdMsg.payload[0] = XCP_CMD_DISCONNECT; /* DISCONNECT */

  XcpLight_CommandProcessor(&cmdMsg);

  TEST_ASSERT_EQUAL_UINT8(   1u, replyMsg.length);
  TEST_ASSERT_EQUAL_UINT8(0xFFu, replyMsg.payload[0]); /* Ok:DISCONNECT */
}

void test_CmdSynch(void)
{
  SET_SESSION_CONNECTED();
  cmdMsg.length = 1u;
  cmdMsg.payload[0] = XCP_CMD_SYNCH;

  XcpLight_CommandProcessor(&cmdMsg);

  TEST_ASSERT_EQUAL_UINT8(   2u, replyMsg.length);
  TEST_ASSERT_EQUAL_UINT8(0xFE,  replyMsg.payload[0]); /* XCP_PID_ERR */
  TEST_ASSERT_EQUAL_UINT8(0x00u, replyMsg.payload[1]); /* ERR_CMD_SYNCH */
}

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_CmdConnect);
  RUN_TEST(test_CmdDisconnect);
  RUN_TEST(test_CmdSynch);
  return UNITY_END();
}
