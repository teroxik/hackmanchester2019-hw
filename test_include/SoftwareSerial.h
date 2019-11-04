#include <chrono>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

class SoftwareSerial;

using payload = std::string;

class software_serial_control {
 public:
  virtual void wait_rx(payload content,
                       std::chrono::duration<uint, std::milli> timeout) = 0;
  virtual void tx(payload content) = 0;
  virtual void add_auto_rxtx(
      payload request, payload response,
      std::chrono::duration<uint, std::milli> timeout) = 0;
  virtual void set_blocking(bool blocking) = 0;
};

class software_serial_mock {
 private:
  std::vector<std::tuple<int, int, int, SoftwareSerial *>> mock_instances;
  software_serial_mock() = default;

 public:
  static software_serial_mock &instance() {
    static software_serial_mock instance;
    return instance;
  };
  software_serial_mock(software_serial_mock const &) = delete;
  void operator=(software_serial_mock const &) = delete;

  void add(SoftwareSerial *instance, int baud, int rx_pin, const int tx_pin) {
    mock_instances.emplace_back(baud, rx_pin, tx_pin, instance);
  }
  SoftwareSerial *get(const int rx_pin) const;
};

class SoftwareSerial {
 private:
  std::vector<payload> writes;

 public:
  void print(const char *line) { writes.emplace_back(line); }
  void println(const char *line) { writes.emplace_back(line); }
  void write(char){};
  char read() { return -1; };
  bool available() { return false; }
  void begin(int baud, int rx_pin, int tx_pin) {
    software_serial_mock::instance().add(this, baud, rx_pin, tx_pin);
  }

  std::vector<payload> get_writes() const;
  void set_read(payload content);
};
