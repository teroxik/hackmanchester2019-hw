#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include "io.h"

transport::transport()
{
    serial.begin(9600, 0, 1);
}

std::vector<payload> transport::receive()
{
    std::vector<payload> result;
    return result;
}

void transport::send(payload payload)
{
}

message parse(const payload payload)
{
    message result;
    if (payload.length() == 2)
        result.param = 0x30 - payload[1];
    result.code = payload[0];
    return result;
}

std::vector<message> parse_payload(const std::vector<payload> payloads)
{
    std::vector<message> result;
    std::transform(payloads.begin(), payloads.end(), result.begin(), parse);
    return result;
}