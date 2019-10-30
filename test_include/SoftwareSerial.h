#include <string>
#include <memory>

class SoftwareSerial
{
private:
    std::string last_write;

public:
    void print(const char *){};
    void println(const char *){};
    void begin(int baud, int rx_pin, int tx_pin);
    void write(char){};
    char read() { return -1; };
    bool available() { return false; }

    std::string get_last_write() const;
    std::string set_read(const std::string content);
};

class software_serial_mock
{
private:
    software_serial_mock(){};

public:
    static software_serial_mock &instance()
    {
        static software_serial_mock instance;
        return instance;
    };
    software_serial_mock(software_serial_mock const &) = delete;
    void operator=(software_serial_mock const &) = delete;

    void add_software_serial(std::weak_ptr<SoftwareSerial> instance, int baud, int rx_pin, int tx_pin)
    {
    }
};
