#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>

#ifndef WIFI_PSW
    #define WIFI_PSW "(WIFI_PSW not defined)"
#endif
#ifndef MQTT_PSW
    #define MQTT_PSW "(MQTT_PSW not defined)"
#endif

const char* ssid = "D-Link-Fibra";
const char* password = WIFI_PSW;
const char* mqttServer = "mqtt.flespi.io";
const int mqttPort = 1883;
const char* mqttName = "ESP--leds-strip";
const char* mqttUser = MQTT_PSW;
const char* mqttPassword = "";

const char* lgChannel = "logs";
const char* inChannel = "lights/leds";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

int rgbPins[3] = {D5, D6, D7};

extern const uint8_t gamma8[];
void setColor(long color);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup_OTA() {
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived in topic ");
  Serial.print(topic);
  Serial.print(": ");
  //string to hex
  payload[length] = '\0';
  Serial.println((char *)payload);
  // Ignore '#'
  payload++;
  Serial.println("Inside fnc_ColorChange attempted to change color");
  // Get rid of '#' and convert it to integer
  const char* trial = "ff0000";
  Serial.println(trial);
  payload[6] = '\0';
  Serial.println((char*)payload);
  long number0 = strtol(trial, NULL, 16);
  Serial.print("Converting to long: ");
  Serial.println(number0);
  long number = strtol((char*)payload, NULL, 16);
  Serial.print("Converting to long: ");
  Serial.println(number);
  setColor(number);
  Serial.println("-----------------------");
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(mqttName, mqttUser, mqttPassword)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(lgChannel, "ESP connected");
      // ... and resubscribe
      client.subscribe(inChannel);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setColor(long color) {
  int r = color >> 16;
  int g = color >> 8 & 0xFF;
  int b = color & 0xFF;

  int length = snprintf( NULL, 0, "Setting led strip to rgb(%d, %d, %d)", r, g, b);
  char* str = (char*) malloc( length + 1 );
  snprintf( str, length + 1, "Setting led strip to rgb(%d, %d, %d)", r, g, b);
  client.publish(lgChannel, str);

  // Multiply by 4 to account for NodeMCU increased resolution (range 0-1023)
  // Make it 4.01 to map 255 -> 1023
  r = round(4.01 * pgm_read_byte(&gamma8[r]));
  g = round(4.01 * pgm_read_byte(&gamma8[g]));
  b = round(4.01 * pgm_read_byte(&gamma8[b]));

  analogWrite(rgbPins[0], r);
  analogWrite(rgbPins[1], g);
  analogWrite(rgbPins[2], b);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  setup_wifi();
  setup_OTA();
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}

void loop() {
  ArduinoOTA.handle();
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

const uint8_t PROGMEM gamma8[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };
