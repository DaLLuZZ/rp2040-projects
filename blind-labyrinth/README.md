# Microcontrollertechnik Projekt

## Links zu den Videos

### Dropbox

https://www.dropbox.com/scl/fi/liq5wrpuyg2psgz6vqx3e/Video-29.02.24-19-17-23.mov?rlkey=9pe78m16jhxmjfddp5ppb6bog&dl=0

### Youtube

https://youtu.be/f3-u0IJb0Tc?si=lES5Ve8WRyNrCKM7

### Setup

Um die SD-Karte vorzubereiten muss ein Ordner mit dem Namen "maps" angelegt werden und die beiden Beispielmaps aus dem Ordner sd-card/maps direkt unter maps plaziert werden.


## Labyrinthspiel von Maurice Kranich und Louis Landeck

Das **Mikrocontrollertechnik Projekt** der Gruppe **Maurice Kranich und Louis  Landeck** behandelt ein **Spiel** in dem der Spieler ein **Labyrinth** durchqueren muss. Dabei werden die **Zeit**, sowie die Anzahl der **Schritte** gemessen und bei Vollendung des Levels / des Spiels wird diese angezeigt.

Das Spiel wird auf dem rp2040-Launchpad mithilfe verschiedenster Sensoren und Aktoren gespielt. Dabei werden ein **LCD-Display, ein Joystick, sowie Knöpfe und LEDs** genutzt.

## Spiel

### Das Labyrinth

Das Labyrinth ist **16x16** Felder groß und beinhaltet verschiedene Arten von Feldern:

* Weg *(Index 0)*
* Wand *(Index 1)*
* Start *(Index 2)*
* Ziel *(Index 3)*

Dabei besteht der Rand des Labyrinths aus Wänden, Start und Zielpunkt.

Die Labyrinthe bzw. Karten werden in **CSV-Dateien** gespeichert und sollen über den **SD-Kartenleser** auf dem rp2040 geladen werden.

Ein Feld des Labyrinths entspricht **8x8** Pixeln. Da das Labyrinth **16x16** Felder groß ist, entspricht dies **128x128** Pixeln und nimmt somit das gesamte **LCD-Display** des Launchpads ein.

### Starten des Spiels

Beim Starten des Spiels, wird der Spieler durch einen **Startbild** begrüßt. Durch das Drücken des unteren Knopfes gelangt man dann zum nächsten Bild.

 Dort kann der Spieler dann das **Labyrinth** auswählen. Falls keine Labyrinthe auf der SD-Karte liegen, oder die SD-Karte nicht gelesen werden kann, wird der Spieler darauf hingewiesen und das **Default-Labyrinth** verwendet. Das Auswählen wird durch das Betätigen des oberen Knopfes und das Bestätigen durch das Betätigen des unteren Knopfes ermöglicht.

 Danach kommt man zur Auswahl der **Schwierigkeit**. Dabei wird ebenfalls, wie zuvor, durch den oberen Knopf die Auswahl getroffen und mit dem unteren Bestätigt.

Sobald die Auswahl dieser Einstellungen bestätigt wurde, wird das Spiel bzw. Level gestartet.

### Spielablauf

Der Spieler startet auf dem **Startfeld** am Rand des Labyrinths. Dabei ist die Form des Labyrinths dem Spieler unbekannt und dieser sieht lediglich das **Start-** und **Zielfeld**, Abhängig von dem ausgewählten Schwierigkeitsgrad kann der Spieler seine **umliegenden Felder**, sowie eine gewisse Anzahl seiner **letzten Schritte** erkennen, während er sich durch das Labyrinth bewegt.

Das Bewegen wird durch das **Halten des unteren Knopfes** und gleichzeitigem Lenken mit dem **Joystick** ermöglicht.

### Spielende

Sobald der Spieler das **Ziel** erreicht, wird die Zeit gestoppt und die Schritte gezählt, sowie der **Score** berechnet. Diese Werte werden dem Spieler auf einem **Endbildschirm** angezeigt. Die Daten werden dann gegebenenfalls in die Bestenliste eingetragen und auf der **SD-Karte** mit der Map gespeichert. Der Spieler kann dann mithilfe des unteren Knopfes erneut zum **Startmenü** zurückkehren und ein neues Spiel beginnen.

## Klassenübersicht

### Tile

Die abstrakte Klasse **Tile** stellt ein Feld in dem Labyrinth da. Ein Teil ist **8x8** Pixel groß auf dem LCD Bildschirm. Es besitzt seine eigenen X- und Y-Koordinaten und die umgerechneten Pixelkoordinaten auf dem LCD Bildschirm. Zudem besitzt die Klasse Zugriff auf den **Visualizer**, sodass dieser das Tile abbilden kann.

