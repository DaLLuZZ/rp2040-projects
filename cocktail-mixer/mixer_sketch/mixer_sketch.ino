#include <SPIFFS.h>
#include <Arduino.h>
#include <Ticker.h>

// zu impotierende Libraries:
#include <WiFi.h>           // https://github.com/espressif/arduino-esp32.git
#include <WiFiClient.h>     // siehe Wifi.h
#include <ESPAsyncWebSrv.h> // https://github.com/me-no-dev/ESPAsyncWebServer.git
#include "AsyncJson.h"      // siehe ESPAsyncWebserver
#include "ArduinoJson.h"    // https://github.com/bblanchon/ArduinoJson.git

#define PUMPE_ON LOW
#define PUMPE_OFF HIGH
#define SSID "Cocktailmixer"
#define PASSWORD "MischeYNJS"

const double fuellzeitFaktor = 0.03;
bool isFilling = false;
bool isFinished = true;
JsonDocument drinksJson;
AsyncWebServer server(80);

/**
   Aufgrund der Anordnung der Pins auf dem ESP32 müssen diese noch gemappt werden
*/
int getPin(int drink) {
    if (drink <= 2) {
        return drink - 1;
    } else {
        return drink + 1;
    }
}

/**
 * Schaltet die Pumpe an/aus
 */
void schaltePumpe(int pin) {
    digitalWrite(pin, !digitalRead(pin));
}

/**
 * Callback-Methode für den Ticker
 */
void schaltePumpeCallback(int pin) {
        digitalWrite(pin, PUMPE_OFF);
}

/**
   Extrahiert das Json mit den Drinks und steuert die Pumpem an
*/
void fillDrink() {
    JsonArray getraenke = drinksJson["getraenk"].as<JsonArray>();
    Ticker *ticker = new Ticker[getraenke.size()];
    for (int i = 0; i < getraenke.size(); i++) {
        JsonObject getraenk = getraenke[i];
        schaltePumpe(getPin(getraenk["drink"].as<int>()));
        ticker[i].once((getPin(getraenk["anteil"].as<double>()) * fuellzeitFaktor), schaltePumpeCallback,
                         getPin(getraenk["drink"].as<int>()));
        }
    // überprüft, welche Pumpen noch laufen und beendet ggf. den Prozess
    while (isFilling) {
        isFinished = true;
        for (int i = 0; i < getraenke.size(); i++) {
            if (ticker[i].active()) {
                isFinished = false;
            }
        }
        if (isFinished) {
            isFilling = false;
        }
        delay(100);
    }
    delete[] ticker;
}


void setup() {
    // setzt alle verwendeten Pins als Outputpin
    for (int i = 1; i <= 6; i++) {
        pinMode(getPin(i), OUTPUT);
        digitalWrite(getPin(i), PUMPE_OFF);
    }

    Serial.begin(115200);
    delay(10);
    //start filesystem
    SPIFFS.begin();
    // Request für alle Ressourcen, damit sie geladen werden können
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", "text/html", false);
    });
    server.on("/Cola.png", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/pictures/Cola.png", "image/png", false);
    });
    server.on("/Fanta.png", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/pictures/Fanta.png", "image/png", false);
    });
    server.on("/Sprite.png", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/pictures/Sprite.png", "image/png", false);
    });
    server.on("/CaptainMorgan.png", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/pictures/CaptainMorgan.png", "image/png", false);
    });
    server.on("/Havana.png", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/pictures/Havana.png", "image/png", false);
    });
    server.on("/Korn.png", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/pictures/Korn.png", "image/png", false);
    });
    server.on("/Asbach.png", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/pictures/Asbach.png", "image/png", false);
    });
    server.on("/Flimm.png", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/pictures/Flimm.png", "image/png", false);
    });
    server.on("/Wodka.png", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/pictures/Wodka.png", "image/png", false);
    });
    server.on("/Bacardi.png", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/pictures/Bacardi.png", "image/png", false);
    });
    server.on("/favicon.png", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/pictures/favicon.png", "image/png", false);
    });
    server.on("/bootstrap.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/css/bootstrap.css", "text/css", false);
    });
    server.on("/stylingPointer.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/css/stylingPointer.css", "text/css", false);
    });
    server.on("/multislider.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/js/multislider.js", "text/javascript", false);
    });
    server.on("/process.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/js/process.js", "text/javascript", false);
    });
    server.on("/stylingSlider.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/js/stylingSlider.js", "text/javascript", false);
    });
    server.on("/tooltip.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/js/tooltip.js", "text/javascript", false);
    });
    server.on("/bootstrap_javascript.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/js/bootstrap_javascript.js", "text/javascript", false);
    });

    // Handler für Request zum Füllen der Getränke
    AsyncCallbackJsonWebHandler *handler = new AsyncCallbackJsonWebHandler("/mixtur", [](AsyncWebServerRequest *request,
                                                                                         JsonVariant &json) {
        if (isFilling) {
            request->send(423);
        } else {
            drinksJson = json;
            // isFilling dient als Trigger für die fillDrink()-Methode in loop
            // Trigger nötig, weil AsyncHandler nicht so lange warten darf
            isFilling = true;
            request->send(200);
        }
    });
    server.addHandler(handler);

    // initialisiert und startet WebServer
    WiFi.mode(WIFI_AP);
    WiFi.softAP(SSID, PASSWORD, 8, 0, 1);
    IPAddress myIP = WiFi.softAPIP();
    server.begin();
}

void loop() {
    if (isFilling) {
        // führt Funktion zum Befüllen aus, die auch Trigger auf false setzt
        fillDrink();
    }
}
