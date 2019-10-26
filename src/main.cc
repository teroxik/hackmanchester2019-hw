#include "sms.h"
#include "pod.h"
#include <string>
#include "pins.h"

transport sms;
pod_container pod_container;
// const std::string callback_number = "+447860033807";
const std::string callback_number = "+447595360442";

void setup()
{
    sms.begin();
    pod_container.begin();

    pinMode(pins::vote_0, INPUT_PULLUP);
    //pinMode(pins::vote_1, INPUT_PULLUP);
    //pinMode(pins::vote_2, INPUT_PULLUP);
    pinMode(pins::vote_now, OUTPUT);
    digitalWrite(pins::vote_now, LOW);
}

void loop()
{
    sms.loop();
    auto lm = sms.last_message();
    if (lm.is_valid())
    {
        Serial.println("Got valid command");
        Serial.println(lm.code);
        Serial.println(lm.param);
        if (lm.code == 'T')
            pod_container.take_now(lm.param);
        if (lm.code == 'V')
            digitalWrite(pins::vote_now, HIGH);
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

    int x = analogRead(pins::vote_0);
    bool v0 = x > 200 && x < 300;
    bool v1 = x > 500 && x < 600;
    bool v2 = x > 800 && x < 900;

    if (v0 || v1 || v2)
    {
        digitalWrite(pins::vote_now, LOW);
        if (v0)
            sms.send(callback_number, "V0");
        if (v1)
            sms.send(callback_number, "V1");
        if (v2)
            sms.send(callback_number, "V2");
    }
}
