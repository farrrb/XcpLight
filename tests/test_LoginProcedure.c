////////////////////////////////////////////////////////////////////////////////
/// MIT License see LICENSE file
/// Copyright (c) 2016-2019 0xFAB - Fabian Zahn
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdint.h>

#include "unity.h"

#include "XcpLight.h"
#include "XcpLightInternals.h"

#include "TestHelper.h"

extern XcpLightMessage_t reply_msg;
extern XcpLightInternals_t XcpLight_self;

XcpLightMessage_t cmdMsg = {0, {0}};

// santa's little helper functions

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
    reply_msg.payload[i] = 0u;
  }
  reply_msg.length = 0u;
}

// test setUp / tearDown

void setUp(void)
{
  clearCmdMessage();
  clearReplyMessage();
  XcpLight_init();
}

void tearDown(void)
{
}

// the actual tests

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
  cmdMsg.payload[2] = XCPLIGHT_RESOURCE_CALPAG;

  XcpLight_processCommand(&cmdMsg);

  TEST_ASSERT_EQUAL_UINT8(8, reply_msg.length);
  TEST_ASSERT_EQUAL_UINT8(XCP_PID_RES, reply_msg.payload[0]);

  TEST_ASSERT_EQUAL_UINT8(10, reply_msg.payload[1]);
  TEST_ASSERT_EQUAL_UINT8(0 + XCPLIGHT_RESOURCE_CALPAG, reply_msg.payload[2]);
  TEST_ASSERT_EQUAL_UINT8(1 + XCPLIGHT_RESOURCE_CALPAG, reply_msg.payload[3]);
  TEST_ASSERT_EQUAL_UINT8(2 + XCPLIGHT_RESOURCE_CALPAG, reply_msg.payload[4]);
  TEST_ASSERT_EQUAL_UINT8(3 + XCPLIGHT_RESOURCE_CALPAG, reply_msg.payload[5]);
  TEST_ASSERT_EQUAL_UINT8(4 + XCPLIGHT_RESOURCE_CALPAG, reply_msg.payload[6]);
  TEST_ASSERT_EQUAL_UINT8(5 + XCPLIGHT_RESOURCE_CALPAG, reply_msg.payload[7]);

  cmdMsg.length = 3u;
  cmdMsg.payload[0] = XCP_CMD_GET_SEED;
  cmdMsg.payload[1] = 1;
  cmdMsg.payload[2] = 0xFF;

  XcpLight_processCommand(&cmdMsg);

  TEST_ASSERT_EQUAL_UINT8(6, reply_msg.length);
  TEST_ASSERT_EQUAL_UINT8(XCP_PID_RES, reply_msg.payload[0]);

  TEST_ASSERT_EQUAL_UINT8(4, reply_msg.payload[1]);
  TEST_ASSERT_EQUAL_UINT8(6 + XCPLIGHT_RESOURCE_CALPAG, reply_msg.payload[2]);
  TEST_ASSERT_EQUAL_UINT8(7 + XCPLIGHT_RESOURCE_CALPAG, reply_msg.payload[3]);
  TEST_ASSERT_EQUAL_UINT8(8 + XCPLIGHT_RESOURCE_CALPAG, reply_msg.payload[4]);
  TEST_ASSERT_EQUAL_UINT8(9 + XCPLIGHT_RESOURCE_CALPAG, reply_msg.payload[5]);

  XcpLight_processCommand(&cmdMsg);

  cmdMsg.length = 3u;
  cmdMsg.payload[0] = XCP_CMD_GET_SEED;
  cmdMsg.payload[1] = 1;
  cmdMsg.payload[2] = 0xFF;

  XcpLight_processCommand(&cmdMsg);
  TEST_ASSERT_EQUAL_UINT8(2, reply_msg.length);
  TEST_ASSERT_EQUAL_UINT8(XCP_PID_ERR,      reply_msg.payload[0]);
  TEST_ASSERT_EQUAL_UINT8(XCP_ERR_SEQUENCE, reply_msg.payload[1]);
}

void test_Unlock(void)
{

}

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_LoginProcedureDummy);
  RUN_TEST(test_GetSeed);
  RUN_TEST(test_Unlock);
  return UNITY_END();
}
