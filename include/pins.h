#include <Arduino.h>

namespace pins
{
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
}; // namespace pins