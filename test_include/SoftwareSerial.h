#include <chrono>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

class SoftwareSerial;

class software_serial_control {
 public:
  virtual void wait_rx(
      std::string write,
      std::chrono::duration<uint, std::milli> timeout) = 0;
  virtual void tx(std::string tx) = 0;
  virtual void add_auto_rxtx(
      std::string request, std::string response,
      std::chrono::duration<uint, std::milli> timeout) = 0;
  virtual void set_blocking(bool blocking) = 0;
};

class software_serial_mock {
 private:
  std::vector<std::tuple<int, int, int, SoftwareSerial *>> mock_instances;
  software_serial_mock() = default;;

 public:
  static software_serial_mock &instance() {
    static software_serial_mock instance;
    return instance;
  };
  software_serial_mock(software_serial_mock const &) = delete;
  void operator=(software_serial_mock const &) = delete;

  void add(SoftwareSerial *instance, const int baud, const int rx_pin,
           const int tx_pin) {
    mock_instances.emplace_back(baud, rx_pin, tx_pin, instance);
  }
  SoftwareSerial *get(const int rx_pin) const;
};

class SoftwareSerial {
 private:
  std::vector<std::string> writes;

 public:
  void print(const char *line) { writes.emplace_back(line); }
  void println(const char *line) { writes.emplace_back(line); }
  void write(char){};
  char read() { return -1; };
  bool available() { return false; }
  void begin(int baud, int rx_pin, int tx_pin) {
    software_serial_mock::instance().add(this, baud, rx_pin, tx_pin);
  }

  std::vector<std::string> get_writes() const;
  std::string set_read(const std::string content);
};
