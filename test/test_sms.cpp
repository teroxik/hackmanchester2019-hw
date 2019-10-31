#include <unity.h>
#include "sms.h"
#include "pins.h"

void test_foo()
{
    transport t;
    software_serial_mock::instance().get(pins::sms_rx);

    t.begin();
    t.send("123", "123");
    t.loop();
}

extern "C" int main(int, char **)
{
    UNITY_BEGIN();
    RUN_TEST(test_foo);
    UNITY_END();
    return 0;
}
