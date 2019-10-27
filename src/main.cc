#include "sms.h"
#include "pod.h"
#include <string>
#include "pins.h"
#include "vote.h"

transport sms;
pod_container pod_container;
vote vote;
const std::string callback_number = "+447860033807";
// const std::string callback_number = "+447595360442";

void setup()
{
    Serial.begin(9600);
    delay(5000); // magic delay for network conn

    vote.begin();
    sms.begin();
    pod_container.begin();
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
            vote.vote_now();
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

    auto vote_result = vote.loop();
    if (vote_result.is_valid())
    {
        char data[10] = {0};
        sprintf(data, "V%d", vote_result.value);
        sms.send(callback_number, std::string(data));
    }
}
