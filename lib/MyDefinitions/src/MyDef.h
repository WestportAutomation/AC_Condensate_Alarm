#ifndef MYDEF_H
#define MYDEF_H
#include <Arduino.h>

// Declaire AlarmState states
enum class AlarmState : uint8_t
{
    Normal,
    Alarm,
    Bypass
};

// Function definitions
void IsNormal(bool); // Function prototype for the Normal state
void IsAlarm(bool);  // Function prototype for the Alarm state
void IsBypass(bool); // Function prototype for the Bypass state
void IsrSilence();   // Function prototype for the interrupt service routine
void getDebug();     // Function prototype for debugging output

// declare a pointer array of functions
void (*States[])(bool i){// An array of functions that takes a bool value.  Not using the bool now just playing around with options.  Pg functions without it.
                         IsNormal,
                         IsAlarm,
                         IsBypass,
                         nullptr};

// Global Varialbles
long StartTime = 0;
long BypassTimer = 30000;
bool WaterFlag = false;
bool SilenceFlag = false;
bool TimeOut = true;
uint8_t VolumeOn = 255;
uint8_t VolumeOff = 0;
uint8_t LedFull = 255;
uint8_t LedHalf = 127;
uint32_t BypassF_Rate = 1000;
uint8_t AlarmF_Rate = 200;

#endif// MyDef.h - Header file for MyDefinitions library