#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include "LiquidCrystal.h"
#include "iarduino_RTC.h"
#include "button.h"

// Define lcd pins.
#define RS 12
#define EN 11
#define D4 5
#define D5 4
#define D6 3
#define D7 2
//----------------------

// Normal mode positions.
#define TIME_POS_X 0
#define TIME_POS_Y 0
#define ILLUM_POS_X 0
#define ILLUM_POS_Y 1
#define SWITCH_POS_X 12
#define SWITCH_POS_Y 1
#define MODE_POS_X 12
#define MODE_POS_Y 0
// Setting mode positions.
#define TIME_SET_POS_X 4
#define TIME_SET_POS_Y 1
#define LIMIT_SET_POS_X 6
#define LIMIT_SET_POS_Y 1
#define MODE_SET_POS_X 6
#define MODE_SET_POS_Y 1

// Define buttons pins.
#define UP 8
#define DOWN 7
#define SETT 9
//----------------------
#define RELAY 13// Relay pin
#define PHOTORES A0 // Photoresistor pin.

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

iarduino_RTC myClock(RTC_DS3231);

Button UpButton(UP);
Button DownButton(DOWN);
Button SettButton(SETT);

enum currentMode : byte
{
    Normal, // Normal working mode.
    Mode_Settings, // Set working mode.
    Time_Settings, // Set time.
    Illum_Limit_Settings, // Set illumination limit.
    Time_Limit_Settings // Set time limit.
} deviceMode;
// Operating mode.
enum managerMode : bool 
{
    AUTO, // Automatic switch on/off. 
    MAN // Manual switch on/off.
} manager;

long unsigned lastTime;
// Timeout to work illumination.
uint8_t timeOut;
// Time when light was switched on.
uint8_t startTime;
// State of illumination.
bool switchLight;
// State of illumination on previsious step.
bool switchLightPrev;
// Illumination's level to switch on light.
uint32_t limitIllumination;
// Current level of illumination.
uint32_t currentIllumination;

// Function define need switch on illumination or not.
bool isNeedIllumination(const bool,
                        const uint32_t, 
                        const uint32_t, 
                        const bool);
// Print normal work screen.
void PrintNormalScreen(const char*, const uint32_t, const managerMode, const bool);
// Print time.
void PrintTime(const char*, const byte, const byte);
// Print illumination level.
void PrintIllumination(const uint32_t, const byte, const byte);
//Print limit.
template <typename T> void PrintLimit(const T, const byte, const byte);
// Print operating mode.
void PrintOperatingMode(const managerMode, const byte, const byte);
// Print switch on/off.
void PrintSwitch(const bool, const byte, const byte);

void ProgSettings();
void TimeSettingsScreen(currentMode&, char*);
void ModeSettingScreen(currentMode&, managerMode&);
void LimitIlumSettingScreen(currentMode&, uint32_t&);
void LimitTimeSettingsScreen(currentMode&, uint8_t&);

#endif