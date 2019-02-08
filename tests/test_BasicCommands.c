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

void test_CmdConnect(void)
{
  cmdMsg.length     = 2u;
  cmdMsg.payload[0] = XCP_CMD_CONNECT; // CONNECT
  cmdMsg.payload[1] = 0x00u; // mode=0

  XcpLight_processCommand(&cmdMsg);

  TEST_ASSERT_EQUAL_UINT8(8u,    reply_msg.length);
  TEST_ASSERT_EQUAL_UINT8(0xFFu, reply_msg.payload[0]); // Ok:CONNECT
}

void test_CmdDisconnect(void)
{
  SET_SESSION_CONNECTED();
  SET_UNLOCK_ALL_RESOURCES();
  cmdMsg.length     = 1u;
  cmdMsg.payload[0] = XCP_CMD_DISCONNECT; // DISCONNECT

  XcpLight_processCommand(&cmdMsg);

  TEST_ASSERT_EQUAL_UINT8(   1u, reply_msg.length);
  TEST_ASSERT_EQUAL_UINT8(0xFFu, reply_msg.payload[0]); // Ok:DISCONNECT
}

void test_CmdSynch(void)
{
  SET_SESSION_CONNECTED();
  SET_UNLOCK_ALL_RESOURCES();
  cmdMsg.length = 1u;
  cmdMsg.payload[0] = XCP_CMD_SYNCH;

  XcpLight_processCommand(&cmdMsg);

  TEST_ASSERT_EQUAL_UINT8(   2u, reply_msg.length);
  TEST_ASSERT_EQUAL_UINT8(0xFEu,  reply_msg.payload[0]); // XCP_PID_ERR
  TEST_ASSERT_EQUAL_UINT8(0x00u, reply_msg.payload[1]);  // ERR_CMD_SYNCH
}

void test_CmdGetCommModeInfo(void)
{
  SET_SESSION_CONNECTED();
  SET_UNLOCK_ALL_RESOURCES();
  cmdMsg.length = 1u;
  cmdMsg.payload[0] = XCP_CMD_GET_COMM_MODE_INFO;

  XcpLight_processCommand(&cmdMsg);

  TEST_ASSERT_EQUAL_UINT8(  8u, reply_msg.length);
  TEST_ASSERT_EQUAL_UINT8(0xFF, reply_msg.payload[0]); // Ok:GET_COMM_MODE_INFO

  TEST_ASSERT(reply_msg.payload[7]); // driver version is not zero
}

void test_CmdGetStatus(void)
{
  SET_SESSION_CONNECTED();
  SET_UNLOCK_ALL_RESOURCES();
  cmdMsg.length = 1u;
  cmdMsg.payload[0] = XCP_CMD_GET_STATUS;

  XcpLight_processCommand(&cmdMsg);

  TEST_ASSERT_EQUAL_UINT8(  6u, reply_msg.length);
  TEST_ASSERT_EQUAL_UINT8(0xFF, reply_msg.payload[0]); // Ok:GET_STATUS
  TEST_ASSERT_BITS(XCP_SES_CONNECTED, 0xFFu, reply_msg.payload[1]); // sessions is at least connected
}

void test_CmdGetDaqProcessorInfo(void)
{
  SET_SESSION_CONNECTED();
  SET_UNLOCK_ALL_RESOURCES();
  cmdMsg.length = 1u;
  cmdMsg.payload[0] = XCP_CMD_GET_DAQ_PROCESSOR_INFO;

  XcpLight_processCommand(&cmdMsg);

  TEST_ASSERT_EQUAL_UINT8(  8u, reply_msg.length);
  TEST_ASSERT_EQUAL_UINT8(0xFF, reply_msg.payload[0]); // Ok:GET_DAQ_PROCESSOR_INFO
}

void test_CmdGetDaqResolutionInfo(void)
{
  SET_SESSION_CONNECTED();
  SET_UNLOCK_ALL_RESOURCES();
  cmdMsg.length = 1u;
  cmdMsg.payload[0] = XCP_CMD_GET_DAQ_RESOLUTION_INFO;

  XcpLight_processCommand(&cmdMsg);

  TEST_ASSERT_EQUAL_UINT8(  8u, reply_msg.length);
  TEST_ASSERT_EQUAL_UINT8(0xFF, reply_msg.payload[0]); // Ok:GET_DAQ_DAQ_RESOLUTION_INFO
}

void test_CmdUserCmd(void)
{
  SET_SESSION_CONNECTED();
  SET_UNLOCK_ALL_RESOURCES();
  cmdMsg.length = 1u;
  cmdMsg.payload[0] = XCP_CMD_USER_CMD;

  XcpLight_processCommand(&cmdMsg);

  TEST_ASSERT_EQUAL_UINT8(  1u, reply_msg.length);
  TEST_ASSERT_EQUAL_UINT8(0x1A, reply_msg.payload[0]);
}

int main(void)
{
  UNITY_BEGIN();

  RUN_TEST(test_CmdConnect);
  RUN_TEST(test_CmdDisconnect);
  RUN_TEST(test_CmdSynch);
  RUN_TEST(test_CmdGetCommModeInfo);
  RUN_TEST(test_CmdGetStatus);
  RUN_TEST(test_CmdUserCmd);
//  RUN_TEST(test_CmdGetDaqProcessorInfo);
//  RUN_TEST(test_CmdGetDaqResolutionInfo);

  return UNITY_END();
}
