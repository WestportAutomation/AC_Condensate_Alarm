#include <Arduino.h>
#include <MyDelay.h> // Include the MyDelay library for delay functionality
#include <ioPins.h>  // Include the ioPins library for pin definitions and setup
#include <MyDef.h> // Include the MyDef library for definitions

MyDelay Scan(5000); // Create a MyDelay object with a 5-second delay
MyDelay Debug(2000); // Create a MyDelay object with a 2-second delay

AlarmState currentAlarmState = AlarmState::Normal; // Initialize an AlarmState variable to Normal

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
        if (now - StartTime < BypassTimer)
        {
            currentAlarmState = AlarmState::Bypass;
        }
        else
        {
            TimeOut = true;
            SilenceFlag = false;
        }
    }
         // Call the approrate function based on state
        //  int call = static_cast<int>(CurrentState);  //cast currentstate enum to int
        States[(uint8_t)currentAlarmState](1); // call the appropriate function based on the current alarm state.  The bool value is not used in this case, but it is passed to match the function signature.
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
