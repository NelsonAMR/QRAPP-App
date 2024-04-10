// Include libraries
#include <WiFiManager.h> // Libreria para conectarse a WiFi
#include <ESP32Servo.h>  // Libreria para controlar servos
#include <HTTP_Method.h> // Libreria para manejar peticiones HTTP
#include <Uri.h>         // Libreria para manejar URLs
#include <WebServer.h>   // Libreria para manejar servidor web


// Inputs/outputs
const int ledSuccess = 18;
const int ledFailure = 19;

// Create instances
WebServer webServer;
Servo servo;

// Función que maneja las solicitudes al servidor web
void app()
{
  if (webServer.hasArg("status")) // Verifica si la solicitud tiene un argumento llamado "status"
  {
    // Si el argumento es "success", enciende el LED de éxito y gira el servo
    if (webServer.arg("status") == "success")
    {
      digitalWrite(ledSuccess, HIGH);
      digitalWrite(ledFailure, LOW);
      for (int x = 0; x <= 75; x++)
      {
        servo.write(x); // Gira el servo a una posición específica
        delay(10);
      }

      delay(3000); // Espera 3 segundos
      digitalWrite(ledSuccess, LOW);
      for (int x = 75; x >= 0; x--)
      {
        servo.write(x); // Gira el servo a una posición específica
        delay(10);
      }
    }
    // Si el argumento es "failure", enciende el LED de fracaso y detiene el servo
    else if (webServer.arg("status") == "failure")
    {
      digitalWrite(ledSuccess, LOW);
      digitalWrite(ledFailure, HIGH);
      servo.write(0); // Detiene el servo
      delay(1000);    // Espera 1 segundo
      digitalWrite(ledFailure, LOW);
    }
    // Si el argumento es "off", apaga ambos LEDs y detiene el servo
    else if (webServer.arg("status") == "off")
    {
      digitalWrite(ledSuccess, LOW);
      digitalWrite(ledFailure, LOW);
      servo.write(0); // Detiene el servo
    }
  }
}

// Función de configuración que se ejecuta una vez al inicio
void setup()
{
  // Inicia la comunicación serial a una velocidad de 115200 baudios
  Serial.begin(115200);

  // Configura los pines de los LEDs como salidas
  pinMode(ledSuccess, OUTPUT);
  pinMode(ledFailure, OUTPUT);
  // Adjunta el servo al pin 23
  servo.attach(23);

  // Conecta a la red WiFi utilizando el SSID y la contraseña proporcionados
  WiFiManager wiFiManager;
  wiFiManager.resetSettings();

  bool res = wiFiManager.autoConnect("QRAPP","12345678"); // password protected ap

  if(!res) {
    Serial.println("Failed to connect!");
  } 
  else {
    Serial.println("Connected");
    // Configura el servidor web para manejar solicitudes a la ruta "/" utilizando la función app()
    webServer.on("/", app);
    // Inicia el servidor web para comenzar a manejar las solicitudes HTTP entrantes
    webServer.begin();
  }
}

// Función de bucle principal que maneja las solicitudes HTTP entrantes
void loop()
{
  webServer.handleClient();
}