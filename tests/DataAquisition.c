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
  XcpLight_Init();
}

void tearDown(void)
{
}

/* the actual tests */

void test_DaqSetup(void)
{
  SET_SESSION_CONNECTED();
  TEST_ASSERT_EQUAL_UINT8(0x00u, _XcpLightData.protectionStatus);

  /* FREE DAQ */
  cmdMsg.length = 1u;
  cmdMsg.payload[0] = XCP_CMD_FREE_DAQ;
  XcpLight_CommandProcessor(&cmdMsg);
  TEST_ASSERT_EQUAL_UINT8(   1u, replyMsg.length);
  TEST_ASSERT_EQUAL_UINT8(0xFFu, replyMsg.payload[0]); /* Ok:FREE_DAQ */

  /* ALLOC DAQ */
  cmdMsg.length = 4u;
  cmdMsg.payload[0] = XCP_CMD_ALLOC_DAQ;
  cmdMsg.payload[1] = 0u;
  /* alloc 256 daq lists */
  cmdMsg.payload[2] = 0xFFu;
  cmdMsg.payload[3] = 0x01u;
  XcpLight_CommandProcessor(&cmdMsg);
  TEST_ASSERT_EQUAL_UINT8(   2u, replyMsg.length);
  TEST_ASSERT_EQUAL_UINT8(0xFEu, replyMsg.payload[0]); /* Error:ALLOC_DAQ */
  TEST_ASSERT_EQUAL_UINT8(0x22u, replyMsg.payload[1]); /* XCP_ERR_OUT_OF_RANGE */

  cmdMsg.length = 4u;
  cmdMsg.payload[0] = XCP_CMD_ALLOC_DAQ;
  cmdMsg.payload[1] = 0u;
  cmdMsg.payload[2] = 3u; /* alloc 3 daq lists */
  cmdMsg.payload[3] = 0u;
  XcpLight_CommandProcessor(&cmdMsg);
  TEST_ASSERT_EQUAL_UINT8(   1u, replyMsg.length);
  TEST_ASSERT_EQUAL_UINT8(0xFFu, replyMsg.payload[0]); /* Ok:ALLOC_DAQ */

  /* ALLOC ODT */
  cmdMsg.length = 5u;
  cmdMsg.payload[0] = XCP_CMD_ALLOC_ODT;
  cmdMsg.payload[1] = 0u;
  cmdMsg.payload[2] = 0u; /* daq 0 */
  cmdMsg.payload[3] = 0u; /* daq 0 */
  cmdMsg.payload[4] = 1u; /* cnt 1 */
  XcpLight_CommandProcessor(&cmdMsg);
  TEST_ASSERT_EQUAL_UINT8(   1u, replyMsg.length);
  TEST_ASSERT_EQUAL_UINT8(0xFFu, replyMsg.payload[0]); /* Ok:ALLOC_ODT */

  cmdMsg.length = 5u;
  cmdMsg.payload[0] = XCP_CMD_ALLOC_ODT;
  cmdMsg.payload[1] = 0u;
  cmdMsg.payload[2] = 1u; /* daq 1 */
  cmdMsg.payload[3] = 0u; /* daq 0 */
  cmdMsg.payload[4] = 1u; /* cnt 1 */
  XcpLight_CommandProcessor(&cmdMsg);
  TEST_ASSERT_EQUAL_UINT8(   1u, replyMsg.length);
  TEST_ASSERT_EQUAL_UINT8(0xFFu, replyMsg.payload[0]); /* Ok:ALLOC_ODT */

  cmdMsg.length = 5u;
  cmdMsg.payload[0] = XCP_CMD_ALLOC_ODT;
  cmdMsg.payload[1] = 0u;
  cmdMsg.payload[2] = 2u; /* daq 2 */
  cmdMsg.payload[3] = 0u; /* daq 0 */
  cmdMsg.payload[4] = 2u; /* cnt 2 */
  XcpLight_CommandProcessor(&cmdMsg);
  TEST_ASSERT_EQUAL_UINT8(   1u, replyMsg.length);
  TEST_ASSERT_EQUAL_UINT8(0xFFu, replyMsg.payload[0]); /* Ok:ALLOC_ODT */

  cmdMsg.length = 5u;
  cmdMsg.payload[0] = XCP_CMD_ALLOC_ODT;
  cmdMsg.payload[1] = 0u;
  cmdMsg.payload[2] = 2u; /* daq 2 */
  cmdMsg.payload[3] = 0u; /* daq 0 */
  cmdMsg.payload[4] = 2u; /* cnt 2 */
  XcpLight_CommandProcessor(&cmdMsg);
  TEST_ASSERT_EQUAL_UINT8(   2u, replyMsg.length);
  TEST_ASSERT_EQUAL_UINT8(0xFEu, replyMsg.payload[0]); /* Er:ALLOC_ODT */
  TEST_ASSERT_EQUAL_UINT8(0x29u, replyMsg.payload[1]); /* XCP_ERR_SEQUENCE */

  cmdMsg.length = 5u;
  cmdMsg.payload[0] = XCP_CMD_ALLOC_ODT;
  cmdMsg.payload[1] = 0u;
  cmdMsg.payload[2] = 3u; /* daq 2 */
  cmdMsg.payload[3] = 0u; /* daq 0 */
  cmdMsg.payload[4] = 3u; /* cnt 2 */
  XcpLight_CommandProcessor(&cmdMsg);
  TEST_ASSERT_EQUAL_UINT8(   2u, replyMsg.length);
  TEST_ASSERT_EQUAL_UINT8(0xFEu, replyMsg.payload[0]); /* Error:ALLOC_ODT */
  TEST_ASSERT_EQUAL_UINT8(0x22u, replyMsg.payload[1]); /* XCP_ERR_SEQUENCE */

}

int main(void)
{
  UNITY_BEGIN();

  RUN_TEST(test_DaqSetup);


  return UNITY_END();
}
