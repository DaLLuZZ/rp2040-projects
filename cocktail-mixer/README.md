# Cocktailmixer
Das Video zum Projekt liegt [hier](https://fhaachen-my.sharepoint.com/:v:/g/personal/js8887s_ad_fh-aachen_de/EamDBifn-WFDpxlZjA2XymABcHoGrmEQKDNPN-1u2kg-rQ?e=rquoVZ)

Die Dokumentation ist an notwendigen Stellen direkt im Code enthalten.

Im Folgenden wird die Ordnerstruktur beschrieben: <br/>
Der Data-Ordner wird auf den ESP32 übertragen und dieser kann mit dem Filesystem SPIFFS darauf zugreifen. <br/>
Der Ordner mixer_sketch ist notwendig, damit man das Projekt mit der ArduinoIDE kompilieren und übertragen kann.
```
<Project Directory>
│   README.md
│
└───mixer_sketch
    │   mixer_sketch.ino
    │
    └───data
        │   index.html
        │
        ├───css
        │       bootstrap.css
        │       stylingPointer.css
        │
        ├───js
        │       bootstrap_javascript.js
        │       multislider.js
        │       process.js
        │       stylingSlider.js
        │       tooltip.js
        │
        └───pictures
                Asbach.png
                Bacardi.png
                CaptainMorgan.png
                Cola.png
                Fanta.png
                favicon.png
                Flimm.png
                Havana.png
                Korn.png
                Sprite.png
                Wodka.png
```
