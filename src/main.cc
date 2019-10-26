#include "sms.h"
#include "pod.h"
#include <string>

transport sms;
pod_container pod_container;
const std::string callback_number = "+447860033807";

void setup()
{
    sms.begin();
    pod_container.begin();
}

void loop()
{
    sms.loop();
    if (sms.last_message().is_valid())
    {
        Serial.println("Got valid command");
        Serial.println(sms.last_message().code);
        Serial.println(sms.last_message().param);
        if (sms.last_message().code == 'T')
            pod_container.take_now(sms.last_message().param);
    }

    auto updated_pods = pod_container.loop();
    for (auto &&pod : updated_pods)
    {
        std::string prefix;
        char data[10] = {0};
        if (pod.taken_incorrectly())
            sprintf(data, "W%d", pod.day);
        else
            sprintf(data, "R%d", pod.day);

        sms.send(callback_number, std::string(data));
    }
}
