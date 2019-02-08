////////////////////////////////////////////////////////////////////////////////
/// MIT License see LICENSE file
/// Copyright (c) 2016-2019 0xFAB - Fabian Zahn
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdint.h>

#include "unity.h"

#include "TestHelper.h"

#include "XcpLight.h"
#include "XcpLightInternals.h"

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
}

void tearDown(void)
{
}

// the actual tests

void test_TimestampAfterInit(void)
{
  XcpLight_init();
  TEST_ASSERT_EQUAL_UINT32(0u, XcpLight_self.timestamp_counter);
}

void test_TimestampIncrement(void)
{
  XcpLight_init();
  XcpLight_updateTimestampCounter();
  TEST_ASSERT_EQUAL_UINT32(1u, XcpLight_self.timestamp_counter);
  XcpLight_updateTimestampCounter();
  TEST_ASSERT_EQUAL_UINT32(2u, XcpLight_self.timestamp_counter);
  XcpLight_updateTimestampCounter();
  XcpLight_updateTimestampCounter();
  XcpLight_updateTimestampCounter();
  TEST_ASSERT_EQUAL_UINT32(5u, XcpLight_self.timestamp_counter);
}

void test_TimestampWrapAround(void)
{
  XcpLight_init();
  XcpLight_self.timestamp_counter = 0xFFFFFFFFu;
  TEST_ASSERT_EQUAL_UINT32(0xFFFFFFFFu, XcpLight_self.timestamp_counter);
  XcpLight_updateTimestampCounter();
  TEST_ASSERT_EQUAL_UINT32(0u, XcpLight_self.timestamp_counter);
}

void test_CmdGetDaqClock(void)
{
  XcpLight_init();

  SET_SESSION_CONNECTED();
  SET_UNLOCK_ALL_RESOURCES();

  XcpLight_self.timestamp_counter = 0xDEADBEEFu;

  cmdMsg.length     = 1u;
  cmdMsg.payload[0] = XCP_CMD_GET_DAQ_CLOCK; // GET_DAQ_CLOCK

  XcpLight_processCommand(&cmdMsg);

  TEST_ASSERT_EQUAL_UINT8(8u,          reply_msg.length);     // 8 bytes of payload
  TEST_ASSERT_EQUAL_UINT8(XCP_PID_RES, reply_msg.payload[0]); // Ok:GET_DAQ_CLOCK

  TEST_ASSERT_EQUAL_UINT8(0x00u, reply_msg.payload[1]); // reserved
  TEST_ASSERT_EQUAL_UINT8(0x00u, reply_msg.payload[2]); // reserved
  TEST_ASSERT_EQUAL_UINT8(0x00u, reply_msg.payload[3]); // reserved
  TEST_ASSERT_EQUAL_UINT8(0xEFu, reply_msg.payload[4]); // timestamp byte 0
  TEST_ASSERT_EQUAL_UINT8(0xBEu, reply_msg.payload[5]); // timestamp byte 1
  TEST_ASSERT_EQUAL_UINT8(0xADu, reply_msg.payload[6]); // timestamp byte 2
  TEST_ASSERT_EQUAL_UINT8(0xDEu, reply_msg.payload[7]); // timestamp byte 3
}

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_TimestampAfterInit);
  RUN_TEST(test_TimestampIncrement);
  RUN_TEST(test_TimestampWrapAround);
  //RUN_TEST(test_CmdGetDaqClock);
  return UNITY_END();
}
