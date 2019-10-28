#include <Arduino.h>

namespace pins
{
#ifdef NODEMCU
constexpr int sunday_green = D0;
constexpr int sunday_red = -1;
constexpr int sunday_detect = D3;

constexpr int friday_red = D4;
constexpr int friday_green = -1;
constexpr int friday_detect = D5;

constexpr int sms_rx = D1;
constexpr int sms_tx = D2;

constexpr int vote_0 = A0;
constexpr int vote_1 = D9;
// constexpr int vote_2 = D8; // A0??
constexpr int vote_now = D6;
#endif

#ifdef ESP32
constexpr int sunday_green = 0;
constexpr int sunday_red = 1;
constexpr int sunday_detect = 2;

constexpr int friday_red = 3;
constexpr int friday_green = 4;
constexpr int friday_detect = 5;

constexpr int sms_rx = 6;
constexpr int sms_tx = 7;

constexpr int vote_0 = 8;
constexpr int vote_1 = 9;
// constexpr int vote_2 = D8; // A0??
constexpr int vote_now = 10;
#endif
}; // namespace pins