// ############# LIBRARIES ############### //

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// ############# VARIABLES ############### //

const char* SSID = "eu"; // rede wifi
const char* PASSWORD = "eueueueu"; // senha da rede wifi

String BASE_URL = "http://192.168.43.235:3000/ring-the-bell";

const int buttonPin = 16; // Pino digital do botão

// ############# PROTOTYPES ############### //

void initSerial();
void initWiFi();
void httpRequest(String path);

// ############### OBJECTS ################# //

WiFiClient client;
HTTPClient http;

// ############## SETUP ################# //

void setup() {
  initSerial();
  initWiFi();
  pinMode(buttonPin, INPUT);
}

void loop() {
  int buttomState = digitalRead(buttonPin);
  Serial.print("##[BUTTOM]## ");
  Serial.println(buttomState);

  if (buttomState == HIGH) {
    httpRequest("true"); 
  }  
}

// ############# HTTP REQUEST ################ //

void httpRequest(String ringingTheBell)
{
  String payload = makeRequest(ringingTheBell);

  if (!payload) {
    return;
  }

  Serial.println("##[RESULT]## ==> " + payload);

}

String makeRequest(String ringingTheBell)
{
  http.begin(BASE_URL);
  http.addHeader("content-type", "application/x-www-form-urlencoded");

  String body = "ringingTheBell=" + ringingTheBell;

  int httpCode = http.POST(body);

  if (httpCode < 0) {
    Serial.print("request error - ");
    Serial.println(httpCode);
    return "";

  }

  if (httpCode != HTTP_CODE_OK) {
    return "";
  }

  String response =  http.getString();
  http.end();

  return response;
}

// ###################################### //

// implementacao dos prototypes

void initSerial() {
  Serial.begin(115200);
  while (!Serial);
}

void initWiFi() {
  delay(10);
  Serial.println("Conectando-se em: " + String(SSID));

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Conectado na Rede " + String(SSID) + " | IP => ");
  Serial.println(WiFi.localIP());
}
