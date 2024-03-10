

// Include libraries
#include <WiFi.h>
#include <ESP32Servo.h>
#include <HTTP_Method.h>
#include <Uri.h>
#include <WebServer.h>

// Add WIFI data
const char *ssid = "";
const char *password = "";

// Inputs/outputs
const int ledSuccess = 18;
const int ledFailure = 19;

WebServer webServer;
Servo servo;

void app()
{
  if (webServer.hasArg("status") && webServer.arg("status") == "success")
  {
    digitalWrite(ledSuccess, HIGH);
    digitalWrite(ledFailure, LOW);
    servo.write(75);

    delay(3000);
    digitalWrite(ledSuccess, LOW);
    servo.write(0);
  }

  else if (webServer.hasArg("status") && webServer.arg("status") == "failure")
  {
    digitalWrite(ledSuccess, LOW);
    digitalWrite(ledFailure, HIGH);
    servo.write(0);

    delay(1000);
    digitalWrite(ledFailure, LOW);
  }

  else if (webServer.hasArg("status") && webServer.arg("status") == "off")
  {
    digitalWrite(ledSuccess, LOW);
    digitalWrite(ledFailure, LOW);
    servo.write(0);
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(ledSuccess, OUTPUT);
  pinMode(ledFailure, OUTPUT);

  servo.attach(23);

  WiFi.begin(ssid, password);
  Serial.print("Connecting...");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.print("\t Connected, my IP: ");
  Serial.println(WiFi.localIP());

  webServer.on("/", app);
  webServer.begin();
}

void loop()
{
  webServer.handleClient();
}