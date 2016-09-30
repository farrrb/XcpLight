#include <stdio.h>
#include <stdint.h>

#include "unity.h"

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
  cmdMsg.length     = 1u;
  cmdMsg.payload[0] = XCP_CMD_DISCONNECT; /* DISCONNECT */

  /* let's see if we are connected... because we can only disconnect if the session is active */
  TEST_ASSERT_BITS(XCP_SES_CONNECTED, XCP_SES_CONNECTED, _XcpLightData.sessionStatus);

  XcpLight_CommandProcessor(&cmdMsg);

  TEST_ASSERT_EQUAL_UINT8(   1u, replyMsg.length);
  TEST_ASSERT_EQUAL_UINT8(0xFFu, replyMsg.payload[0]); /* Ok:DISCONNECT */
}

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_CmdConnect);
  RUN_TEST(test_CmdDisconnect);
  return UNITY_END();
}
