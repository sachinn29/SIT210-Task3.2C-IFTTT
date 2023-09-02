#include <SPI.h>
#include <WiFiNINA.h>

// Replace with your Wi-Fi credentials
char ssid[] = "sachin";
char pass[] = "sachinarora";

// Replace with your IFTTT webhook key and event names
const char* iftttWebhookKey = "bQ_Rj9jnVy1dwyegwUKM58pbcjILuvKaHwoEgPcyrNY";
const char* sunlightStartEvent = "sunlight_start";
const char* sunlightStopEvent = "sunlight_gone";

const int ldrPin = A0;
bool isSunlightDetected = false;

WiFiClient client;

void setup() {
  Serial.begin(9600);

  // Connect to Wi-Fi
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  int ldrValue = analogRead(ldrPin);

  if (!isSunlightDetected && ldrValue < 500) {
    sendIFTTTEvent(sunlightStartEvent);
    isSunlightDetected = true;
  } else if (isSunlightDetected && ldrValue > 500) {
    sendIFTTTEvent(sunlightStopEvent);
    isSunlightDetected = false;
  }

  delay(1000);  // Delay between readings
}

void sendIFTTTEvent(const char* eventName) {
  if (client.connect("maker.ifttt.com", 80)) {
    Serial.println("Connected to IFTTT");

    String url = "/trigger/";
    url += eventName;
    url += "/with/key/";
    url += iftttWebhookKey;

    client.println("POST " + url + " HTTP/1.1");
    client.println("Host: maker.ifttt.com");
    client.println("Connection: close\r\n\r\n");
    delay(10);

    while (client.available()) {
      char c = client.read();
      Serial.print(c);
    }
    Serial.println();
    client.stop();
    Serial.println("IFTTT event sent");
  } else {
    Serial.println("Connection to IFTTT failed");
  }
}
