#include <Arduino.h>
#include <Wire.h>
#include <ThingSpeak.h> // Include the ThingSpeak library
#include <WiFi.h> 

// Pin Definitions
const int flexPin = A0;
const int ledPin = 23;
const int xPin = 34;
const int yPin = 35;
const int zPin = 32;

#define HMC5883L_ADDR 0x1E

// ThingSpeak settings
const char* ssid = ""; // Your WiFi SSID
const char* password = ""; // Your WiFi password
const char* thingSpeakApiKey = ""; // Your ThingSpeak API key
const unsigned long updateInterval = 15000; // Update interval in milliseconds (15 seconds)
unsigned long lastUpdateTime = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  Wire.begin();
  delay(100);
  Serial.println("Initializing HMC5883L...");
  Wire.beginTransmission(HMC5883L_ADDR);
  Wire.write(0x02); // Select mode register
  Wire.write(0x00); // Continuous measurement mode
  Wire.endTransmission();

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}

void loop() {
  // Check if it's time to update ThingSpeak
  if (millis() - lastUpdateTime >= updateInterval) {
    sendToThingSpeak(); // Send data to ThingSpeak
    lastUpdateTime = millis(); // Update last update time
  }

  // Read sensors and print values
  readSensors();
  delay(1000); // Delay for stability
}

void readSensors() {
  // Read flex sensor value
  int flexValue = analogRead(flexPin);
  Serial.print("Flex sensor: ");
  Serial.println(flexValue);

  // Control LED based on flex sensor value
  if (flexValue > 3600)
    digitalWrite(ledPin, HIGH);
  else
    digitalWrite(ledPin, LOW);

  // Read accelerometer values
  int xValue = analogRead(xPin);
  int yValue = analogRead(yPin);
  int zValue = analogRead(zPin);

  // Print accelerometer values
  Serial.print("X: ");
  Serial.print(xValue);
  Serial.print(", Y: ");
  Serial.print(yValue);
  Serial.print(", Z: ");
  Serial.println(zValue);

  // Read gyroscope values
  int x, y, z;
  Wire.requestFrom(HMC5883L_ADDR, 6);
  if (Wire.available() == 6) {
    x = Wire.read() << 8 | Wire.read();
    z = Wire.read() << 8 | Wire.read();
    y = Wire.read() << 8 | Wire.read();
  }
  int heading = atan2(y, x);
  if (heading < 0)
    heading += 2 * PI;
  int degrees = heading * 180 / M_PI;
  Serial.print("Heading: ");
  Serial.print(degrees);
  Serial.println(" degrees");
}

void sendToThingSpeak() {
  // Prepare data to send to ThingSpeak
  String data_set = String("field1=") + analogRead(xPin) + "&field2=" + analogRead(yPin) + "&field3=" + analogRead(zPin) + "&field4=" + analogRead(flexPin) + "&field5=" + random(100) + "&field6=" + random(100) + "&field7=" + random(100);

  // Send data to ThingSpeak
  int statusCode = ThingSpeak.writeFields(Channel_ID, thingSpeakApiKey, data_set);
  if (statusCode == 200) {
    Serial.println("Data sent to ThingSpeak successfully!");
  } else {
    Serial.println("Failed to send data to ThingSpeak. Status code: " + String(statusCode));
  }
}
