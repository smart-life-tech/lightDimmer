const int dimmerPin = 3; // Pin connected to dimmer circuit
int dimmerValue = 0;     // Initial dimmer value

void setup()
{
    pinMode(dimmerPin, OUTPUT); // Set dimmerPin as output
}

void loop()
{
    for (dimmerValue = 0; dimmerValue <= 255; dimmerValue++)
    {
        analogWrite(dimmerPin, dimmerValue); // Output PWM signal with increasing duty cycle
        delay(500);                           // Delay between increments
    }
    for (dimmerValue = 255; dimmerValue >= 0; dimmerValue--)
    {
        analogWrite(dimmerPin, dimmerValue); // Output PWM signal with decreasing duty cycle
        delay(500);                           // Delay between decrements
    }
}