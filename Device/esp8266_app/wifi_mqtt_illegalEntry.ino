#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <Ticker.h>

// Pin port definition
const int collision_pin = 5; // D1
const int echo_pin = 4; // D2
const int buzzer_pin = 0; // D3
const int led_pin = 2; // D4

// MQTT Broker
const char *mqtt_broker = "k4c5aa11.cn-hangzhou.emqx.cloud";
const char *topic_pub = "illegal-entry/pub";
const char *topic_sub = "illegal-entry/sub";
const char *mqtt_username = "telecom";
const char *mqtt_password = "telecom";
const int mqtt_port = 15487;
const int device_id = 7013

WiFiClient espClient;
PubSubClient client(espClient);
Ticker ticker; // Ticker object for time-round function
String msg_pub = "701310";
 
void setup() {
  //初始化串口设置
  Serial.begin(9600);

  // Initialize pins
  pinMode(collision_pin,INPUT);
  pinMode(echo_pin,INPUT);
  pinMode(buzzer_pin, OUTPUT);
  pinMode(led_pin, OUTPUT);
 
  // Instantiate a WiFiManager object
  WiFiManager wifiManager;

  //  wifiManager.resetSettings();
  
  // Auto-connecting WiFi
  // The argument is AP's name when we connecting to ESP8266
  wifiManager.autoConnect("Group23-AutoConnectAP");

  // WiFi
  Serial.print("WiFi Connected!\n");
  String ssid = wifiManager.getWiFiSSID(); // Enter your WiFi name
  String password = wifiManager.getWiFiPass(); // Enter WiFi password

  // connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  //connecting to a mqtt broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
      String client_id = "esp8266-client-";
      client_id += String(WiFi.macAddress());
      Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
      if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
          Serial.println("Emqx mqtt broker connected");
      } else {
          Serial.print("failed with state ");
          Serial.print(client.state());
          delay(2000);
      }
  }
  // publish and subscribe
  client.publish("hello", "hello emqx");
  client.subscribe(topic_sub);

  // Run data_publishment once in each seconds
  ticker.attach(1, data_publishment);
}

// the loop function runs over and over again forever
void loop() {
  client.loop();

  // gather data
  data_collection();
}

void data_collection(){
  msg_pub = ""

  int door = digitalRead(collision_pin);
  float dist = pulseIn(echo_pin, HIGH) / 58.0; // per cm
  dist = (int(cm * 100.0)) / 100.0;

  if(door == 0 && cm <= 10) digitalWrite(led_pin, HIGH);
  else digitalWrite(led_pin, LOW);

  msg_pub += device_id;
  msg_pub += door;
  msg_pub += door;

  Serial.println(msg_pub);
  delay(10);      //延时1秒
}

void data_publishment(){
  Serial.println("Msg published successfully!");
  client.publish(topic_pub, msg_pub.c_str());
}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
     Serial.print((char) payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");
}
