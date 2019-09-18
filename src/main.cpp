#include "main.h"

void setup()
{
  pinMode(PHOTORES, INPUT);
  pinMode(RELAY, OUTPUT);

  myClock.begin();
  lcd.begin(16, 2);
  lcd.print("Phitolight v.1.0");

  switchLight = false;
  manager = MAN;
  limitIllumination = 300;
  timeOut = 0;
  currentIllumination = analogRead(PHOTORES);
  delay(1000);
  PrintNormalScreen(myClock.gettime("H:i:s"), analogRead(PHOTORES), manager, switchLight);
  digitalWrite(RELAY, switchLight);
  deviceMode = Normal;
}

void loop()
{
  SettButton.GetState();
  UpButton.GetState();
  DownButton.GetState();

  if(deviceMode == Normal)
  {
    if((millis() - lastTime) >= 1000)
    {
      currentIllumination = analogRead(PHOTORES);
      PrintTime(myClock.gettime("H:i:s"), TIME_POS_X, TIME_POS_Y);
      PrintIllumination(currentIllumination, ILLUM_POS_X, ILLUM_POS_Y);
      
      if(manager != MAN)
      {
        switchLightPrev = switchLight;
        switchLight = isNeedIllumination(switchLight,
                                         currentIllumination,
                                         limitIllumination,
                                         ((startTime + timeOut) < 24 ? startTime + timeOut : (startTime + timeOut) - 24) < myClock.Hours ? true : false);
        startTime = switchLight && switchLight != switchLightPrev ? myClock.Hours : startTime;
        if(switchLight != switchLightPrev) 
        {
          PrintSwitch(switchLight, SWITCH_POS_X, SWITCH_POS_Y);
          digitalWrite(RELAY, switchLight);
        }
      }
      lastTime = millis();
    }
  }
  else
  {
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Settings mode");
    delay(1000);
    lcd.clear();
    ProgSettings();
    lcd.clear();
    PrintNormalScreen(myClock.gettime("H:i:s"), analogRead(PHOTORES), manager, switchLight);
  }
  if(SettButton.isReleased())
  {
    deviceMode = Mode_Settings;
  }
  if(manager == MAN)
  {
    if(UpButton.isReleased())
    {
      if(!switchLight)
      {
        switchLightPrev = switchLight;
        switchLight = true;
        PrintSwitch(switchLight, SWITCH_POS_X, SWITCH_POS_Y);
        digitalWrite(RELAY, switchLight);
      }
    }
    if(DownButton.isReleased())
    {
      if(switchLight)
      {
        switchLightPrev = switchLight;
        switchLight = false;
        PrintSwitch(switchLight, SWITCH_POS_X, SWITCH_POS_Y);
        digitalWrite(RELAY, switchLight);
      }
    }
  }
}

// Function define need switch on illumination or not.
// Args:
//  currentState: current state (on/off);
//  currentIllumination: level of illumination;
//  limit: limit of illumination;
//  isTimeFinished: is time to auxiliary illumination fihished.
bool isNeedIllumination(const bool currentState, const uint32_t currentIllumination, const uint32_t limit, const bool isTimeFinished)
{
  bool needIllum = currentState;

  if(isTimeFinished && needIllum) return false;

  if(currentIllumination < limit) needIllum = true;

  return needIllum;
}

// Print normal work screen.
// Args:
//  currentTime: Current time;
//  illum: current illumination level;
//  currentMode: operating mode;
//  sw: state of illumination.
void PrintNormalScreen(const char* currentTime, const uint32_t illum, const managerMode currentMode, const bool sw)
{
  lcd.clear();
  PrintTime(currentTime, TIME_POS_X, TIME_POS_Y);
  PrintOperatingMode(currentMode, MODE_POS_X, MODE_POS_Y);
  PrintIllumination(illum, ILLUM_POS_X, ILLUM_POS_Y);
  PrintSwitch(sw, SWITCH_POS_X, SWITCH_POS_Y);
}

// Print time.
// Args:
//  currentTime: Current time;
//  xPos: x position to print;
//  yPos: y position to print.
void PrintTime(const char* currentTime, const byte xPos, const byte yPos)
{
  lcd.setCursor(xPos, yPos);
  lcd.print(currentTime);
}

// Print illumination level.
// Args:
//  illum: level of illumination;
//  xPos: x position to print;
//  yPos: y position to print.
void PrintIllumination(const uint32_t illum, const byte xPos, const byte yPos)
{
  lcd.setCursor(xPos, yPos);
  lcd.print("    ");
  lcd.setCursor(xPos, yPos);
  lcd.print(illum);
}

template <typename T> void PrintLimit(const T num, const byte xPos, const byte yPos)
{
  lcd.setCursor(xPos, yPos);
  lcd.print("    ");
  lcd.setCursor(xPos, yPos);
  lcd.print(num);
}

// Print switch on/off.
// Args:
//  sw: current state (on/off);
//  xPos: x position to print;
//  yPos: y position to print.
void PrintSwitch(const bool sw, const byte xPos, const byte yPos)
{
  lcd.setCursor(xPos, yPos);
  lcd.print("   ");
  lcd.setCursor(xPos, yPos);
  lcd.print(sw ? "ON": "OFF");
}

// Print operating mode.
// Args:
//  manager: current operating mode (automatic/manual);
//  xPos: x position to print;
//  yPos: y position to print.
void PrintOperatingMode(const managerMode manager, const byte xPos, const byte yPos)
{
  lcd.setCursor(xPos, yPos);
  lcd.print("    ");
  lcd.setCursor(xPos, yPos);
  lcd.print(manager == AUTO ? "AUTO" : "MAN");
}

