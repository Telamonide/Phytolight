#include <Arduino.h>
#ifndef BUTTON_H
#define BUTTON_H

typedef struct
{
    bool btnDebounce = false; // Кнопка в режиме дребезга.
    bool btnHolded = false; // Кнопка в режиме удержания.
    bool btnHold = false; // Кнопка в режиме удержания.
    bool btnChangedState = false; // Положение кнопки поменялось;
    bool btnState = false; // Положение кнопки дребезг кнопки прошел;
    bool btnPrevState = false; // Предыдущее состояние кнопки.
    bool btnLastState = false; // Текущее состояние кнопки.
    bool btnCurrentState = false; // Считанное состояние кнопки;
    bool btnReleased = false;
} buttonFlags;

class Button
{
    public:
        Button(uint8_t);
        void GetState();
        bool isReleased();
        bool isHolded();
    private:
        uint32_t currTime;
        uint32_t lastTime;
        uint32_t debounceTime;
        uint32_t timeout;
        uint8_t buttonPin;
        buttonFlags flags;
};
#endif