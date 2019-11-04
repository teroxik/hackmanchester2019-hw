#include "pod.h"
#include <freertos/FreeRTOS.h>
#include <algorithm>
#include "pins.h"

pod_container::pod_container() {
  pods.emplace_back(make_led_pod(0, pins::sunday_red, pins::sunday_green,
                                 pins::sunday_detect));  // 0, -1, D0, D3
  pods.emplace_back(make_led_pod(5, pins::friday_red, pins::friday_green,
                                 pins::friday_detect));  // 5, D4, -1, D5
}

void pod_container::begin() {
  for (auto &pod : pods) {
    // pinMode(pod.detect_pin, INPUT_PULLUP);
    // if (pod.red_pin != -1) pinMode(pod.red_pin, OUTPUT);
    // if (pod.green_pin != -1) pinMode(pod.green_pin, OUTPUT);
  }
}

std::vector<led_pod> pod_container::loop() {
  std::vector<led_pod> updates;
  std::for_each(std::begin(pods), std::end(pods), [&updates](auto &&pod) {
    // if (digitalRead(pod.detect_pin) == LOW && pod.taken_at == 0) {
    //   pod.taken_at = millis();
    //   updates.emplace_back(pod);
    // }
  });
  // for (auto &&pod : pods) {
  //   if (digitalRead(pod.detect_pin) == LOW && pod.taken_at == 0) {
  //     // drug pulled out
  //     pod.taken_at = millis();
  //     updates.push_back(pod);
  //   }
  // }

  std::for_each(std::begin(pods), std::end(pods), [](auto &&pod) {
    // if (pod.take_now() && pod.green_pin != -1) {
    //   if (pod.taken_correctly())
    //     digitalWrite(pod.green_pin, LOW);
    //   else
    //     digitalWrite(pod.green_pin, HIGH);
    // }

    // if (pod.taken_incorrectly() && pod.red_pin != -1)
    //   digitalWrite(pod.red_pin, HIGH);
  });
  // for (auto &&pod : pods) {
  //   if (pod.take_now() && pod.green_pin != -1) {
  //     if (pod.taken_correctly())
  //       digitalWrite(pod.green_pin, LOW);
  //     else
  //       digitalWrite(pod.green_pin, HIGH);
  //   }

  //   if (pod.taken_incorrectly() && pod.red_pin != -1)
  //     digitalWrite(pod.red_pin, HIGH);
  // }

  return updates;
}

void pod_container::take_now(const unsigned int day) {
  std::for_each(std::begin(pods), std::end(pods), [&](auto &&pod) {
    if (pod.day == day) pod.prompted_at = system_get_time();
  });
  // for (auto &&pod : pods) {
  //   if (pod.day == day) pod.prompted_at = millis();
  // }
}

led_pod make_led_pod(int day, int red_pin, int green_pin, int detect_pin) {
  led_pod result;
  result.day = day;
  result.red_pin = red_pin;
  result.green_pin = green_pin;
  result.detect_pin = detect_pin;
  result.prompted_at = 0;
  result.taken_at = 0;

  return result;
}
