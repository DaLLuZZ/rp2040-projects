#undef __STRICT_ANSI__

#include "Arduino.h"

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>
#include "NTPClient.h"


const char *ssid = "SSID;
const char *password = "Password";
String serverAdress = "http://127.0.0.1:3000";
String serverName = serverAdress + "/zeiten";
String serverNameGame = serverAdress + "/zeiten/spiel";

const int led = 4;
NTPClient ntpClient;
String msg;

void blinksuccess() {
    digitalWrite(led, 1);
    delay(500);
    digitalWrite(led, 0);
}

void httpPost(String &pServerName, String &httpRequestData, String &type, bool isID) {
    if (WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        HTTPClient http;

        // Start der Sitzung
        http.begin(client, pServerName);

        // Content-Type Header
        http.addHeader("Content-Type", "application/json");

        // HTTP POST Request senden
        int httpResponseCode = http.POST(httpRequestData);

        while (httpResponseCode < 0) { //versuchen bis erfolgreich
            // HTTP POST Request senden
            httpResponseCode = http.POST(httpRequestData);
            delay(100);
        }
        if (httpResponseCode > 0) { // Wenn Anfrage erfolgreich war
            if (isID) {
                String payload = http.getString();
                JSONVar objectId = JSON.parse(payload);
                double id = objectId[0]["ID"];

                JSONVar sendObjectID;
                sendObjectID["Anweisung"] = "GameID";
                sendObjectID["ID"] = String(id);
                Serial.println(JSON.stringify(sendObjectID));
            }
            blinksuccess();
        }

        // Ressourcen freigeben
        http.end();
    }
}

void setup(void) {
    delay(3000);//warten -> Debuginfo wird nicht gesendet
    pinMode(led, OUTPUT);
    digitalWrite(led, 0);
    // Initialisierung des serial port
    Serial.begin(115200);

    //Verbinden mit Router
    WiFi.begin(ssid, password);

    // Warten auf Verbindung
    while (WiFi.status() != WL_CONNECTED) {
        digitalWrite(led, 1);
        delay(300);
        digitalWrite(led, 0);
        delay(300);
    }
    JSONVar sendObjectConnected;
    sendObjectConnected["Anweisung"] = "Verbunden";
    Serial.println(JSON.stringify(sendObjectConnected));
}

void loop(void) {
    // prüfen ob char verfügbar ist
    if (Serial.available()) {
        //char holen
        char c = Serial.read();
        // Ende der Zeile erreicht?
        if (c == '\n') {
            JSONVar object = JSON.parse(msg);

            JSONVar operation = object["Anweisung"];
            String convOperation = operation; //Cast zur Weiterverarbeitung
            if (convOperation == "Hochladen") {
                JSONVar action = object["Aktion"];
                JSONVar time = object["Zeit"];
                JSONVar gameID = object["game_ID"];

                // Zu sendende Daten
                String convAction = action; //Cast zur Weiterverarbeitung
                double convTime = time;
                double convGameID = gameID;

                JSONVar httpRequestData;
                httpRequestData["modus"] = convAction;
                httpRequestData["zeit"] = convTime;
                httpRequestData["game_ID"] = convGameID;

                String data = JSON.stringify(httpRequestData);
                String type = "Hochladen";
                httpPost(serverName, data, type, false);
            } else if (convOperation == "StartGame") {
                String starttime = ntpClient.getTime();
                JSONVar httpRequestData;
                httpRequestData["starttime"] = starttime;

                String data = JSON.stringify(httpRequestData);
                String type = "StartGame";
                httpPost(serverNameGame, data, type, true);
            }
            // Zurücksetzen des message buffer
            msg.clear();
        } else {
            // char zur Message hinzufügen
            msg += c;
        }
    }
}

