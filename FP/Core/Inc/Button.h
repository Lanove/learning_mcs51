#ifndef BUTTON_H
#define BUTTON_H

#include "stm32f4xx_hal.h"
#include <functional>
#include <chrono>

class Button {
public:
    enum ButtonState {
        IDLE,
        PRESSED,
        HOLD,
        DOUBLE_CLICK
    };

    Button(GPIO_TypeDef *port, uint16_t pin, uint32_t holdDurationMs = 1000, uint32_t doubleClickIntervalMs = 200);

    void loop(); // Service loop to handle state updates
    void handleInterrupt(); // Call this in the interrupt callback
    ButtonState getState() const;

private:
    GPIO_TypeDef *port;
    uint16_t pin;
    ButtonState state;

    uint32_t lastPressTime;
    uint32_t lastReleaseTime;
    uint32_t holdDuration;
    uint32_t doubleClickInterval;

    bool isPressed;
};

#endif // BUTTON_H
