#include <stdio.h>
#include <stdint.h>

#include "unity.h"
#include "XcpLight.h"

void test_BasicCommandDummy(void)
{
  TEST_ASSERT_TRUE(1);
}

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_BasicCommandDummy);
  return UNITY_END();
}
