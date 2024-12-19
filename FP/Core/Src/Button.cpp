#include "Button.h"
#include "stm32f4xx_hal.h"

Button::Button(GPIO_TypeDef *port, uint16_t pin, uint32_t holdDurationMs, uint32_t doubleClickIntervalMs)
    : port(port), pin(pin), state(IDLE), lastPressTime(0), lastReleaseTime(0),
      holdDuration(holdDurationMs), doubleClickInterval(doubleClickIntervalMs), isPressed(false) {}


void Button::handleInterrupt() {
    uint32_t currentTime = HAL_GetTick();

    if (HAL_GPIO_ReadPin(port, pin) == GPIO_PIN_RESET) { // Button pressed
        if (!isPressed) {
            isPressed = true;
            lastPressTime = currentTime;


            if (currentTime - lastReleaseTime <= doubleClickInterval) {
                state = DOUBLE_CLICK;
            } else {
                state = PRESSED;
            }
        }

    } else { // Button released
        if (isPressed) {
            isPressed = false;
            lastReleaseTime = currentTime;

            state = IDLE; // Reset state to idle after release
        }
    }
}

void Button::loop() {
    uint32_t currentTime = HAL_GetTick();

    if (isPressed && (currentTime - lastPressTime >= holdDuration)) {
        state = HOLD;
    }
}

Button::ButtonState Button::getState() const {
    return state;
}
