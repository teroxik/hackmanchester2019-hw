#include <unity.h>
#include "pins.h"
#include "sms.h"

void test_foo() {
  transport t;
  Serial.begin(9600);

  auto serial = software_serial_mock::instance().get(pins::sms_rx);
  serial->add_auto_response("AT\n", "OK\n");
  t.begin();
  t.send("123", "123");
  t.loop();

  std::cout << "Test end " << serial->get_tx() << std::endl;

  // std::for_each(std::begin(writes), std::end(writes),
  //               [](const auto &w) { std::cout << w << std::endl; });
}

extern "C" int main(int, char **) {
  UNITY_BEGIN();
  RUN_TEST(test_foo);
  UNITY_END();
  return 0;
}
