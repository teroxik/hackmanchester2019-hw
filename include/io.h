#include <memory>
#include <SoftwareSerial.h>

using payload = std::string;
using phone_number = std::string;
struct message
{
    char code;
    uint8_t param;

    bool is_valid() const
    {
        return code == 'T' || code == 'V';
    }
};

class transport
{
private:
    payload lp;
    message lm;
    SoftwareSerial serial;
    void print(const payload payload);
    void println(const payload payload);

public:
    void begin();
    void loop();
    void send(const phone_number phone_number, const payload payload);
    payload last_payload() const;
    message last_message() const;
};
