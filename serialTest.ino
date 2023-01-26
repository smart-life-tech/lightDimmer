#include <Dimmer.h> //

// #define USE_SERIAL  SerialUSB //Serial for boards whith USB serial port
#define USE_SERIAL Serial
#define outputPin 3 // pwm pin
#define zerocross 2
// dimmerLamp dimmer(outputPin, zerocross);
//dimmerLamp dimmer(outputPin); // initialase port for dimmer for MEGA, Leonardo, UNO, Arduino M0, Arduino Zero
Dimmer dimmer(3, DIMMER_COUNT);
int outVal = 0;

void setup()
{
    USE_SERIAL.begin(9600);
    dimmer.begin(); // dimmer initialisation: name.begin(MODE, STATE)
    USE_SERIAL.println("Dimmer Program is starting...");
    USE_SERIAL.println("Set value");
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
    int preVal = outVal;

    if (USE_SERIAL.available())
    {
        int buf = USE_SERIAL.parseInt();
        if (buf != 0)
        {
            outVal = buf;
            if(outVal>100)outVal=100;
        }
        dimmer.set(outVal); // setPower(0-100%);
        delay(200);
    }

    if (preVal != outVal)
    {
        dimmer.set(outVal); // setPower(0-100%);
        USE_SERIAL.print("lampValue -> ");
        printSpace(outVal);
       // USE_SERIAL.print(dimmer.getPower());
        USE_SERIAL.println("%");
        USE_SERIAL.println("Dimmer Program ...");
        USE_SERIAL.println("Set value from 0 to 100");
    }
    delay(50);
}