#undef __STRICT_ANSI__
#include "Arduino.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>

const char *ssid = "*******";
const char *password = "************";


ESP8266WebServer server(80);

int counter = 0;

String antwort = "ok T:97.64 /200.00 B:18.16 /0.00 @:127 B@:0";		//Beispiel einer Antwort vom 3D Drucker wenn nach der Temperatur gefragt wird
String extrudertemp = "";
String buildplatetemp = "";

//FS* fileSystem = &SPIFFS;

File uploadFile;

String thumbsup(){       //ein HTML Text der ausgegeben wird, wenn eine Datei oder ein Text submited wird
	String out = "";
	out += "<html>";
    out +=    "<body>";
	out +=    "<pre>";
	out +=        "           ||||                    \n";
    out +=        "          |||||                    \n";
	out +=        "         ||| ||                    \n";
	out +=        "         ||   |                    \n";
	out +=        "         ||   ||                   \n";
	out +=        "          ||   |||                 \n";
	out +=        "           ||    ||                \n";
	out +=        "           ||     |||              \n";
	out +=        "            ||      ||             \n";
	out +=        "       |||||||       ||            \n";
	out +=        "    |||||              ||| ||      \n";
	out +=        "   ||     ||||          ||||||     \n";
	out +=        "   ||  ||||  |||             ||    \n";
	out +=        "   ||        |||             ||    \n";
	out +=        "    |||||||||| |||           ||    \n";
	out +=        "    ||        ||||           ||    \n";
	out +=        "    |||||||||||  ||          ||    \n";
	out +=        "    |||||              ||| ||      \n";
	out +=        "      ||       ||||     ||||||     \n";
	out +=        "      |||||||||| ||    ||| ||      \n";
	out +=        "         ||     |||| |||           \n";
	out +=        "         |||||||||||||             \n";
	out +=        "                                   \n";
	out +=    "</pre>";
    out +=    "</body>";
    out += "</html>";
	return out;
}

