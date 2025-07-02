#include <Arduino.h>
#include <MyDelay.h> // Include the MyDelay library for delay functionality
#include <ioPins.h>

MyDelay Scan(5000); // Create a MyDelay object with a 5-second delay
MyDelay Debug(2000); // Create a MyDelay object with a 2-second delay

// Declaire AlarmState states
enum class AlarmState : uint8_t
{
    Normal,
    Alarm,
    Bypass
};
AlarmState currentAlarmState = AlarmState::Normal; // Initialize an AlarmState variable to Normal

// Function definitions
void IsNormal(bool); // Function prototype for the Normal state
void IsAlarm(bool);  // Function prototype for the Alarm state
void IsBypass(bool); // Function prototype for the Bypass state
void IsrSilence();   // Function prototype for the interrupt service routine
void getDebug();     // Function prototype for debugging output

void PinModeSetup()
{
    pinMode(_SilencePin, INPUT_PULLUP); // Set Silence pin as input with pull-up resistor
    pinMode(_WaterPin, INPUT_PULLUP);   // Set Water pin as input with pull-up resistor
    pinMode(_BuzzerPin, OUTPUT);        // Set Buzzer pin as output
    pinMode(_LedPin, OUTPUT);           // Set Led pin as output
}

// declare a pointer array of functions
void (*States[])(bool i){// An array of functions that takes a bool value.  Not using the bool now just playing around with options.  Pg functions without it.
                         IsNormal,
                         IsAlarm,
                         IsBypass,
                         nullptr};

// Global Varialbles
long StartTime = 0;
long SilenceTime = 30000;
bool WaterFlag = false;
bool SilenceFlag = false;
bool TimeOut = true;
uint8_t VolumeOn = 155;
uint8_t VolumeOff = 0;
uint8_t LedFull = 255;
uint8_t LedHalf = 127;
uint32_t BypassF_Rate = 1000;
uint8_t AlarmF_Rate = 200;

void setup()
{
    Serial.begin(115200); // Initialize serial communication at 115200 baud rate
    while (!Serial)
    {
    }; // Wait for serial port to connect (for native USB device
    PinModeSetup();                                                           // Set pin modes for the pins used in the project
    attachInterrupt(digitalPinToInterrupt(_SilencePin), IsrSilence, FALLING); // Attach an interrupt to the Silence button pin
    Serial.println("Begin");
}

// Interrupt Service Routine for the Silence button
// This function is called when the Silence button is pressed
void IsrSilence()
{
    StartTime = millis();
    SilenceFlag = true;
    TimeOut = false;
    currentAlarmState = AlarmState::Bypass; // Set the current alarm state to Bypass
}

void loop()
{
    // Chk if there is water present
    if (!digitalRead(_WaterPin))
    {
        WaterFlag = true;
    }
    else
    {
        WaterFlag = false;
    }
    // chk if the bypass button has not been pushed
    if (WaterFlag && !SilenceFlag)
    {
        currentAlarmState = AlarmState::Alarm;
    }
    // Reset to normal
    if (!WaterFlag)
    {
        currentAlarmState = AlarmState::Normal;
    }
    // Chk if the bypass button has been pressed and the bypass time has not timed out
    if (SilenceFlag && !TimeOut)
    {
        long now = millis();
        if (now - StartTime < SilenceTime)
        {
            currentAlarmState = AlarmState::Bypass;
        }
        else
        {
            TimeOut = true;
            SilenceFlag = false;
        }

        // Call the approrate function based on state
        //  int call = static_cast<int>(CurrentState);  //cast currentstate enum to int
        States[(uint8_t)currentAlarmState](1); // call the appropriate function based on the current alarm state.  The bool value is not used in this case, but it is passed to match the function signature.
    }
    if (Debug.Tick()){
        getDebug();
    }
}

// Subroutine
void IsNormal(bool i)
{
    digitalWrite(_LedPin, LedFull); // Turn off the LED
    analogWrite(_BuzzerPin, VolumeOff); // Turn off the buzzer
}

void IsAlarm(bool i)
{
    analogWrite(_BuzzerPin, VolumeOn);
    analogWrite(_LedPin, LedFull);
    delay(AlarmF_Rate);
    analogWrite(_LedPin, 0);
    delay(AlarmF_Rate);
}

void IsBypass(bool i)
{
    analogWrite(_BuzzerPin, VolumeOff);
    analogWrite(_LedPin, LedFull);
    delay(BypassF_Rate);
    analogWrite(_LedPin, 0);
    delay(BypassF_Rate);
}

void getDebug()
{
    Serial.print("WaterFlag: ");
    Serial.println(WaterFlag);
    Serial.print("SilenceFlag: ");
    Serial.println(SilenceFlag);
    Serial.print("TimeOut: ");
    Serial.println(TimeOut);
    Serial.print("Current Alarm State: ");
    Serial.println((uint8_t)currentAlarmState);
}