#include <vector>

struct pod
{
    unsigned int day;
    unsigned long taken_at;
    unsigned long prompted_at;

    bool take_now() const
    {
        return prompted_at > 0L;
    }

    bool taken_incorrectly() const
    {
        return taken_at < prompted_at;
    }
};

struct led_pod : pod
{
    int red_pin;
    int green_pin;
    int detect_pin;
};

led_pod make_led_pod(int day, int red_pin, int green_pin, int detect_pin);

class pod_container
{
private:
    std::vector<led_pod> pods;

public:
    pod_container();
    void begin();
    void take_now(const unsigned int day);
    std::vector<led_pod> loop();
};