String generateHTML(String value, String newvalue){         //Die Controll HTML mit mehreren Buttons und einer Temperatur anzeige
	String out = "<html><head><meta http-equiv=\"refresh\" content=\"3\">";
	out += "<script>";
	out += "function sendString(str) {";             //Die Funktion bekommt bei einem Button Klick ein String übergeben 
	out += "var xhttp = new XMLHttpRequest();";      //und d/durch kann man diesen String dann an den 3D Drucker übergeben
	out += "xhttp.onreadystatechange = function(){";
	out += "if(this.readyState == 4 && this.status == 200){";
	out += "document.getElementById(\"response\").innerHTML == this.responseText;";
	out += "}};";
	out += "xhttp.open(\"GET\", \"/sendString?value=\" + str, true);";
	out += "xhttp.send();}";
	out += "</script>";
	out += "</head><body>";
	out += "<style>";
	out += ".heizbutton{top:11%; left:10%; width:120px; height:40px; position: absolute;}";
	out += ".cancelbutton{top:20%; left:10%; width:120px; height:40px; position: absolute;}";
	out += ".heizbettbutton{top:11%; left:20%; width:100px; height:40px; position: absolute;}";
	out += ".cancelbettbutton{top:20%; left:20%; width:100px; height:40px; position: absolute;}";
	out += ".up{top:30%; left:15%; width:40px; height:40px; position: absolute;}";
	out += ".down{top:50%; left:15%; width:40px; height:40px; position: absolute;}";
	out += ".left{top:40%; left:8%; width:40px; height:40px; position: absolute;}";
	out += ".right{top:40%; left:22%; width:40px; height:40px; position: absolute;}";
	out += ".home{top:40%; left:13%; width:100px; height:40px; position: absolute;}";
	out += ".zup{top:35%; left:30%; width:50px; height:40px; position: absolute;}";
	out += ".zdown{top:45%; left:30%; width:50px; height:40px; position: absolute;}";
	out += ".calibrate{top:40%; left:35%; width:100px; height:40px; position: absolute;}";
	out += "</style>";
	out += "<h1>Extruder: " + value + "C Buildplate: " + newvalue + "C</h1>";    // Die Temperatur wird hier mit weiterem Text angezeigt
	out += "<input type=\"button\" class=\"heizbutton\" id=\"heizen\" onclick=\" sendString('M104 S200 T0')\" value=\"heizen extruder\">";
	out += "<input type=\"button\" class=\"cancelbutton\" id=\"cancel\" onclick=\" sendString('M104 S0 T0')\" value=\"cancel extruder\">";
	out += "<input type=\"button\" class=\"heizbettbutton\" id=\"heizenbett\" onclick=\" sendString('M140 S60')\" value=\"heizen bett\">";
	out += "<input type=\"button\" class=\"cancelbettbutton\" id=\"cancelbett\" onclick=\" sendString('M140 S0')\" value=\"cancel bett\">";
	out += "<input type=\"button\" class=\"up\" id=\"up\" onclick=\"sendString('G0 X0.0 Y-10.0 Z0.0 F3000')\" value=\"y-\">";
	out += "<input type=\"button\" class=\"down\" id=\"down\" onclick=\"sendString('G0 X0.0 Y10.0 Z0.0 F3000')\" value=\"y+\">";
	out += "<input type=\"button\" class=\"left\" id=\"left\" onclick=\"sendString('G0 X10.0 Y0.0 Z0.0 F3000')\" value=\"x+\">";
	out += "<input type=\"button\" class=\"right\" id=\"right\" onclick=\"sendString('G0 X-10.0 Y0.0 Z0.0 F3000')\" value=\"x-\">";
	out += "<input type=\"button\" class=\"home\" id=\"home\" onclick=\"sendString('G28')\" value=\"Home\">";
	out += "<input type=\"button\" class=\"zup\" id=\"zup\" onclick=\"sendString('G0 X0.0 Y0.0 Z10.0 F3000')\" value=\"Up\">";
	out += "<input type=\"button\" class=\"zdown\" id=\"zdown\" onclick=\"sendString('G0 X0.0 Y0.0 Z-10.0 F3000')\" value=\"Down\">";
	out += "<input type=\"button\" class=\"calibrate\" id=\"calibrate\" onclick=\"sendString('G29')\" value=\"calibrate\">";
	out += "</body></html>";
	return out; 
}

int getposfromString(String fullstring, char part){   // eine einfache Funktion, die nur für die Temperatur Antwort ein gutes Ergebnis wiedergibt 
	int pos = 0;
	for(int i = 0; i < fullstring.length(); i++){
		if(fullstring.charAt(i) == part){
			return i;
		}
	}
	return pos;
}

int getposfromString(String fullstring, char part, int start){   // die gleiche Funktion, nur ein anderer Startpunkt
	int pos = 0;
	for(int i = start; i < fullstring.length(); i++){
		if(fullstring.charAt(i) == part){
			return i;
		}
	}
	return pos;
}

void handletext(){
	
	String out = "";           //HTML für Textübergabe
	out += "<html>";
    out +=    "<body>";
    out +=        "<form action=\"/textupload\" action=\"get\">";
	out +=			"<input type=\"text\" name=\"gcode\">send gcode to printer.";
	out +=			"<input type=\"submit\" name=\"submitInfo\" value=\"submit\">";
	out +=		"</form>";
    out +=    "</body>";
    out += "</html>";
	server.send(200, "text/html", out);
}

void handlefile(){
	
	String out = "";        //HTML für Fileimport
	out += "<html>";
    out +=    "<body>";
    out +=        "<form action=\"/fileupload\" action=\"get\">";
	out +=			"<input type=\"file\" id=\"gcode\" name=\"gcode\">send gcode to printer.";
	out +=			"<input type=\"submit\" name=\"submitInfo\" value=\"submit\">";
	out +=		"</form>";
    out +=    "</body>";
    out += "</html>";
	server.send(200, "text/html", out);
}

void handletextupload(){
	
	server.send(200, "text/html", thumbsup());         //Wenn der Text submitted wird, wird der Befehl über uart gesendet
	
	String gcode = server.arg(0);
    Serial.print(gcode + "\n");
}



