#include "io.h"

transport sms;

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
        sms.send("+447595360442", "Got valid cmd");
    }
}

/*
#include <SoftwareSerial.h>

SoftwareSerial gsm;

void updateSerial()
{
    delay(500);
    while (Serial.available())
    {
        gsm.write(Serial.read());
    }
    while (gsm.available())
    {
        Serial.write(gsm.read());
    }
}

void begin()
{
    gsm.println("AT");
    updateSerial();
    gsm.println("AT+CSQ");
    updateSerial();
    gsm.println("AT+CNUM");
    updateSerial();
    gsm.println("AT+CREG=?");
    updateSerial();
    gsm.println("AT+CREG=2");
    updateSerial();
    gsm.println("AT+CPIN?");
    updateSerial();
    Serial.println("Waiting for net");
    delay(10000);

    gsm.println("AT+CNMI=1,2,0,0,0");
    updateSerial();
}

void setup()
{
    Serial.begin(9600);
    delay(10000);
    Serial.println("GO");

    gsm.begin(9600, D1, D2);
    begin();
}

int counter = 0;
void loop()
{
    updateSerial();

    counter++;

    if (counter % 100 == 0)
    {
        gsm.println("AT+CMGF=1");
        updateSerial();
        gsm.println("AT+CMGS=\"+447595360442\"");
        updateSerial();
        gsm.print("YO :)");
        updateSerial();
        gsm.write(26);
    }
}
*/