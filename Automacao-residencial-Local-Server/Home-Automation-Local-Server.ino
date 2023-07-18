#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>

//Define the board's output and input pins.
#define RELAY_PIN_1 18        //Relay Pin 1
#define RELAY_PIN_2 19        //Relay Pin 2
#define RELAY_PIN_3 23        //Relay Pin 3
#define RELAY_PIN_4 05        //Relay Pin 4
#define WIFI_LED 25           //Network LED indicator pin
#define BUTTON_RESET_WIFI 17  //WiFi network reset button pin

#define BAUD_RATE 9600

//Help Variables in the Web App
String TEXT_STATE_1 = "OFF";
String TEXT_STATE_2 = "OFF";
String TEXT_STATE_3 = "OFF";
String TEXT_STATE_4 = "OFF";

WiFiServer server(80);
String header;

void appWebCliente() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            //Control relays on and off
            //relay 1
            if (header.indexOf("GET /relay1/on") >= 0) {
              Serial.println("Relay 1 on");
              TEXT_STATE_1 = "ON";
              digitalWrite(RELAY_PIN_1, LOW);
            } else if (header.indexOf("GET /relay1/off") >= 0) {
              Serial.println("Relay 1 off");
              TEXT_STATE_1 = "OFF";
              digitalWrite(RELAY_PIN_1, HIGH);
            }

            //relay 2
            else if (header.indexOf("GET /relay2/on") >= 0) {
              Serial.println("Relay 2 on");
              TEXT_STATE_2 = "ON";
              digitalWrite(RELAY_PIN_2, LOW);
            } else if (header.indexOf("GET /relay2/off") >= 0) {
              Serial.println("Relay 2 off");
              TEXT_STATE_2 = "OFF";
              digitalWrite(RELAY_PIN_2, HIGH);
            }

            //Relay 3
            else if (header.indexOf("GET /relay3/on") >= 0) {
              Serial.println("Relay 3 on");
              TEXT_STATE_3 = "ON";
              digitalWrite(RELAY_PIN_3, LOW);
            } else if (header.indexOf("GET /relay3/off") >= 0) {
              Serial.println("Relay 3 off");
              TEXT_STATE_3 = "OFF";
              digitalWrite(RELAY_PIN_3, HIGH);
            }

            //Relay 4
            else if (header.indexOf("GET /relay4/on") >= 0) {
              Serial.println("Relay 4 on");
              TEXT_STATE_4 = "ON";
              digitalWrite(RELAY_PIN_4, LOW);
            } else if (header.indexOf("GET /relay4/off") >= 0) {
              Serial.println("Relay 4 off");
              TEXT_STATE_4 = "OFF";
              digitalWrite(RELAY_PIN_4, HIGH);
            }


            //Off All
            else if (header.indexOf("GET /off-all") >= 0) {
              Serial.println("All relays off");
              TEXT_STATE_1 = "OFF";
              TEXT_STATE_2 = "OFF";
              TEXT_STATE_3 = "OFF";
              TEXT_STATE_4 = "OFF";

              digitalWrite(RELAY_PIN_1, HIGH);
              digitalWrite(RELAY_PIN_2, HIGH);
              digitalWrite(RELAY_PIN_3, HIGH);
              digitalWrite(RELAY_PIN_4, HIGH);
            }

            //Html web page interface
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #1E90FF; border-radius: 20px; color: white; padding: 4px 100px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            client.println("<body><h1>App Web &#128241;</h1>");


            //Relay 1
            client.println("<h4>RELAY 1</h4>");
            if (TEXT_STATE_1 == "OFF") {
              client.println("<p><a href=\"/relay1/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/relay1/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            //Relay 2
            client.println("<h4>RELAY 2</h4>");
            if (TEXT_STATE_2 == "OFF") {
              client.println("<p><a href=\"/relay2/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/relay2/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            //Relay 3
            client.println("<h4>RELAY 3</h4>");
            if (TEXT_STATE_3 == "OFF") {
              client.println("<p><a href=\"/relay3/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/relay3/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            //Relay 4
            client.println("<h4>RELAY 4</h4>");
            if (TEXT_STATE_4 == "OFF") {
              client.println("<p><a href=\"/relay4/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/relay4/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            client.println("<h4>OFF ALL<h4>");
            client.println("<p><a href=\"/off-all\"><button class=\"button\">ALL</button></a></p>");

            //End Html
            client.println("</body></html>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    header = "";

    //Close connection
    client.stop();
    Serial.println("Cliente Desconectado");
    Serial.println("");
  }
}

//Main configuration
void setup() {
  Serial.begin(BAUD_RATE);
  wifiConfigESP();

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("Address IP: ");
  Serial.println(WiFi.localIP());
  server.begin();

  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(RELAY_PIN_2, OUTPUT);
  pinMode(RELAY_PIN_3, OUTPUT);
  pinMode(RELAY_PIN_4, OUTPUT);
  pinMode(WIFI_LED, OUTPUT);

  pinMode(BUTTON_RESET_WIFI, INPUT_PULLUP);

  //During start-up all relays must turn OFF
  digitalWrite(RELAY_PIN_1, HIGH);
  digitalWrite(RELAY_PIN_2, HIGH);
  digitalWrite(RELAY_PIN_3, HIGH);
  digitalWrite(RELAY_PIN_4, HIGH);
  digitalWrite(WIFI_LED, LOW);
}

//Main loop
void loop() {
  changeNetwork();
  appWebCliente();

  if (WiFi.status() != WL_CONNECTED) {
    //Serial.println("WiFi Not Connected :( );
    digitalWrite(WIFI_LED, LOW);
    delay(100);
    digitalWrite(WIFI_LED, HIGH);
    delay(100);
  } else {
    //Serial.println("WiFi Connected");
    digitalWrite(WIFI_LED, HIGH);
  }
}