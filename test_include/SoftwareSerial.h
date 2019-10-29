#include <string>

class SoftwareSerial
{
public:
    void print(const char *){};
    void println(const char *){};
    void begin(int, int, int){};
    void write(char){};
    char read() { return -1; };
    bool available() { return false; }
};