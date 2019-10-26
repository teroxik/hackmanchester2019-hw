// #include <Arduino.h>
// #include <SoftwareSerial.h>

// SoftwareSerial mySerial;

// // the setup function runs once when you press reset or power the board
// void setup()
// {
//     // initialize digital pin LED_BUILTIN as an output.
//     pinMode(LED_BUILTIN, OUTPUT);
//     Serial.begin(9600);
//     mySerial.begin(9600, 5, 6);
// }

// // the loop function runs over and over again forever
// void loop()
// {
//     digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
//     delay(1000);                     // wait for a second
//     digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
//     delay(1000);                     // wait for a second
//     Serial.println("Hello");
// }

#include <SoftwareSerial.h>

//Create software serial object to communicate with SIM800L
SoftwareSerial mySerial; //SIM800L Tx & Rx is connected to Arduino #3 & #2

void updateSerial()
{
    delay(500);
    while (Serial.available())
    {
        mySerial.write(Serial.read()); //Forward what Serial received to Software Serial Port
    }
    while (mySerial.available())
    {
        Serial.write(mySerial.read()); //Forward what Software Serial received to Serial Port
    }
}

void begin()
{
    mySerial.println("AT");
    updateSerial();
    mySerial.println("AT+CSQ");
    updateSerial();
    mySerial.println("AT+CNUM");
    updateSerial();
    mySerial.println("AT+CREG=?");
    updateSerial();
    mySerial.println("AT+CREG=2");
    updateSerial();
    mySerial.println("AT+CPIN?");
    updateSerial();
    Serial.println("Waiting for net");
    delay(10000);

    mySerial.println("AT+CNMI=1,2,0,0,0"); // Decides how newly arrived SMS messages should be handled
    updateSerial();
}

void setup()
{
    //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
    Serial.begin(9600);
    delay(10000);
    Serial.println("GO");

    mySerial.begin(9600, D1, D2);
    begin();
}

int counter = 0;
void loop()
{
    updateSerial();

    counter++;

    if (counter % 10 == 0)
    {
        mySerial.println("AT+CMGF=1");
        updateSerial();
        mySerial.println("AT+CMGS=\"+447595360442\"");
        updateSerial();
        mySerial.print("YO :)");
        updateSerial();
        mySerial.write(26);
    }
}
