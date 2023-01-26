#include <Dimmer.h> //

// #define USE_SERIAL  SerialUSB //Serial for boards whith USB serial port
#define USE_SERIAL Serial
#define outputPin 3 // pwm pin
#define zerocross 2
// dimmerLamp dimmer(outputPin, zerocross);
// dimmerLamp dimmer(outputPin); // initialase port for dimmer for MEGA, Leonardo, UNO, Arduino M0, Arduino Zero
Dimmer dimmer(3, DIMMER_COUNT);
int outVal = 0;

void setup()
{
    USE_SERIAL.begin(9600);
    dimmer.begin(); // dimmer initialisation: name.begin(MODE, STATE)
    USE_SERIAL.println("Dimmer Program is starting...");
    USE_SERIAL.println("auto matic mode") ;
}

void printSpace(int val)
{
    if ((val / 100) == 0)
        USE_SERIAL.print(" ");
    if ((val / 10) == 0)
        USE_SERIAL.print(" ");
}

void loop()
{
    for (int i = 0; i < 100; i++)
    {
        dimmer.set(i); // setPower(0-100%);
        delay(500);
        printSpace(outVal);
        USE_SERIAL.println("%");
        USE_SERIAL.println("Dimmer Program ...");
    }
}