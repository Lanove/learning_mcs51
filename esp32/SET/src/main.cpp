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
    while(1);
  }

  while (WiFi.status() != WL_CONNECTED) {
    lcd.print("Wait Wi-Fi");
    delay(300);
    lcd.clear();
    delay(300);
  }

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
  String val = "($userId === auth.uid && auth.token.premium_account === true && auth.token.admin === true)";
  Firebase.RTDB.setReadWriteRules(&fbdo, base_path, var, val, val,
                                  DATABASE_SECRET);

  // pinMode(26, INPUT_PULLUP);
  // pinMode(12, OUTPUT);
}

void loop() {
  // scan_i2c();
  // static bool last_buttonState;
  // bool buttonState = digitalRead(26);
  // if (buttonState != last_buttonState && buttonState == 0) {
  //   Serial.printf("Set counter... %s to %d\n",
  //                 Firebase.RTDB.setInt(&fbdo, "/Lampu/counter", ++count)
  //                     ? "ok"
  //                     : fbdo.errorReason().c_str(),
  //                 count);
  //   if (Firebase.RTDB.getBool(&fbdo, "/Lampu/Lampu1")) {

  //     String fbdoData = fbdo.stringData();
  //     Serial.printf("Get OK, Data: %s\n", fbdoData.c_str());
  //     if (fbdoData == "true")
  //       ledState = true;
  //     else
  //       ledState = false;

  //     digitalWrite(12, ledState);
  //   } else {
  //     Serial.println("Get Failed");
  //   }
  // }
  // last_buttonState = buttonState;
  // Firebase.ready();
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