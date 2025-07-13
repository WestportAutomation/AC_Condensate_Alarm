#ifndef IOPINS_H
#define IOPINS_H
    static const int _SilencePin = 2; // Example pin number 
    static const int _WaterPin = 3;   // Example pin number
    static const int _BuzzerPin = 9;  // pin chosen because pwm
    static const int _LedPin = 10;    // chosen because of pwm

    void PinModeSetup()
{
    pinMode(_SilencePin, INPUT_PULLUP); // Set Silence pin as input with pull-up resistor
    pinMode(_WaterPin, INPUT_PULLUP);   // Set Water pin as input with pull-up resistor
    pinMode(_BuzzerPin, OUTPUT);        // Set Buzzer pin as output
    pinMode(_LedPin, OUTPUT);           // Set Led pin as output
}

#endif
