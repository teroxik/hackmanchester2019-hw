#include <io.h>
#include <vector>
#include <unity.h>

void test_parse_payload(void)
{
    std::vector<payload> payloads;
    auto result = parse_payload(payloads);
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_parse_payload);
    UNITY_END();

    return 0;
}