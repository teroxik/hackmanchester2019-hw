#include <unity.h>
#include "sms.h"

void test_foo()
{
}

extern "C" int main(int, char **)
{
    UNITY_BEGIN();
    RUN_TEST(test_foo);
    UNITY_END();
    return 0;
}