// Set operating mode.
void ModeSettingScreen(currentMode &currMode, managerMode &manager)
{
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Mode setting:");

  SettButton.GetState();
  UpButton.GetState();
  DownButton.GetState();

  PrintOperatingMode(manager, MODE_SET_POS_X, MODE_SET_POS_Y);
  while(currMode == Mode_Settings)
  {
    if(SettButton.isHolded())
    {
      currMode = Normal;
    }
    if(SettButton.isReleased())
    {
      currMode = Time_Settings;
    }
    if(DownButton.isReleased())
    {
      manager = MAN;
      PrintOperatingMode(manager, MODE_SET_POS_X, MODE_SET_POS_Y);
    }
    if(UpButton.isReleased())
    {
      manager = AUTO;
      PrintOperatingMode(manager, MODE_SET_POS_X, MODE_SET_POS_Y);
    }
  }
}

// Set illumination limit
void LimitIlumSettingScreen(currentMode &currMode, uint32_t &il)
{
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Limit light:");

  SettButton.GetState();
  UpButton.GetState();
  DownButton.GetState();

  PrintLimit(il, LIMIT_SET_POS_X, LIMIT_SET_POS_Y);
  while(currMode == Illum_Limit_Settings)
  {
    if(SettButton.isHolded())
    {
      currMode = Normal;
    }
    if(SettButton.isReleased())
    {
      currMode = Time_Limit_Settings;
    }
    if(DownButton.isReleased() || DownButton.isHolded())
    {
      il--;
      if(il < 0) il = 0;
      PrintLimit(il, LIMIT_SET_POS_X, LIMIT_SET_POS_Y);
    }
    if(UpButton.isReleased() || UpButton.isHolded())
    {
      il++;
      if(il > 1023) il = 0;
      PrintLimit(il, LIMIT_SET_POS_X, LIMIT_SET_POS_Y);
    }
  }
}

//TODO: finish this function.
void TimeSettingsScreen(currentMode &currMode, char *myTime)
{
  // Set time mode: 0-no 1-sec 2-min 3-hour
  uint8_t VAR_mode_SET = 1; 

  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Time setting:");
  

  SettButton.GetState();
  UpButton.GetState();
  DownButton.GetState();

  PrintTime(myTime, TIME_SET_POS_X, TIME_SET_POS_Y);
  while (currMode == Time_Settings)
  {
    myClock.blinktime(VAR_mode_SET);
    myClock.gettime("H:i:s");
    PrintTime(myTime, TIME_SET_POS_X, TIME_SET_POS_Y);

    if(SettButton.isHolded())
    {
      currMode = Illum_Limit_Settings;
      myClock.blinktime(0);
    }
    if(SettButton.isReleased())
    {
      VAR_mode_SET++;
      VAR_mode_SET = VAR_mode_SET > 3 ? 1 : VAR_mode_SET;
    }
    if(UpButton.isReleased() || UpButton.isHolded())
    {
      switch (VAR_mode_SET)
      {          
        case 1: 
        {
          myClock.settime(0, -1, -1, -1, -1, -1, -1); // sec
          break;
        }
        case 2:
        {
          myClock.settime(-1, (myClock.minutes == 59 ? 0 : myClock.minutes + 1), -1, -1, -1, -1, -1); // min
          break;
        }
        case 3: 
        {
          myClock.settime(-1, -1, (myClock.Hours == 23 ? 0 : myClock.Hours + 1), -1, -1, -1, -1); // hour
          break;
        }
      }
    }
    if(DownButton.isReleased() || DownButton.isHolded())
    {
      switch (VAR_mode_SET)
      {                                     
        case 1:
        {
          myClock.settime(0, -1, -1, -1, -1, -1, -1); //sec
          break;
        }
        case 2: 
        {
          myClock.settime(-1, (myClock.minutes == 0 ? 59 : myClock.minutes - 1), -1, -1, -1, -1, -1); //min
          break;
        }
        case 3:
        {
          myClock.settime(-1, -1, (myClock.Hours == 0 ? 23 : myClock.Hours - 1), -1, -1, -1, -1); //hour
          break;
        }
      }
    }
  }
  
}

// Set time limit.
void LimitTimeSettingsScreen(currentMode &currMode, uint8_t &workTime)
{
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Time limit:");

  SettButton.GetState();
  UpButton.GetState();
  DownButton.GetState();

  PrintLimit(workTime, LIMIT_SET_POS_X, LIMIT_SET_POS_Y);

  while(currMode == Time_Limit_Settings)
  {
    if(SettButton.isHolded())
    {
      currMode = Normal;
    }
    if(SettButton.isReleased())
    {
      currMode = Mode_Settings;
    }
    if(DownButton.isReleased())
    {
      workTime--;
      if(workTime < 0) workTime = 0;
      PrintLimit(workTime, LIMIT_SET_POS_X, LIMIT_SET_POS_Y);
    }
    if(UpButton.isReleased())
    {
      workTime++;
      if(workTime > 24) workTime = 0;
      PrintLimit(workTime, LIMIT_SET_POS_X, LIMIT_SET_POS_Y);
    }
  }
}

// Settings mode.
void ProgSettings()
{
  while(deviceMode != Normal)
  {
    switch (deviceMode)
    {
      case Mode_Settings:
        ModeSettingScreen(deviceMode, manager);
        break;
      case Time_Settings:
        TimeSettingsScreen(deviceMode, myClock.gettime("H:i:s"));
        break;
      case Illum_Limit_Settings:
        LimitIlumSettingScreen(deviceMode, limitIllumination);
        break;
      case Time_Limit_Settings:
        LimitTimeSettingsScreen(deviceMode, timeOut);
        break;
    }
  }
}