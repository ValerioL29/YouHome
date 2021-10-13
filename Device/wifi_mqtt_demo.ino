#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiManager.h>
#include <PubSubClient.h>

// MQTT Broker
const char *mqtt_broker = "k4c5aa11.cn-hangzhou.emqx.cloud";
const char *topic_pub = "type/pub";
const char *topic_sub = "type/sub";
const char *mqtt_username = "deviceid";
const char *mqtt_password = "deviceid";
const int mqtt_port = 15487;

WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() {
  //初始化串口设置
  Serial.begin(115200);

  // Initialize the LED_BUILTIN pin as an output
  pinMode(LED_BUILTIN, OUTPUT);
 
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
}

// the loop function runs over and over again forever
void loop() {
  client.loop();

  // gather data

  // publish data
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
