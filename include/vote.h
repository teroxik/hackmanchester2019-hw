#include <FastLED.h>

struct vote_result
{
    int value;
    bool is_valid() const { return value != -1; }
};

class vote
{
private:
    static constexpr int num_leds = 0;
    CRGB leds[num_leds];
    bool show_vote_now;

public:
    void begin();
    void vote_now();
    vote_result loop();
};
