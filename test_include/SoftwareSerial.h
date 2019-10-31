#include <string>
#include <memory>
#include <vector>
#include <map>

class SoftwareSerial;

class software_serial_mock
{
private:
    std::vector<std::tuple<int, int, int, std::weak_ptr<SoftwareSerial>>> mock_instances;
    software_serial_mock(){};

public:
    static software_serial_mock &instance()
    {
        static software_serial_mock instance;
        return instance;
    };
    software_serial_mock(software_serial_mock const &) = delete;
    void operator=(software_serial_mock const &) = delete;

    void add_software_serial(const std::weak_ptr<SoftwareSerial> instance, const int baud, const int rx_pin, const int tx_pin)
    {
        mock_instances.push_back(std::make_tuple(baud, rx_pin, tx_pin, instance));
    }
    std::shared_ptr<SoftwareSerial> get(const int rx_pin) const;
};

class SoftwareSerial
{
private:
    std::string last_write;

public:
    void print(const char *){};
    void println(const char *){};
    void begin(int baud, int rx_pin, int tx_pin)
    {
        auto sp = std::shared_ptr<SoftwareSerial>(this);
        software_serial_mock::instance().add_software_serial(sp, baud, rx_pin, tx_pin);
    }
    void write(char){};
    char read() { return -1; };
    bool available() { return false; }

    std::string get_last_write() const;
    std::string set_read(const std::string content);
};
