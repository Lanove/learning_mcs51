int pin_lm35 = A0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int adc_lm35 = analogRead(pin_lm35);
  int temp_lm35 = (double)adc_lm35 * (500./1024.);
  Serial.print("Temperatur : ");
  Serial.print(temp_lm35);
  Serial.println("C");
  delay(100);
}
