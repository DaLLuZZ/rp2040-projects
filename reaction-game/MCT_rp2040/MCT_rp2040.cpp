#include "gpio_rp2040.h"
#include "task.h"
#include "uart_rp2040.h"
#include "adc_rp2040.h"
#include "Display.h"
#include "Stoppuhr.h"
#include "PlaySound.h"
#include "yahal_String.h"
#include <cassert>
#include "tiny-json.h"


#define SWITCH1           16 // oberer S1 Button
#define SWITCH2           17 // unterer S2 Button
#define JOYSTICKBUTTON    19 // Joystick Button

Display display;
// Mit ESP8266 verbundenes UART
uart_rp2040 uart_esp(1, 20, 21, 115200);
int gameID = 0;
gpio_rp2040_pin s1(SWITCH1); // oberer S1 Button
gpio_rp2040_pin s2(SWITCH2); // unterer S2 Button
gpio_rp2040_pin joystickBtn(JOYSTICKBUTTON); // Joystick Button
adc_rp2040_channel joy_X(1); // Joystick in x-Richtung
adc_rp2040_channel joy_Y(3); // Joystick in y-Richtung
uint16_t offsetX = 0;
uint16_t offsetY = 0;

bool debounce(gpio_rp2040_pin switch1) {
    if (!switch1.gpioRead()) {
        task::sleep(100);
        if (switch1.gpioRead()) {
            task::sleep(100);
            return true;
        }
    }
    return false;
}

void startGame() {
    display.showText("Starten");
    uart_esp.puts("{\"Anweisung\":\"StartGame\"}\n");
}

void endGame() {
    display.showText("Restart");
    while (!debounce(s1)); //warten bis S1 gedrückt
    startGame();
}

String getAction(int number) {
    switch (number) {
        case 1:
            return "S1";
        case 2:
            return "S2";
        case 3:
            return "joystick";
        case 4:
            return "up";
        case 5:
            return "down";
        case 6:
            return "left";
        case 7:
            return "right";
    }
    return String();
}

void uploadAction(int time, const String &action) { //Sendet JSON an Esp8266 zum Hochladen der Aktion auf dem Server
    String str = "{\"Anweisung\":\"Hochladen\", \"Aktion\":\"";
    str += action;
    str += "\", \"Zeit\":";
    str += to_String(time);
    str += ", \"game_ID\":";
    str += to_String(gameID, 10);
    str += "}\n";
    uart_esp.puts(str.c_str());
}

bool checkAction(const String &action) { // Misst die Zeit und prüft, ob richtige Aktion rechtzeitig durchgeführt wurde
    PlaySound ps;
    ENTRY click[] = {{e2, 2},
                     {P,  0}};
    ENTRY wrong[] = {{e1, 8},
                     {d1, 8},
                     {c1, 16},
                     {P,  0}};
    Stoppuhr stoppuhr(1000, 10000, 0);
    /*ENTRY timeMelody[] = {{h2, 2}, //Hier war der Plan, 3 Sekunden vor Schluss noch sekündlich zu Piepen. Wenn das eingebaut war ist allerdings das Programm abgestürzt
                          {P,  0}};
    timer_rp2040 timer(2); //Timer zum Prüfen, ob Zeit fast abgelaufen
    timer.setPeriod(1000000, TIMER::PERIODIC);
    timer.setCallback([&]() {
        if (10000 - stoppuhr.getTime() <= 3000) {
            ps.playMelody(timeMelody, 60);
        }
    });*/
    stoppuhr.start();
    //timer.start();
    String actionDone = "";
    while (stoppuhr.isRunning()) { //läuft die Zeit ab gibt isRunning() false zurück und das Spiel wird beendet
        uint16_t valX = joy_X.adcReadRaw();
        uint16_t valY = joy_Y.adcReadRaw();
        // Joystick nach links bewegt
        if ((valX - offsetX) < -1000) {
            actionDone = "left";
            break;
        }
        // Joystick nach rechts bewegt
        if ((valX - offsetX) > 1000) {
            actionDone = "right";
            break;
        }
        // Joystick nach unten bewegt
        if ((valY - offsetY) < -1000) {
            actionDone = "down";
            break;
        }
        // Joystick nach oben bewegt
        if ((valY - offsetY) > 1000) {
            actionDone = "up";
            break;
        }
        // S1 gedrückt
        if (debounce(s1)) {
            actionDone = "S1";
            break;
        }
        // S2 gedrückt
        if (debounce(s2)) {
            actionDone = "S2";
            break;
        }
        // Joystick gedrückt
        if (debounce(joystickBtn)) {
            actionDone = "joystick";
            break;
        }
    }
    if (!stoppuhr.isRunning()) { //wenn Zeit abgelaufen
        actionDone = "time";
    }
    stoppuhr.stop();
    //timer.stop();
    int time = stoppuhr.getTime();
    if (actionDone == action) { //wenn richtige Aktion durchgeführt
        uploadAction(time, action);
        ps.playMelody(click, 60);
        return true;
    } else { //wenn falsche Aktion durchgeführt, oder Zeit abgelaufen
        if (actionDone == "time") {
            display.showText("Zeit");
        } else {
            display.showText("falsch");
        }
        ps.playMelody(wrong, 60);
        return false;
    }
}

