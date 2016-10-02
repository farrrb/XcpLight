#include <stdio.h>
#include <stdint.h>

#include "unity.h"

#include "XcpLight.h"
#include "XcpLight_internals.h"
#include "XcpLightMem.h"

#include "TestHelper.h"

XcpLightMemory_t mem = {0};
uint8_t Buffer[4] = {1u, 2u, 3u, 4u};

/* test setUp / tearDown */
void setUp(void)
{
}

void tearDown(void)
{
}

/* the actual tests */
void test_XcpLightMem_Init(void)
{
  TEST_ASSERT(XcpLightMem_Init(&mem, &Buffer[0], 4u) == 0);
  TEST_ASSERT_NOT_NULL(mem.pBuffer);
  TEST_ASSERT_NOT_NULL(mem.pAlloc);
  TEST_ASSERT(mem.size == 4u);
  TEST_ASSERT(mem.used == 0u);
  TEST_ASSERT_EQUAL_UINT8(1u, mem.pBuffer[0]);
  TEST_ASSERT_EQUAL_UINT8(2u, mem.pBuffer[1]);
  TEST_ASSERT_EQUAL_UINT8(3u, mem.pBuffer[2]);
  TEST_ASSERT_EQUAL_UINT8(4u, mem.pBuffer[3]);
}

void test_XcpLightMem_Clear(void)
{
  TEST_ASSERT(XcpLightMem_Init(&mem, &Buffer[0], 4u) == 0);
  /* lets fake some mem usage */
  mem.used = 23;
  TEST_ASSERT(XcpLightMem_Clear(&mem) == 0);
  TEST_ASSERT(mem.size == 4u);
  TEST_ASSERT(mem.used == 0u);
  TEST_ASSERT_EQUAL_UINT8(0u, mem.pBuffer[0]);
  TEST_ASSERT_EQUAL_UINT8(0u, mem.pBuffer[1]);
  TEST_ASSERT_EQUAL_UINT8(0u, mem.pBuffer[2]);
  TEST_ASSERT_EQUAL_UINT8(0u, mem.pBuffer[3]);
}

void test_XcpLightMem_Alloc(void)
{
  uint8_t * ptr;
  TEST_ASSERT(XcpLightMem_Init(&mem, &Buffer[0], 4u) == 0);
  TEST_ASSERT(XcpLightMem_Clear(&mem) == 0);

  ptr = XcpLightMem_Alloc(&mem, 2);
  TEST_ASSERT_NOT_NULL(ptr);
  TEST_ASSERT(mem.pBuffer == &Buffer[0]);
  TEST_ASSERT(ptr == mem.pBuffer);
  TEST_ASSERT(mem.used == 2u);

  ptr = XcpLightMem_Alloc(&mem, 1);
  TEST_ASSERT_NOT_NULL(ptr);
  TEST_ASSERT(mem.pBuffer == &Buffer[0]);
  TEST_ASSERT(ptr == mem.pBuffer + 2);
  TEST_ASSERT(mem.used == 3u);
}

void test_XcpLightMem_AllocEverything(void)
{
  uint8_t * ptr;
  TEST_ASSERT(XcpLightMem_Init(&mem, &Buffer[0], 4u) == 0);
  TEST_ASSERT(XcpLightMem_Clear(&mem) == 0);

  ptr = XcpLightMem_Alloc(&mem, 4);
  TEST_ASSERT_NOT_NULL(ptr);
  TEST_ASSERT(ptr == mem.pBuffer);
  TEST_ASSERT(mem.used == 4u);

  TEST_ASSERT(mem.pBuffer == &Buffer[0]);
}

void test_XcpLightMem_AllocTooMuch(void)
{
  uint8_t * ptr;
  TEST_ASSERT(XcpLightMem_Init(&mem, &Buffer[0], 4u) == 0);
  TEST_ASSERT(XcpLightMem_Clear(&mem) == 0);

  ptr = XcpLightMem_Alloc(&mem, 5);
  TEST_ASSERT_NULL(ptr);
  TEST_ASSERT(mem.used == 0u);
  TEST_ASSERT(mem.pAlloc == mem.pBuffer);

  TEST_ASSERT(mem.pBuffer == &Buffer[0]);
}

int main(void)
{
  UNITY_BEGIN();

  RUN_TEST(test_XcpLightMem_Init);
  RUN_TEST(test_XcpLightMem_Clear);
  RUN_TEST(test_XcpLightMem_Alloc);
  RUN_TEST(test_XcpLightMem_AllocEverything);
  RUN_TEST(test_XcpLightMem_AllocTooMuch);

  return UNITY_END();
}
