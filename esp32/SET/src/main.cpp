#include "si5351.h"
#include <Arduino.h>
#include <Firebase_ESP_Client.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <Wire.h>
#include <addons/RTDBHelper.h>
#include <addons/TokenHelper.h>

// I2C device found at address 0x23
// I2C device found at address 0x60
#define I2C_SDA 21
#define I2C_SCL 22
#define DDS_MULTIPLIER 100ULL
#define LCD_PERIOD 2000

#define WIFI_SSID "realme C15"
#define WIFI_PASSWORD "lpkojihu"

#define API_KEY "AIzaSyCWzuvdP0zmkR30zkM6ekgVc2hgGnLlcCg"
#define USER_EMAIL "esp32@esp32.com"
#define USER_PASSWORD "esp32esp32"
#define DATABASE_URL "https://kohigashi-b72ca-default-rtdb.firebaseio.com/"
#define DATABASE_SECRET "DATABASE_SECRET"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

LiquidCrystal_I2C lcd(0x23, 16, 2);
Si5351 dds;

bool ledState = false;
bool last_get_state = false;
bool lcd_state = true;
uint8_t button_pin = 26;
uint8_t led_pin = 12;
uint32_t last_millis;
long ch0_val, ch1_val, ch2_val;

int count = 0;
void scan_i2c();

void setup() {

  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");
  unsigned long ms = millis();
  Wire.begin();

  lcd.init(I2C_SDA, I2C_SCL); // initialize the lcd to use user defined I2C pins
  lcd.backlight();
  lcd.clear();
  if (!dds.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0)) {
    lcd.setCursor(0, 0);
    lcd.print("DDS Not FOUND");
    while (1)
      ;
  }

  while (WiFi.status() != WL_CONNECTED) {
    lcd.print("Wait Wi-Fi");
    delay(300);
    lcd.clear();
    delay(300);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connected");
  lcd.setCursor(0, 1);
  lcd.print("Wait FB Auth");

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;

  Firebase.reconnectNetwork(true);

  fbdo.setBSSLBufferSize(4096, 1024);

  fbdo.setResponseSize(4096);

  String base_path = "/UsersData/";

  config.token_status_callback =
      tokenStatusCallback; // see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  String var = "$userId";
  String val = "($userId === auth.uid && auth.token.premium_account === true "
               "&& auth.token.admin === true)";
  Firebase.RTDB.setReadWriteRules(&fbdo, base_path, var, val, val,
                                  DATABASE_SECRET);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connected");
  lcd.setCursor(0, 1);
  lcd.print("Press to start");

  pinMode(button_pin, INPUT_PULLUP);
  pinMode(led_pin, OUTPUT);
}

void loop() {
  static bool last_buttonState;
  bool buttonState = digitalRead(button_pin);
  if (buttonState != last_buttonState && buttonState == 0) {
    last_get_state = false;
    if (!Firebase.ready()) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("FB not ready");
      Serial.println("Firebase not ready");
      digitalWrite(led_pin, LOW);
      return;
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Getting data...");
    digitalWrite(led_pin, HIGH);
    bool ch0_get = Firebase.RTDB.getString(&fbdo, "/dds/ch0");
    if (ch0_get) {
      String ch0_string = fbdo.stringData();
      ch0_val = ch0_string.toInt();
    }
    bool ch1_get = Firebase.RTDB.getString(&fbdo, "/dds/ch1");
    if (ch1_get) {
      String ch1_string = fbdo.stringData();
      ch1_val = ch1_string.toInt();
    }
    bool ch2_get = Firebase.RTDB.getString(&fbdo, "/dds/ch2");
    if (ch2_get) {
      String ch2_string = fbdo.stringData();
      ch2_val = ch2_string.toInt();
    }
    if (ch0_get && ch1_get && ch2_get) {
      last_get_state = true;
      Serial.printf("Ch0: %d, Ch1: %d, Ch2: %d\n", ch0_val, ch1_val, ch2_val);
      dds.set_freq(ch0_val * 1000 * DDS_MULTIPLIER, SI5351_CLK0);
      dds.set_freq(ch1_val * 1000 * DDS_MULTIPLIER, SI5351_CLK1);
      dds.set_freq(ch2_val * 1000 * DDS_MULTIPLIER, SI5351_CLK2);
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Fail to get data");
    }
    digitalWrite(led_pin, LOW);
  }

  if (millis() - last_millis >= LCD_PERIOD && last_get_state) {
    last_millis = millis();
    if (lcd_state) {
      lcd.clear();
      lcd.setCursor(0, 0);
      if(ch0_val < 1000)
        lcd.printf("%ldkHz", ch0_val);
      else
        lcd.printf("%.1fMHz", (float) ch0_val / 1000.);
      lcd.setCursor(9, 0);
      if(ch1_val < 1000)
        lcd.printf("%ldkHz", ch1_val);
      else
        lcd.printf("%.1fMHz", (float) ch1_val / 1000.);
      lcd.setCursor(0, 1);
      if(ch2_val < 1000)
        lcd.printf("%ldkHz", ch2_val);
      else
        lcd.printf("%.1fMHz", (float) ch2_val / 1000.);
    }else{
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("CH0");
      lcd.setCursor(9, 0);
      lcd.print("CH1");
      lcd.setCursor(0, 1);
      lcd.print("CH2");
    }
    lcd_state = !lcd_state;
  }

  last_buttonState = buttonState;
}

void scan_i2c() {
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
      nDevices++;
    } else if (error == 4) {
      Serial.print("Unknow error at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  } else {
    Serial.println("done\n");
  }
}