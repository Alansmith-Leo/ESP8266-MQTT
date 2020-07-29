#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char *ssid = "*******"; // Enter your WiFi name
const char *password = "********";  // Enter WiFi password
const char *mqtt_broker = "broker.emqx.io";
const int mqtt_port = 1883;
const char* mqtt_pub_topic="******";
float a,b,c;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  a=0;
  b=21.1;
  c=2.4552;
  
  // Set software serial baud to 115200;
  Serial.begin(115200);
  
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
    Serial.println("Connecting to public emqx mqtt broker.....");
    if (client.connect("esp8266-client")) {
      Serial.println("Public emqx mqtt broker connected");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  // publish and subscribe
  client.publish("esp8266/test", "hello emqx");
  client.subscribe("esp8266/test");
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

void loop() {
  
  client.loop();
  String json = "x:"+String(a)+"y:"+String(b)+"z:"+String(c)+"";
  client.publish(mqtt_pub_topic,json.c_str());
  delay(1000);
}
