#include <ESP8266WiFi.h>
#include "config.h"
#include "Communication.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"


// Set up the MQTT client class by passing in the Wi-Fi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Set up feeds for Adafruit.io: Notice MQTT paths for AIO follow the form: <username>/feeds/<feed name>
// Set up a feed called 'brightness' for publishing the brightness measurement values.
Adafruit_MQTT_Publish brightness = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/room.brightness");

// Set up a feed called 'temperature' for publishing the temperature measurement values.
Adafruit_MQTT_Publish temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/room.temperature");

void setup() {
    Serial.begin(115200);
    delay(10);

    // Connect to Wi-Fi access point.
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(WLAN_SSID);

    WiFi.begin(WLAN_SSID, WLAN_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();

    Serial.print("Connected to ");
    Serial.println(WLAN_SSID);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP().toString());
}

void loop() {
    // Ensure the connection to the MQTT server is alive (this will make the first connection and automatically reconnect when disconnected).
    MQTT_connect();

    String msg;
    // Check if message with new measurement value is available
    if (Serial.available()) {
        String measurementType = Serial.readStringUntil(' ');
        msg += measurementType;
        msg += ' ';

        float measurementValue = Serial.parseFloat();
        msg += measurementValue;

        String measurementUnit = Serial.readStringUntil('\n');
        msg += measurementUnit;

        if (measurementType.startsWith("Temperature:")) {
            // Publish temperature and send the message back
            Serial.println(msg);
            PublishValue(measurementValue, temperature);
        } else if (measurementType.startsWith("Brightness:")) {
            // Publish brightness and send the message back
            Serial.println(msg);
            PublishValue(measurementValue, brightness);
        } else {
            Serial.print("Something went wrong, ");
            Serial.print(measurementType);
            Serial.println(" couldn't be assigned to a suitable measurement type.");
        }
    }
}

void MQTT_connect() {
    int8_t ret;

    // Stop if already connected.
    if (mqtt.connected()) {
        return;
    }

    Serial.print("Connecting to MQTT... ");

    uint8_t retries = 3;
    while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
        Serial.println(mqtt.connectErrorString(ret));
        Serial.println("Retrying MQTT connection in 5 seconds...");
        mqtt.disconnect();
        delay(5000);  // wait 5 seconds
        retries--;
        if (retries == 0) {
            // basically die and wait for WDT to reset me
            while (true);
        }
    }
    Serial.println("MQTT Connected!");
}

void PublishValue(double value, Adafruit_MQTT_Publish &publisher) {
    Serial.print(F("Sending value "));
    Serial.print(value);
    Serial.print(" ... ");
    if (!publisher.publish(value)) {
        Serial.println(F("Failed"));
    } else {
        Serial.println(F("OK!"));
    }
}