#include <unity.h>
#include "pins.h"
#include "sms.h"

using namespace fakeit;

void test_foo() {
  transport t;

  When(Method(ArduinoFake(), delay)).AlwaysReturn();
  t.begin();
  t.send("123", "123");
  t.loop();

  auto writes =
      (*software_serial_mock::instance().get(pins::sms_rx))->get_writes();
  std::for_each(std::begin(writes), std::end(writes),
                [](const auto &w) { std::cout << w << std::endl; });
}

extern "C" int main(int, char **) {
  UNITY_BEGIN();
  RUN_TEST(test_foo);
  UNITY_END();
  return 0;
}
