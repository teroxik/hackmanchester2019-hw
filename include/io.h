#include <memory>
#include <SoftwareSerial.h>

using payload = std::string;

class transport
{
private:
    SoftwareSerial serial;

public:
    transport();
    void send(payload payload);
    std::vector<payload> receive();
};

struct message
{
    char code;
    uint8_t param;
};

std::vector<message> parse_payload(const std::vector<payload> payloads);