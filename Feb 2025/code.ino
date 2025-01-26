// Importing libraries
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>

// Firebase requires these helpers
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// Wi-Fi credentials
#define WIFI_SSID "insert Wifi Name here"
#define WIFI_PASSWORD "insert Wifi Password here"

// Firebase credentials
#define API_KEY "insert API Key here"
#define DATABASE_URL "insert API Url here"

// Hardware components (LED & button)
const int ledPin = D2; 
const int buttonPin = D5;
const int buttonLedPin = D6;

// Global variables
bool firebaseData = false;
int buttonState = 1;

// Define Firebase Data object and other necessary objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
unsigned long lastFirebaseUpdate = 0;
int count = 0;
bool signupOK = false;

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buttonLedPin, OUTPUT);

  connectWiFi();
  connectFirebase();
  digitalWrite(buttonLedPin, HIGH);
}

void loop() {
  if (Firebase.ready() && signupOK && (millis() - lastFirebaseUpdate > 1000)) {  // Avoid overwhelming the database
    lastFirebaseUpdate = millis();

    buttonState = digitalRead(buttonPin);
    Serial.println(buttonState);
    uploadData(buttonState);
    downloadData();
    manageLED(buttonState, firebaseData);
  }
  delay(5);
}

void connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(buttonLedPin, HIGH);
    delay(500);
    digitalWrite(buttonLedPin, LOW);
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected with IP: ");
  Serial.println(WiFi.localIP());
}

void connectFirebase() {
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Firebase initialized successfully");
    signupOK = true;
  } else {
    Serial.printf("Firebase signup error: %s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void uploadData(int buttonstate) {
  if (Firebase.RTDB.setInt(&fbdo, "/user_1", buttonstate)) {
    Serial.println("Data UPLOAD successful");
  } else {
    Serial.printf("Data UPLOAD failed: %s\n", fbdo.errorReason().c_str());
  }
}

void downloadData() {
  if (Firebase.RTDB.getInt(&fbdo, "/user_2")) {
    firebaseData = fbdo.intData();
    Serial.println("Data DOWNLOAD successful");
  } else {
    Serial.printf("Data DOWNLOAD failed: %s\n", fbdo.errorReason().c_str());
  }
}

void manageLED(int buttonState, int firebaseData) {
  if (buttonState == HIGH || firebaseData == HIGH) {
    digitalWrite(ledPin, HIGH);
    delay(1000);
    digitalWrite(ledPin, LOW);
  } else {
    digitalWrite(ledPin, LOW);
  }
}
