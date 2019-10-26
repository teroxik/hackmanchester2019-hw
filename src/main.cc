#include "sms.h"

transport sms;
const std::string callback_number = "+447860033807";

void setup()
{
    sms.begin();
}

void loop()
{
    sms.loop();
    Serial.println(sms.last_payload().c_str());
    if (sms.last_message().is_valid())
    {
        Serial.println("Got valid command");
        Serial.println(sms.last_message().code);
        Serial.println(sms.last_message().param);
        sms.send(callback_number, "Got valid cmd");
    }
}
