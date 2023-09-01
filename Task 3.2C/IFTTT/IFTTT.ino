// SIT210 Task 3.2C
// Name: Om Tomar
// Roll No: 2210994882

#include <WiFiNINA.h>
#include <BH1750.h>
#include <Wire.h>

// Please enter your sensitive data in the Secret tab
char ssid[] = "Om";            // Your Wi-Fi network SSID
char pass[] = "87654321";      // Your Wi-Fi network password

WiFiClient client;              // Create a WiFi client instance
BH1750 lightMeter;              // Create an instance of the BH1750 light sensor

char   HOST_NAME[] = "maker.ifttt.com"; // IFTTT server hostname
String PATH_NAME  = "/trigger/lightsensor/with/key/bv-rmgYhrL56VgM_haxZIX"; // IFTTT event path
String queryString = "?value1=57&value2=25"; // Query parameters for the IFTTT event

void setup() {

  Serial.begin(9600);
  Serial.print("START");
  while (!Serial);

  // Connect to the Wi-Fi network
  WiFi.begin(ssid, pass);
  Wire.begin();
  while (true) {
    if (client.connect(HOST_NAME, 80)) {
      // If connected to the server:
      Serial.println("Connected to server");
      break;
    } else {
      // If not connected, print an error message and retry
      Serial.println("Connection failed");
    }
    delay(500);
  }

  // Initialize the light sensor
  lightMeter.begin();
  Serial.println("Connected to server");
}

void loop() {
  Serial.print("START");
  float lux = lightMeter.readLightLevel(); // Read the light level from the sensor

  queryString += "?value1=";   // Build the query string
  queryString += lux;           // Add the light level to the query string
  Serial.println(queryString);

  if (lux > 500) { // Check if the light level is above a threshold

    // Make an HTTP request to IFTTT
    // Send HTTP header
    client.println("GET " + PATH_NAME + queryString + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println(); // End HTTP header

    while (client.connected()) {
      if (client.available()) {
        // Read an incoming byte from the server and print it to the serial monitor
        char c = client.read();
        Serial.print(c);
      }
    }

    // The server is disconnected, stop the client
    client.stop();
    Serial.println();
    Serial.println("Disconnected");
  }
  queryString = ""; // Clear the query string
  delay(3000);      // Delay before the next loop iteration
}
