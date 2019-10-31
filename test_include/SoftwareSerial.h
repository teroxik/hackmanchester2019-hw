#include <string>
#include <memory>
#include <vector>
#include <map>
#include <iostream>
#include <chrono>

class SoftwareSerial;

class software_serial_control
{
public:
    virtual void wait_rx(const std::string write, const std::chrono::duration<uint, std::milli> timeout) = 0;
    virtual void tx(const std::string tx) = 0;
    virtual void add_auto_rxtx(const std::string request,
                               const std::string response,
                               const std::chrono::duration<uint, std::milli> timeout) = 0;
    virtual void set_blocking(const bool blocking) = 0;
};

class software_serial_mock
{
private:
    std::vector<std::tuple<int, int, int, SoftwareSerial *>> mock_instances;
    software_serial_mock(){};

public:
    static software_serial_mock &instance()
    {
        static software_serial_mock instance;
        return instance;
    };
    software_serial_mock(software_serial_mock const &) = delete;
    void operator=(software_serial_mock const &) = delete;

    void add(SoftwareSerial *instance, const int baud, const int rx_pin, const int tx_pin)
    {
        mock_instances.push_back(std::make_tuple(baud, rx_pin, tx_pin, instance));
    }
    SoftwareSerial *get(const int rx_pin) const;
};

class SoftwareSerial
{
private:
    std::vector<std::string> writes;

public:
    void print(const char *line) { writes.push_back(std::string(line)); }
    void println(const char *line) { writes.push_back(std::string(line)); }
    void write(char){};
    char read() { return -1; };
    bool available() { return false; }
    void begin(int baud, int rx_pin, int tx_pin)
    {
        software_serial_mock::instance().add(this, baud, rx_pin, tx_pin);
    }

    std::vector<std::string> get_writes() const;
    std::string set_read(const std::string content);
};
