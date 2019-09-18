#include "button.h"

Button::Button(uint8_t connectedPin)
{
    buttonPin = connectedPin;
    pinMode(buttonPin, INPUT_PULLUP);
    timeout = 1000;
    debounceTime = 50;
}

bool Button::isHolded()
{
    bool retVal = false;
    Button::GetState();
    if(flags.btnHolded)
    {
        retVal = flags.btnHolded;
        flags.btnHold = false;
        flags.btnHolded = false;
    }
    return retVal;
}

bool Button::isReleased()
{
    bool retVal = false;
    Button::GetState();
    if(flags.btnChangedState) 
    {
        retVal = flags.btnReleased;
        flags.btnReleased = false;
    }
    return retVal;
}

void Button::GetState()
{
    flags.btnCurrentState = digitalRead(buttonPin);

    if(!flags.btnState)
    {
        if(!flags.btnDebounce)
        {  
            flags.btnDebounce = true;
            flags.btnState = false;
            currTime = millis();
        }
        else
        {
            if((millis() - currTime) >= debounceTime)
            {
                flags.btnState = true;
                flags.btnDebounce = false;
                flags.btnPrevState = flags.btnLastState;
                flags.btnLastState = !flags.btnCurrentState;
            }
        }   
    }
    if(flags.btnState)
    {
        if(flags.btnLastState ^ flags.btnPrevState)
        {
            if(flags.btnHold) flags.btnReleased = !flags.btnLastState;
            flags.btnHold = true;
            flags.btnHolded = false;
            flags.btnChangedState = true;
        }
        else
        {
            flags.btnChangedState = false;
        }
        
        if(flags.btnLastState && flags.btnChangedState) 
        {
            lastTime = millis();
        }
        if(flags.btnHold && flags.btnLastState && flags.btnPrevState && (millis() - lastTime) >= timeout)
        {
            flags.btnHolded = true;
        }
    }
    flags.btnState = false;
}