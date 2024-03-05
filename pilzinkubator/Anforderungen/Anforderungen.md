### Anforderungen

1. Anschließen der Sensoren und Aktoren über Stecker (Austauschbarkeit des Board)
2. Auslesen der Luftfeuchte und Temperatur in regelmäßigen Abständen
3. Auslesen des Feuchtefühlers in regelmäßigen Abständen
5. Regelung der Luftfeuchte über die Lüftersteuerung (Min-Max-Hysterese)
6. Regelmäßiges Austauschen der Luft mittels Lüftersteuerung (z.B. jeden Tag 1 Minute lüften)
7. Bodenbefeuchtung mittels Wasserpumpe und Wassertank über die Sprühdüsen im Deckel (Min-Max-Hysterese)
8. Logging aller Mess-, Steuer und Regelwerte auf eine SD-Karte
9. Bereitstellen der Mess-, Steuer- und Regelwerte über REST-Schnittstelle
10. Speichern der Steuerwerte auf der SD-Karte zur persistenz.


### Optional
- Konfiguration der REST-Schnittstelle bzw. WLAN-Daten mittels config-file auf der SD-Karte
- Konfiguration der Regelparameter mittels config-file auf der SD-Karte
- 