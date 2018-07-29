int WET= 16; // Wet Indicator at Digital PIN D0
int DRY= 2;  // Dry Indicator at Digital PIN D4

int sense_Pin= 0; // Soil Sensor input at Analog PIN A0
int value= 0;

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <SPI.h>

const char* ssid = "wifi name";
const char* password = "wifi pass";

//Api
const char* host = "api.pushbullet.com";
//https://api.pushbullet.com/v2/pushes
const int httpsPort = 443;

const char* accessToken = "o.0GoDPKLYn9uMQMk9yJwFWOmfaiDeu50J";

const char* fingerprint = "BBFC9F1BC13CD996F268A2E34129D1478FB933BE";

void conn(char msg[]){
  WiFiClientSecure client;
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  if (client.verify(fingerprint, host)) {
    Serial.println("certificate matches");
  } else {
    Serial.println("certificate doesn't match");
  }

  String url = "/v2/pushes";
  Serial.print("requesting URL: "); Serial.println(url);

  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: ESP8266\r\n" +
               "Access-Token: " + accessToken + "\r\n" +
               "Content-length: 114\r\n"
               "Content-Type: application/json\r\n" +
               "Connection: close\r\n\r\n" +msg
              );


  Serial.println("request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  //String line1 = client.readString(); // this line is good for debugging error messages more then a single line
  if (line.startsWith("{\"active\":true")) {
    Serial.println("esp8266/Arduino CI successfull!");
  } else {
    Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("closing connection");
  
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  
  Serial.println("");
  
   pinMode(WET, OUTPUT);
   pinMode(DRY, OUTPUT);
   delay(2000);
}

void loop() {
   Serial.print("MOISTURE LEVEL : ");
   value= analogRead(sense_Pin);
   value= value/10;
   Serial.println(value);
   if(value<50)
   {
      digitalWrite(WET, HIGH);
      char a[]="{\"body\":\"Tmo Capsule And One Tablet was taken Successfully. Thank You\",\"title\":\"OK, Medicine Taken\",\"type\":\"note\"}";
       conn(a);
   }
   else
   {
      digitalWrite(DRY,HIGH);
     char a[]="{\"body\":\"Two Capsule And One Tablet was taken Successfully. Thank You\",\"title\":\"OK, Medicine Taken\",\"type\":\"note\"}";
    conn(a);
   }
   delay(1000);
   digitalWrite(WET,LOW);
   digitalWrite(DRY, LOW);
}
