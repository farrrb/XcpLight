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

// test variables for mem read & write
uint32_t uploadVariable       = 0xDEADBEEFu;
uint32_t shortUploadVariable  = 0xDEADBEEFu;
uint32_t downloadVariable     = 0xDEADBEEFu;

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

void test_Upload(void)
{
  uint32_t addr = (uint32_t)(&uploadVariable);
  TEST_ASSERT_EQUAL_UINT32(0xDEADBEEFu, uploadVariable);

  SET_SESSION_CONNECTED();
  SET_UNLOCK_ALL_RESOURCES();

  cmdMsg.length = 8u;
  cmdMsg.payload[0] = XCP_CMD_SET_MTA;
  cmdMsg.payload[1] = 0u;
  cmdMsg.payload[2] = 0u;
  cmdMsg.payload[3] = 0u;
  cmdMsg.payload[4] = ((addr >>  0) & 0xFFu);
  cmdMsg.payload[5] = ((addr >>  8) & 0xFFu);
  cmdMsg.payload[6] = ((addr >> 16) & 0xFFu);
  cmdMsg.payload[7] = ((addr >> 24) & 0xFFu);

  XcpLight_processCommand(&cmdMsg);

  TEST_ASSERT(addr == (uint32_t)XcpLight_self.mta);

  clearCmdMessage();
  clearReplyMessage();

  cmdMsg.length = 2u;
  cmdMsg.payload[0] = XCP_CMD_UPLOAD;
  cmdMsg.payload[1] = 4u;

  XcpLight_processCommand(&cmdMsg);

  TEST_ASSERT_EQUAL_UINT8(   5u, reply_msg.length);
  TEST_ASSERT_EQUAL_UINT8(0xFFu, reply_msg.payload[0]); // Ok:UPLOAD
  // 0xDEADBEEFu
  TEST_ASSERT_EQUAL_UINT8(0xEFu, reply_msg.payload[1]);
  TEST_ASSERT_EQUAL_UINT8(0xBEu, reply_msg.payload[2]);
  TEST_ASSERT_EQUAL_UINT8(0xADu, reply_msg.payload[3]);
  TEST_ASSERT_EQUAL_UINT8(0xDEu, reply_msg.payload[4]);

  uint32_t address = (uint32_t)&uploadVariable;
  TEST_ASSERT_EQUAL_UINT32(address + 4, XcpLight_self.mta);
}

void test_ShortUpload(void)
{
  uint32_t addr = (uint32_t)(&shortUploadVariable);

  SET_SESSION_CONNECTED();
  SET_UNLOCK_ALL_RESOURCES();

  cmdMsg.length = 8u;
  cmdMsg.payload[0] = XCP_CMD_SHORT_UPLOAD;
  cmdMsg.payload[1] = 4u;
  cmdMsg.payload[2] = 0u;
  cmdMsg.payload[3] = 0u;
  cmdMsg.payload[4] = ((addr >>  0) & 0xFFu);
  cmdMsg.payload[5] = ((addr >>  8) & 0xFFu);
  cmdMsg.payload[6] = ((addr >> 16) & 0xFFu);
  cmdMsg.payload[7] = ((addr >> 24) & 0xFFu);

  XcpLight_processCommand(&cmdMsg);

  TEST_ASSERT_EQUAL_UINT8(   5u, reply_msg.length);
  TEST_ASSERT_EQUAL_UINT8(0xFFu, reply_msg.payload[0]); // Ok:SHORT_UPLOAD
  // 0xDEADBEEFu
  TEST_ASSERT_EQUAL_UINT8(0xEFu, reply_msg.payload[1]);
  TEST_ASSERT_EQUAL_UINT8(0xBEu, reply_msg.payload[2]);
  TEST_ASSERT_EQUAL_UINT8(0xADu, reply_msg.payload[3]);
  TEST_ASSERT_EQUAL_UINT8(0xDEu, reply_msg.payload[4]);

  uint32_t address = (uint32_t)&shortUploadVariable;
  TEST_ASSERT_EQUAL_UINT32(address + 4, XcpLight_self.mta);
}

void test_Download(void)
{
  uint32_t addr = (uint32_t)(&downloadVariable);
  TEST_ASSERT_EQUAL_UINT32(0xDEADBEEFu, downloadVariable);

  SET_SESSION_CONNECTED();
  SET_UNLOCK_ALL_RESOURCES();

  cmdMsg.length = 8u;
  cmdMsg.payload[0] = XCP_CMD_SET_MTA;
  cmdMsg.payload[1] = 0u;
  cmdMsg.payload[2] = 0u;
  cmdMsg.payload[3] = 0u;
  cmdMsg.payload[4] = ((addr >>  0) & 0xFFu);
  cmdMsg.payload[5] = ((addr >>  8) & 0xFFu);
  cmdMsg.payload[6] = ((addr >> 16) & 0xFFu);
  cmdMsg.payload[7] = ((addr >> 24) & 0xFFu);

  XcpLight_processCommand(&cmdMsg);

  TEST_ASSERT(addr == (uint32_t)XcpLight_self.mta);

  clearCmdMessage();
  clearReplyMessage();

  cmdMsg.length = 6u;
  cmdMsg.payload[0] = XCP_CMD_DOWNLOAD;
  cmdMsg.payload[1] = 4u;
  cmdMsg.payload[2] = 0x37u;
  cmdMsg.payload[3] = 0x13u;
  cmdMsg.payload[4] = 0xB1u;
  cmdMsg.payload[5] = 0xFAu;

  XcpLight_processCommand(&cmdMsg);

  TEST_ASSERT(downloadVariable == 0xFAB11337);
  uint32_t address = (uint32_t)&downloadVariable;
  TEST_ASSERT_EQUAL_UINT32(address + 4, XcpLight_self.mta);

}

int main(void)
{
  UNITY_BEGIN();

  RUN_TEST(test_Upload);
  RUN_TEST(test_ShortUpload);
  RUN_TEST(test_Download);

  return UNITY_END();
}
