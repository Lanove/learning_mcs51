const byte ledPin = 13;
const byte incrementPin = 2;
const byte decrementPin = 3;
bool ledState = false;
volatile int led_delay = 100;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(incrementPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(incrementPin), increment, FALLING);
  pinMode(decrementPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(decrementPin), decrement, FALLING);
}

void loop() {
  ledState = !ledState;
  digitalWrite(ledPin, ledState);
  delay(led_delay);
}

void increment() {
  led_delay += 100;
  led_delay = led_delay > 1000 ? 1000 : led_delay;
}

void decrement(){
  led_delay -= 100;
  led_delay = led_delay <= 100 ? 100 : led_delay;
}