Zudem besitzt jedes Tile die Methode **inVision** um anhand der Spielerkoordinaten zu überprüfen, ob das Tile in Sichtweite des Spielers ist oder nicht. Dazu wird zusätzlich das Attribut **vision_range** genutz, welche vorgibt, wie groß die maximale Sichtweite ist. Des weiteren besitzt jedes Tile die Eigenschaft **passable** welche aussagt, ob das Tile begehbar für den Spieler ist oder nicht.

Da das Tile eine abstrakte Klasse ist, gibt es verschiedene Unterklassen:

#### PathTile

Das PathTile ist eine dieser Unterklassen und stellt ein Weg-Feld des Labyrinths dar. Die Besonderheiten des PathTiles sind, dass es ein **begehbares Feld** für den Spieler bietet. Zudem werden die letzten besuchten PathTiles des Spieler farblich hervorgehoben, um die **letzten Schritte des Spieler** nachverfolgen zu können.
Grundsätzlich werden die Weg-Felder nur angezeigt, solange sie in Sichtweite sind, oder zum Tail des Spielers gehören.

#### WallTile

Das WallTile ist der gegenSpieler zum PathTile und stellt eine Wand des Labyrinths dar. Im Gegensatz zum PathTile ist die Wand **nicht begehbar** für den Spieler. Sie wurd zudem auch nur angezeigt, wenn sie im Sichtbereich des Spieler ist.

#### StartTile

Das StartTile ist das Feld, auf dem der Spieler **zu Beginn des Labyrinthsdurchlaufs** gesetzt wird. Dieses Feld ist begehbar und immer sichtbar.

#### FinishTile

Das FinishTile ist das Feld, zu dem der Spieler gehen muss. Das Feld ist immer sichtbar und beim **Erreichen des Feldes** gilt das Labyrinth als durchlaufen. Es ist wie das StartTile immer sichtbar und begehbar.

### Labyrinth

Die Klasse Labyrinth dient zur **Koordination und Darstellung des gesamten Labyrinths**. Es besitzt ein zweidimensionales Array an Tiles, welche das Labyrinth definieren.

### Player

Die Player-Klasse **repräsentiert den Spieler** und ermöglicht das Durchkreuzen des Labyrinths. Er besitzt Koordinaten, sowie die Möglichkeiten, **seine Position zu ändern** und **seine Schritte zu zählen**. Dazu nutzt er einen **Timer**, um seine benötigte Zeit zu messen.

### Visualizer

Der Visualizer ist die zuständige Klasse für das **Visualisieren jeglicher Objekte auf dem Display**. Er besitzt verschiedene Methoden zur Visualisierung des Startbildschirms, Endbildschirms und der Labyrinthen. Jegliche Visualisierung läuft über diese Klasse.

### SDReader und SDWriter

Diese Hilfsklassen dienen zur **Verwendung der SD-Karte vom Mikrocontroller**. Sie bieten die Möglichkeit Labyrinthe und deren Highscores auf der SD-Karte zu speicher und in das Spiel zu laden.

### Game Master

Der Game Master dient zum Zusammenfügen der anderen Klassen zum letztendlichen Spiel. Er teilt das Spiel in **verschiedene Phasen** auf, welche Schritt für Schritt abgearbeitet werden.

### Mode

Der Mode ist ein Struct, welches die Schwierigkeit des Spiels repräsentiert und mehrere Hilfsmethoden dafür anbietet. Es legt die **Sichtweite** und **Tail-Länge** des Spielers fest.

Dazu ist dort das Enum ModeName hinterlegt, welches die verschiedenen Schwierigkeitsstufen repräsentiert,

## Format der CSV-Dateien

Bei den CSV-Dateien der Labyrinthe gibt es ein besonderes Format. Der Name der Datei, ist auch der wiedergegebene Name im Spiel (UpperCase). Die ersten 16 Zeilen müssen dabei 16 durch "," getrennte Werte von 0 bis 3 sein, wobei 2 und 3 nur einmal vorkommen dürfen.
In der 17en Zeile (Index 16) steht der Highscore des Labyrinths. Danach muss in der letzten Zeile ein "#" sein. Dies ist Essenziell als Endzeichen der Datei.

## ACHTUNG

Das Projekt befindet sich in einem unterordner von YAHAL. Unser Projektpfad sieht folgendermaßen aus:
*"YAHAL/Projekt/MCT-Projekt-Labyrinth"*