void startAction() { //Startet zufällige Aktion und zeigt diese an
    // ADC Wert des Mikrofons lesen
    uint32_t val = adc_rp2040::inst.adcReadRaw(6) << 16;
    // ADC Werte des Joysticks addieren für größere Variation des Wertes
    val += joy_X.adcReadRaw();
    val += joy_Y.adcReadRaw();
    std::srand(val);
    int actionNumber = std::rand() % 7 + 1;
    String action = getAction(actionNumber);
    display.showAction(action);
    if (checkAction(action)) { //wenn richtige Aktion rechtzeitig durchgeführt
        startAction();
    } else { //wenn falsche oder keine Aktion durchgeführt
        endGame();
    }
}

int main() {
    //Initialisierung der Buttons und Offsets
    s1.gpioMode(GPIO::INPUT);
    s2.gpioMode(GPIO::INPUT);
    joystickBtn.gpioMode(GPIO::INPUT);
    offsetX = joy_X.adcReadRaw();
    offsetY = joy_Y.adcReadRaw();
    display.showText("Startbildschirm");

    while (!debounce(s1)); //warten bis S1 gedrückt

    display.showText("Verbinde");
    gpio_rp2040_pin gpio3(3);
    gpio3.gpioMode(GPIO::OUTPUT | GPIO::INIT_LOW);

    // Reset des ESP8266
    gpio_rp2040_pin esp_reset(6);
    esp_reset.gpioMode(GPIO::OUTPUT | GPIO::INIT_LOW);
    task::sleep(200);
    esp_reset.gpioWrite(HIGH);

    task::sleep(2200);//warten, um keine Dubuginfo zu empfangen


    // Message Handler:
    String msg;
    msg.reserve(150);//Buffer allokieren für weniger Zeitaufwand durch dynamische Speicherverwaltung
    bool msgAvailable = false;
    uart_esp.uartAttachIrq([&](char c) {
        if (c == '\n') {
            assert(msgAvailable == false); //Vorherige Eingabe sollte bereits verarbeitet sein
            msgAvailable = true;
        } else {
            msg += c; // Char zur Message hinzufügen
        }
    });

    while (true) {
        if (msgAvailable) { //wenn Nachricht verfügbar diese auswerten
            String msg1 = msg; //Kopie für schnellere Verarbeitung
            msg.clear();
            msgAvailable = false;

            json_t mem[5];
            char *input = const_cast<char *>(msg1.c_str());
            json_t const *json = json_create(input, mem, 5);
            json_t const *anweisungField = json_getProperty(json, "Anweisung");
            String operation = json_getValue(anweisungField);

            //Entsprechende Aktion durchführen
            if (operation == "Verbunden") {
                startGame(); //Spiel starten
            } else if (operation == "GameID") {
                json_t const *idfield = json_getProperty(json, "ID");
                String id = String(json_getValue(idfield));
                gameID = atoi(id.c_str());
                startAction();
            }
        }
    }
}