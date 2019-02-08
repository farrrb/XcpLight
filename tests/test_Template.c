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

XcpLightMessage_t cmdMsg = {{0}};

// santa's little helper functions

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
