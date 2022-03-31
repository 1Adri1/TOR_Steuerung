#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
 

 //Change user, password and IP to get it running.
const char* ssid = "user";
const char* password = "password";

const int tor1 = D6;
const int tor2 = D7;

 
ESP8266WebServer server(80);

const char* www_username = "user";
const char* www_password = "password";

 
void gettor() {
int tor;

  if (!server.authenticate(www_username, www_password)) {
      return server.requestAuthentication();
    }

switch(server.arg("tor").toInt()){
    case 0:
    tor = D6;
    break;
    case 1:
    tor = D7;
    break;
    default:
    return;
}



  if(server.hasArg("tor")){
    server.send(200, "text/json", "Tor "+server.arg("tor")+" betaetigt");
    digitalWrite(tor, LOW);
    delay(1000);
    digitalWrite(tor, HIGH);   
  }
  else{
    server.send(200, "text/json", "WRONG INPUT");
  }
}

/*
void gettorip(String adresse) {
    HTTPClient http;  //Object of class HTTPClient
    WiFiClient client;
    http.begin(client, adresse);
    //http.setAuthorization("guest", "guest");
    http.setAuthorization("");
    int httpCode = http.GET();
    //Check the returning code
    if (httpCode > 0) {
      // Get the request response payload
      String payload = http.getString();
      server.send(200, "text/json", payload);
    }
    http.end();   //Close connection
}
*/

 
// Define routing
void restServerRouting() {
    /*server.on("/", HTTP_GET, []() {
        server.send(200, F("text/html"),
            F("Welcome to the REST Web Server"));
    });
    */
    server.on(F("/tor"), HTTP_GET, gettor);
   

}
 
// Manage not found URL
void handleNotFound() {


  
  /*
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  */
  server.send(404, "text/html", "<html>\
<h1 style='text-align: center;'><span style='color: #ff0000;'><strong>404</strong></span></h1>\
<p style='text-align: center;'><span style='color: #ff0000;'><strong>! Seite nicht gefunden !</strong></span></p>\
</html>");
  
}
 
void setup(void) {
  pinMode(tor1, OUTPUT);
  pinMode(tor2, OUTPUT);
  digitalWrite(tor1, HIGH);
  digitalWrite(tor2, HIGH);
  Serial.begin(115200);
  IPAddress ip(192, 168, 2, 220);
  IPAddress dns(192, 168, 2, 1);
  IPAddress gateway(192, 168, 2, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.mode(WIFI_STA);
  WiFi.config(ip, dns, gateway, subnet);
  WiFi.begin(ssid, password);
  Serial.println("");
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

    
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
 
  // Set server routing
  restServerRouting();
  // Set not found response
  server.onNotFound(handleNotFound);
  // Start server
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void) {
  server.handleClient();
}
