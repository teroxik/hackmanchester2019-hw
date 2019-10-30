#include <SoftwareSerial.h>

void SoftwareSerial::begin(int baud, int rx_pin, int tx_pin)
{
    auto sp = std::shared_ptr<SoftwareSerial>(this);
    software_serial_mock::instance().add_software_serial(sp, baud, rx_pin, tx_pin);
}

std::string SoftwareSerial::get_last_write() const
{
    return last_write;
}