void handlefileupload(){
	
	server.send(200, "text/html", thumbsup());          //Hierbei wird ein File uploaded, aber ich habe diesen nicht verarbeited
	HTTPUpload& upload = server.upload();                      //Dieses File müsste ausgelesen werden und Stück für Stück an den Drucker gesendet werden.
	if(upload.status == UPLOAD_FILE_START){
		String filename = upload.filename;
		if(filename.startsWith("/")){ filename = "/" + filename;}
		Serial.println("handleFileUpload Name: " + filename);
		uploadFile = SPIFFS.open("/" + filename, "w");
	}
	else if(upload.status == UPLOAD_FILE_WRITE){
		if(uploadFile){
			size_t bytesWritten = uploadFile.write(upload.buf, upload.currentSize);
		}
	}
	else if(upload.status == UPLOAD_FILE_END){
		if(uploadFile) { uploadFile.close(); }
	}
}


	
void handleString()
{
	String str = server.arg("value");
	if(str.startsWith("G0")){
		Serial.print("G91\n");    //wechelt auf relatives Koordinatensystem 
		delay(1000);
	}
	
	Serial.print(str + "\n");
	
	if(str.startsWith("G0")){
		delay(1000);
		Serial.print("G90\n");   //wechelt zurück auf ein absolutes Koordinatensystem 
	}
}

void handleRoot()
{
    server.send(200, "text/html", generateHTML(extrudertemp, buildplatetemp));
}


void handleNotFound()
{
    //digitalWrite(led, 1);
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";

    for (uint8_t i = 0; i < server.args(); i++)
    {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }

    server.send(404, "text/plain", message);
    //digitalWrite(led, 0);
}

void setup(void)
{
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    Serial.println("");


    // Wait for connection
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
	
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
	

	
    if (MDNS.begin("esp8266"))
    {
        Serial.println("MDNS responder started");
    }
	

    server.on("/", handleRoot);
	server.on("/textupload", handletextupload);
	server.on("/text", handletext);
	server.on("/file", handlefile);
	//server.on("/fileupload", handlefileupload);
	server.on("/sendString", handleString);
    /*
	server.on("/inline", []()
    {
        server.send ( 200, "text/plain", "this works as well" );
    });
	*/
    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println("HTTP server started");
}

void loop(void)
{
    server.handleClient();
	
	if(Serial.available() != 0){
	  String temp = Serial.readString();  //liest die nwort ein und wird für die Temperaturverwendet
	  temp.trim();
	  antwort = temp;
  }
  
  int pos1 = getposfromString(antwort, 'T');
  int pos2 = getposfromString(antwort, '/');
  int pos3 = getposfromString(antwort, 'B');
  int pos4 = getposfromString(antwort, '/', pos3);
  
  extrudertemp = antwort.substring(pos1+2, pos2-1);
  buildplatetemp = antwort.substring(pos3+2, pos4-1);
  
  int temperaturextruder = rand() % 100;
  int temperaturbuildplate = rand() % 100;
  
  extrudertemp = String(temperaturextruder);			//Das habe ich nur gemacht um zu zeigen, dass sich die Werte bei der HTML sich ändern, da ich leider nicht mit dem 
  buildplatetemp = String(temperaturbuildplate);		//3D Drucker kommunizieren kann
  
  
  //Serial.print(String(timer.getCounter()) + "\n");
  
  counter++;
  if(counter > 10000 && antwort.startsWith("ok")){	//&& Serial.available() != 0
	  Serial.print("M105\n");         //Ein Befehl, der eine Antwort vom 3D Drucker anfortert und 
	  counter = 0;                    //eine Antwort bekommt, aus der man die Temperatur auslesen kann.
  }                                   //Wird immer wieder ausgeführt, damit man die Temperaturwechsel auch erkennen kann.
}


//Leider konnte ich mit diesen Befehlen nicht meinen 3D DRucker ansteuern. Ich verstehe leider nicht wieso es nicht funktoniert, da ich die Befehle ausgelesen habe,
// als ich den Drucker mit meinem Laptop verbunden habe und die Befehle von meinem Laptop über uart ausgelesen habe. Dies habe ich immitiert mit meinem Program. 