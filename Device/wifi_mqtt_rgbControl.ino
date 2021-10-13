#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiManager.h>
#include <PubSubClient.h>

// MQTT Broker
const char *mqtt_broker = "k4c5aa11.cn-hangzhou.emqx.cloud";
const char *topic_pub = "rgb-control/pub";
const char *topic_sub = "rgb-control/sub";
const char *mqtt_username = "deviceid";
const char *mqtt_password = "deviceid";
const int mqtt_port = 15487;
const in device_id = 7001

WiFiClient espClient;
PubSubClient client(espClient);

int pin_r = 2;  // D4 GPIO_2 LED Red port
int pin_g = 14; // D5 GPIO_14 LED Green port
int pin_b = 12; // D6 GPIO_12 LED Blue port

String cmd_msg = "";

void setup() {
  //初始化串口设置
  Serial.begin(115200);

  // Initialize the RGB pin as an output
  pinMode(pin_r,OUTPUT);
  pinMode(pin_g,OUTPUT);
  pinMode(pin_b,OUTPUT);
 
  // Instantiate a WiFiManager object
  WiFiManager wifiManager;

  // Wifi reset
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
  client.publish("hello", "hello emqx from rgb-control");
  client.subscribe(topic_sub);
}

// the loop function runs over and over again forever
void loop() {
  client.loop();

  // Execute cmd
  if(cmd_msg != ""){
    rgbControl(cmd_msg);
    cmd_msg = "";
    client.publish(topic_pub, "1");
  }
}

void callback(char *topic, byte *payload, unsigned int length) {
  cmd_msg = "";
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    cmd_msg += (char) payload[i];
  }
  Serial.println(cmd_msg);
  Serial.println("-----------------------");
}

int lumi;
void rgbControl(String cmd){
  Serial.println(cmd);
  lumi = (int)cmd[0] - 48;

  Serial.print("lumi: ");
  Serial.println(lumi);

  if(lumi == 0) color(0,0,0);
  if(lumi == 1) color(127,127,127);
  if(lumi == 2) color(255,255,255);

  delay(10);
}

void color(int r, int g, int b){//设置灯光颜色以及亮度
  analogWrite(pin_r,r);
  analogWrite(pin_g,g);
  analogWrite(pin_b,b);
}