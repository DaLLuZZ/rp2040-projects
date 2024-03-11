# Beschreibung
Reaktionsspiel, welches die Reaktionszeit misst und einem die Möglichkeit gibt, diese im Browser auszuwerten

# Problem
Leider konnte ich eine Warnung durch ein akustisches Signal nicht einbauen, wenn die Zeit fast abgelaufen ist, da an der Stelle leider immer das Programm terminiert ist, wenn ich das eingebaut habe. Entsprechend ist dies auskommentiert in der Datei [MCT_rp2040.cpp](.\MCT_rp2040\MCT_rp2040.cpp)

# Projektstruktur
## MCT_rp2040
Teil für den RP2040
- MCT_rp2040.cpp: Enthält die int main()
- Display.cpp: Methoden zur Anzeige auf dem Display
- PlaySound.cpp: Methoden zum Spielen von Sound
- Stoppuhr.cpp: Methoden zum Messen der Zeit mittels eines Timers
- tiny-json.c [Bibliothek für Verarbeitung von JSON](https://github.com/rafagafe/tiny-json)

## MCT_esp8266
Teil für den WifiTick
- MCT_esp8266.cpp: Verarbeiten der Anfragen vom RP2040 und Senden an Rest API
- NTPClient.cpp: Methoden zum Ermitteln der Zeit - Orientiert an Implementation in [makerpro-esp8266-ntp](https://github.com/dalmatianrex/articles/blob/master/makerpro-esp8266-ntp/ESP8266_Time.ino)

## MCT Web
Weboberfläsche
- db/MCT.db: Datenbank
- modules/database.js: Kommunikation mit Datenbank
- public/javascripts/auswertungen.js: DOM Baum Manipulation für Ausgabe des Diagramms und Dropdown
- public/index.html: Darstellung der Website
- routes/auswertungen.js: Definition der ROuten der Rest API
- app.js: Einstiegspunkt

# zusätzlich installierte Bibliotheken
in der Arduino-IDE unter Sketch - Bibliothek einbinden - Bibliotheken verwalten - Arduino_JSON installiert [Arduino_JSON](https://github.com/arduino-libraries/Arduino_JSON)

# Nötige Anpasssungen zum Testen des Programms auf dem WifiTick
- in [MCT_esp8266.cpp](./MCT_esp8266/MCT_esp8266.cpp) in Z.12-14 die SSID, und das Passwort des WLAN-Routers und die Adresse des Servers eingeben


# Befehle zum Starten des Webservers
- npm install
- node app.js

# Datenbankschema
Spiele(ID, Startzeit) \
Zeiten(ID, Modus, Zeit, Spiel_ID)

# weitere Dokumentationen
- [Protokoll](./weitere%20Dokumentation/Protokoll.pdf)
- [API](./weitere%20Dokumentation/API.pdf)
