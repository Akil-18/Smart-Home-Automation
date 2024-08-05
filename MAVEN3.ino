#include "DHT.h"
#include <WiFi.h>
#include <WebServer.h>
#include <stdint.h>

// Relay
#define fanRelay        17
#define bulbRelay       5

// Ultrasonic Sensor
#define echoPin         26
#define trigPin         27

// LDR
#define ldrPin          33

// DHT11
#define DHTPIN          16
#define DHTTYPE         DHT11

// Status LED
#define automaticLed    4
#define manualLed       15

// Values
#define automatic       0
#define manual          1

// WiFi Credentials
const char* ssid = "Akil";
const char* password = "virat1234";

//Global Variables
uint8_t mode = 0;
uint8_t fanState = LOW;
uint8_t bulbState = LOW;
uint8_t modeChange = 1;
uint8_t personFound;
uint8_t distanceThreshold = 10;
long duration;
float distance_cm;
float temperature;

// DHT object definition
DHT dht(DHTPIN, DHTTYPE);

// WiFiServer object
WiFiServer server(80);

//Setup Function
void setup() 
{
  // Serial Monitor Initialization
  Serial.begin(115200);
  // Components Initialization
  relayInit();
  ultrasonicInit();
  ldrInit();
  dhtInit();
  ledInit();
  // WiFi Initialization
  wifiInit();
}

void relayInit()
{
  pinMode(fanRelay, OUTPUT);
  digitalWrite(fanRelay, LOW);
  pinMode(bulbRelay, OUTPUT);
  digitalWrite(bulbRelay, LOW);
}

void ultrasonicInit()
{
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
} 

void ldrInit()
{
  pinMode(ldrPin, INPUT);
}

void dhtInit()
{
  dht.begin();
}

void ledInit()
{
  pinMode(automaticLed, OUTPUT);
  pinMode(manualLed, OUTPUT);
}

void wifiInit()
{
  uint8_t timeout = 20;
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED && timeout!=0) {
    delay(500);
    Serial.print(".");
    timeout--;
  }

  if(timeout == 0)
  {
    Serial.println();
    Serial.println("Couldn't connect to WiFi. Automatic Mode Activated by default");
    automaticMode();
    return;
  }

  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());
  server.begin();

}

void automaticMode(){
  unsigned long previousMillis = 0;
  const long interval = 1000; 

  while (mode == automatic) {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      //Ultrasonic Sensor Values
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);

      duration = pulseIn(echoPin, HIGH);
      distance_cm = duration * 0.034 / 2;

      if(distance_cm > distanceThreshold)
      {
          personFound = 0;
          digitalWrite(fanRelay, LOW);
          digitalWrite(bulbRelay, LOW);
          Serial.println("Person Not Detected within Threshold");
          continue;
      }
      personFound = 1;
      Serial.print("Person Detected within ");
      Serial.print(distanceThreshold);
      Serial.println(" cm");

      temperature = dht.readTemperature();
      int ldrValue = analogRead(ldrPin);

      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.print(" °C");
      Serial.println(ldrValue);

      if (temperature > 30) {
        digitalWrite(fanRelay, HIGH);
        fanState = HIGH;
        Serial.println("Temperature High. Fan ON");
      } else {
        digitalWrite(fanRelay, LOW);
        fanState = LOW;
        Serial.println("Temperature Normal - Fan OFF");
      }

      if (ldrValue < 4000) {
        digitalWrite(bulbRelay, LOW);
        Serial.println("Room is well-lit. Bulb OFF");
        Serial.println(ldrValue);
        bulbState = LOW;
      } else {
        digitalWrite(bulbRelay, HIGH);
        bulbState = HIGH;
        Serial.println("Room is dark. Bulb ON");
      }
    }
    manualMode();
    if (mode == manual)
    {
      break;
    }
    delay(200);
  }
}

void toggleFan() {
  fanState = !fanState;
  digitalWrite(fanRelay, fanState);
  Serial.print("Fan is ");
  Serial.println(fanState ? "ON" : "OFF");
}

void toggleBulb() {
  bulbState = !bulbState;
  digitalWrite(bulbRelay, bulbState);
  Serial.print("Bulb is ");
  Serial.println(bulbState ? "ON" : "OFF");
}

