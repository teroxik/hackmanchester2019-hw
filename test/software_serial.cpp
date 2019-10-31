#include <SoftwareSerial.h>

std::vector<std::string> SoftwareSerial::get_writes() const
{
    return writes;
}

SoftwareSerial *software_serial_mock::get(const int rx_pin) const
{
    auto found = std::find_if(std::begin(mock_instances), std::end(mock_instances), [&](const auto &x) {
        return std::get<1>(x) == rx_pin;
    });
    if (found != std::end(mock_instances))
        return std::get<3>(*found);
    throw nullptr;
}
