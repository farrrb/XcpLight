/* MIT License see LICENSE file             */
/* - Copyright (c) 2016 0xFAB - Fabian Zahn */

#include <stdio.h>
#include <stdint.h>

#include "unity.h"

#include "XcpLight.h"
#include "XcpLight_internals.h"

#include "TestHelper.h"

extern XcpLightMessage_t replyMsg;
extern XcpLightInternals_t _XcpLightData;

XcpLightMessage_t cmdMsg = {{0}};

/* santa's little helper functions */

void clearCmdMessage(void)
{
  int i;
  for (i=0; i< XCPLIGHT_CFG_XTO_LENGTH; i++)
  {
    cmdMsg.payload[i] = 0u;
  }
  cmdMsg.length = 0u;
}

void clearReplyMessage(void)
{
  int i;
  for (i=0; i< sXCPLIGHT_CFG_XTO_LENGTH; i++)
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
  XcpLight_Init();
}

void tearDown(void)
{
}

/* the actual tests */

void test_Template(void)
{
  TEST_ASSERT_TRUE(1);
}

int main(void)
{
  UNITY_BEGIN();

  RUN_TEST(test_Template);

  return UNITY_END();
}
