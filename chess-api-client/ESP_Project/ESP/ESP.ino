#include <Arduino.h>
#include <ArduinoJson.h>
// ESP32
/*
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
*/
// ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

#include "creds.h"

// Replace with your network credentials
const char* ssid = SSID;
const char* password = PASSWORD;
const char* token = TOKEN;

String msg;
String requestURL;
bool requestReady = false;
enum REQUEST_METHOD {
    GET,
    POST,
    NONE
};
REQUEST_METHOD requestMethod = NONE;

void setup() {
    Serial.begin(4800);

    Serial.println();
    Serial.println();
    Serial.println();

    //Connect to Wi-Fi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(1000);
    }
    Serial.println(String("Bearer ") + String(token)); // Please do tell why removing this breaks authentication????
}

String r_get(String url) {
    // ESP8266
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    // Ignore SSL certificate validation
    client->setInsecure();

    // ESP32
    //WiFiClientSecure client;
    //client.setInsecure();

    //create an HTTPClient instance
    HTTPClient https;
    //Initializing an HTTPS communication using the secure client
    //Serial.print("[HTTPS] begin...\n");
    // ESP8266
    if (https.begin(*client, url)) {  // HTTPS
        // ESP32
        // if (https.begin(client, url)) {  // HTTPS
        //Serial.print("[HTTPS] GET...\n");
        https.addHeader("Authorization", AUTH);
        // start connection and send HTTP header
        int httpCode = https.GET();
        // httpCode will be negative on error
        if (httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            //Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
            // file found at server
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
                String payload = https.getString();
                return payload;
            }
        } else {
            //Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
        }

        https.end();
    } else {
        //Serial.printf("[HTTPS] Unable to connect\n");
    }
    return "";
}

String r_post(String url, String post_body) {
    // ESP8266
    //std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    // Ignore SSL certificate validation
    //client->setInsecure();

    // ESP32
    WiFiClientSecure client;
    client.setInsecure();

    //create an HTTPClient instance
    HTTPClient https;
    //Initializing an HTTPS communication using the secure client
    //Serial.print("[HTTPS] begin...\n");
    // ESP8266
    // if (https.begin(*client, url)) {  // HTTPS
    // ESP32
    if (https.begin(client, url)) {  // HTTPS
        //Serial.print("[HTTPS] GET...\n");
        https.addHeader("Authorization", AUTH);
        // start connection and send HTTP header
        int httpCode = https.POST(post_body);
        // httpCode will be negative on error
        if (httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            //Serial.printf("[HTTPS] POST... code: %d\n", httpCode);
            // file found at server
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
                String payload = https.getString();
                return payload;
            }
        } else {
            //Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
        }

        https.end();
    } else {
        //Serial.printf("[HTTPS] Unable to connect\n");
    }
    return "";
}

void loop() {
    if (Serial.available()) {
        requestReady = false;
        char c = Serial.read();
        if (c == '\n') {
            if(msg.startsWith("GET:")) {
                requestMethod = GET;
                requestURL = msg.substring(4);
                //Serial.print("GET URL: ");
                //Serial.println(requestURL);
                requestReady = true;
            } else if(msg.startsWith("POST:")) {
                requestMethod = POST;
                requestURL = msg.substring(5);
                //Serial.print("POST URL: ");
                //Serial.println(requestURL);
                requestReady = true;
            } else {
                //Serial.print("Invalid Method+URL: ");
                //Serial.println(msg);
            }
            msg.clear();
        } else {
            msg += c;
        }
    }
    // wait for Wi-Fi connection
    if (requestReady && (WiFi.status() == WL_CONNECTED)) {
        requestReady = false;
        // Initializing an HTTPS communication using the secure client
        switch (requestMethod) {
            case GET: {
                String payload = r_get(requestURL);
                Serial.println(payload);
                break;
            }
            case POST: {
                String payload = r_post(requestURL, "");
                Serial.println(payload);
                break;
            }
            default:
                break;
        }
        // String payload = r_get("https://lichess.org/api/board/game/stream/0vrcMRN9");
        // Serial.println(r_post("https://lichess.org/api/board/game/0vrcMRN9/move/d1h5", ""));
        requestMethod = NONE;
    }
}