void manualMode()
{
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client.");

    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // HTML content starts here
            client.println("<!DOCTYPE html>");
            client.println("<html lang=\"en\">");
            client.println("<head>");
            client.println("<meta charset=\"UTF-8\">");
            client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
            client.println("<title>Maven Silicon Project: Home Automation using ESP32</title>");
            client.println("<style>");
            client.println("body { font-family: Arial, sans-serif; background-color: #f0f0f0; margin: 0; padding: 0; display: flex; justify-content: center; align-items: center; min-height: 100vh; }");
            client.println(".container { max-width: 400px; width: 100%; padding: 20px; box-sizing: border-box; }");
            client.println(".control-box { background-color: #ffffff; padding: 20px; border-radius: 10px; box-shadow: 0 0 10px rgba(0,0,0,0.1); text-align: center; margin-bottom: 20px; }");
            client.println(".mode-link, .device-toggle { display: inline-block; width: 90%; padding: 12px; font-size: 16px; margin: 10px 5px; text-decoration: none; color: #ffffff; background-color: #007bff; border: 1px solid #007bff; border-radius: 5px; cursor: pointer; }");
            client.println(".device-toggle { width: 45%; font-size: 14px; padding: 10px; background-color: #28a745; border: 1px solid #28a745; }");
            client.println(".mode-display { margin-top: 20px; font-size: 18px; color: red; text-align: center; }");
            client.println("h2 { margin-top: 0; margin-bottom: 20px; font-size: 24px; color: #333333; text-align: center; }");
            client.println("</style>");
            client.println("</head>");
            client.println("<body>");
            client.println("<div class=\"container\">");
            client.println("<h2>Home Automation using ESP32</h2>");
            client.println("<div class=\"control-box\">");
            client.println("<a href=\"/automaticMode\" class=\"mode-link\">Automatic Mode</a>");
            client.println("<a href=\"/manualMode\" class=\"mode-link\">Manual Mode</a>");
            client.println("<div id=\"manual-controls\" style=\"display: none;\">");
            client.println("<a href=\"/toggleFan\" class=\"device-toggle\">Fan: Off</a>");
            client.println("<a href=\"/toggleBulb\" class=\"device-toggle\">Bulb: Off</a>");
            client.println("</div>");
            client.println("</div>");
            client.println("<div class=\"mode-display\" id=\"modeDisplay\"></div>");
            client.println("</div>");

            // JavaScript for handling clicks
            client.println("<script>");
            client.println("document.querySelectorAll('.mode-link').forEach(link => {");
            client.println("  link.addEventListener('click', function(event) {");
            client.println("    event.preventDefault();");
            client.println("    let modeText = link.textContent.trim();");
            client.println("    if (modeText === 'Automatic Mode') {");
            client.println("      document.getElementById('manual-controls').style.display = 'none';");
            client.println("      document.getElementById('modeDisplay').textContent = 'Automatic Mode Activated';");
            client.println("      fetch('/automaticMode');");
            client.println("    } else if (modeText === 'Manual Mode') {");
            client.println("      document.getElementById('manual-controls').style.display = 'block';");
            client.println("      document.getElementById('modeDisplay').textContent = 'Manual Mode Activated';");
            client.println("      fetch('/manualMode');");
            client.println("    }");
            client.println("  });");
            client.println("});");

            client.println("document.querySelectorAll('.device-toggle').forEach(button => {");
            client.println("  button.addEventListener('click', function(event) {");
            client.println("    event.preventDefault();");
            client.println("    let buttonText = button.textContent.trim();");
            client.println("    if (buttonText.startsWith('Fan')) {");
            client.println("      fetch('/toggleFan').then(() => button.textContent = 'Fan: ' + (buttonText.endsWith('Off') ? 'On' : 'Off'));");
            client.println("    } else if (buttonText.startsWith('Bulb')) {");
            client.println("      fetch('/toggleBulb').then(() => button.textContent = 'Bulb: ' + (buttonText.endsWith('Off') ? 'On' : 'Off'));");
            client.println("    }");
            client.println("  });");
            client.println("});");
            client.println("</script>");
            client.println("</body>");
            client.println("</html>");
            // HTML content ends here

            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }

        if (currentLine.endsWith("GET /automaticMode")) {
          Serial.println("Switching to Automatic Mode");
          mode = automatic;
          modeChange = 1;
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();
          client.println("<!DOCTYPE html><html><body><h1>Automatic Mode Activated</h1></body></html>");
          client.println();
          break;
        }

        if (currentLine.endsWith("GET /manualMode")) {
          Serial.println("Switching to Manual Mode");
          mode = manual;
          modeChange = 1;
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();
          client.println("<!DOCTYPE html><html><body><h1>Manual Mode Activated</h1></body></html>");
          client.println();
          return;
        }

        if (currentLine.endsWith("GET /toggleFan")) {
          Serial.println("Toggling Fan");
          toggleFan();
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();
          client.println("<!DOCTYPE html><html><body><h1>Fan Toggled</h1></body></html>");
          client.println();
          break;
        }

        if (currentLine.endsWith("GET /toggleBulb")) {
          Serial.println("Toggling Bulb");
          toggleBulb();
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();
          client.println("<!DOCTYPE html><html><body><h1>Bulb Toggled</h1></body></html>");
          client.println();
          break;
        }
      }
    }

    client.stop();
    Serial.println("Client Disconnected.");
  }
}

//Loop Function
void loop() 
{
  if (modeChange) {
    if (mode == automatic) {
      Serial.println("Entering Automatic Mode");
      digitalWrite(automaticLed, HIGH);
      digitalWrite(manualLed, LOW);
      automaticMode();
    }
    if(mode == manual){
      Serial.println("Entering Manual Mode");
      digitalWrite(automaticLed, LOW);
      digitalWrite(manualLed, HIGH);
    }
  }
  modeChange = 0;
  manualMode();
}
