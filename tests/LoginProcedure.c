/* MIT License see LICENSE file             */
/* - Copyright (c) 2016-2017 0xFAB - Fabian Zahn */

#include <stdio.h>
#include <stdint.h>

#include "unity.h"

#include "XcpLight.h"
#include "XcpLight_internals.h"

#include "TestHelper.h"

extern XcpLightMessage_t replyMsg;
extern XcpLightInternals_t _XcpLightData;

XcpLightMessage_t cmdMsg = {0, {0}};

/* santa's little helper functions */

void clearCmdMessage(void)
{
  int i;
  for (i=0; i < XCPLIGHT_CFG_XTO_LENGTH; i++)
  {
    cmdMsg.payload[i] = 0u;
  }
  cmdMsg.length = 0u;
}

void clearReplyMessage(void)
{
  int i;
  for (i=0; i < XCPLIGHT_CFG_XTO_LENGTH; i++)
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

void test_LoginProcedureDummy(void)
{
  TEST_ASSERT_TRUE(1);
}

void test_GetSeed(void)
{
  SET_SESSION_CONNECTED();
  cmdMsg.length = 3u;
  cmdMsg.payload[0] = XCP_CMD_GET_SEED;
  cmdMsg.payload[1] = 0;
  cmdMsg.payload[2] = XCPLIGHT_RES_CALPAG;

  XcpLight_CommandProcessor(&cmdMsg);

  TEST_ASSERT_EQUAL_UINT8(8, replyMsg.length);
  TEST_ASSERT_EQUAL_UINT8(XCP_PID_RES, replyMsg.payload[0]);

  TEST_ASSERT_EQUAL_UINT8(10, replyMsg.payload[1]);
  TEST_ASSERT_EQUAL_UINT8(0 + XCPLIGHT_RES_CALPAG, replyMsg.payload[2]);
  TEST_ASSERT_EQUAL_UINT8(1 + XCPLIGHT_RES_CALPAG, replyMsg.payload[3]);
  TEST_ASSERT_EQUAL_UINT8(2 + XCPLIGHT_RES_CALPAG, replyMsg.payload[4]);
  TEST_ASSERT_EQUAL_UINT8(3 + XCPLIGHT_RES_CALPAG, replyMsg.payload[5]);
  TEST_ASSERT_EQUAL_UINT8(4 + XCPLIGHT_RES_CALPAG, replyMsg.payload[6]);
  TEST_ASSERT_EQUAL_UINT8(5 + XCPLIGHT_RES_CALPAG, replyMsg.payload[7]);

  cmdMsg.length = 3u;
  cmdMsg.payload[0] = XCP_CMD_GET_SEED;
  cmdMsg.payload[1] = 1;
  cmdMsg.payload[2] = 0xFF;

  XcpLight_CommandProcessor(&cmdMsg);

  TEST_ASSERT_EQUAL_UINT8(6, replyMsg.length);
  TEST_ASSERT_EQUAL_UINT8(XCP_PID_RES, replyMsg.payload[0]);

  TEST_ASSERT_EQUAL_UINT8(4, replyMsg.payload[1]);
  TEST_ASSERT_EQUAL_UINT8(6 + XCPLIGHT_RES_CALPAG, replyMsg.payload[2]);
  TEST_ASSERT_EQUAL_UINT8(7 + XCPLIGHT_RES_CALPAG, replyMsg.payload[3]);
  TEST_ASSERT_EQUAL_UINT8(8 + XCPLIGHT_RES_CALPAG, replyMsg.payload[4]);
  TEST_ASSERT_EQUAL_UINT8(9 + XCPLIGHT_RES_CALPAG, replyMsg.payload[5]);

  XcpLight_CommandProcessor(&cmdMsg);

  cmdMsg.length = 3u;
  cmdMsg.payload[0] = XCP_CMD_GET_SEED;
  cmdMsg.payload[1] = 1;
  cmdMsg.payload[2] = 0xFF;

  XcpLight_CommandProcessor(&cmdMsg);
  TEST_ASSERT_EQUAL_UINT8(2, replyMsg.length);
  TEST_ASSERT_EQUAL_UINT8(XCP_PID_ERR,      replyMsg.payload[0]);
  TEST_ASSERT_EQUAL_UINT8(XCP_ERR_SEQUENCE, replyMsg.payload[1]);

}

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_LoginProcedureDummy);
  RUN_TEST(test_GetSeed);
  return UNITY_END();
}
