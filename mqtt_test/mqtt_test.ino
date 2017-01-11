#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// const char *ssid = "xxx";
// const char *password = "xxx";
const char *mqtt_server = "192.168.1.154";
const int mqtt_port = 1883;
const char *mqtt_user = "/:espuser";
const char *mqtt_password = "esppassword";
const char *mqtt_topic = "esptopic";

WiFiClient espclient;
PubSubClient client(espclient);

void callback(char* topic, byte* body, unsigned int len) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  for (uint32_t i = 0; i < len; i++)
    Serial.print((char)body[i]);
  Serial.println();
}

void setup()
{
  Serial.begin(9600);
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop()
{
  if (!client.connected()) {
    while (!client.connected()) {
      Serial.print("Connecting to MQTT broker...");
      if (client.connect("ESP8266 Client", mqtt_user, mqtt_password)) {
        Serial.println("connected");
        client.subscribe(mqtt_topic, 1);
      } else {
        Serial.print("failed (");
        Serial.print(client.state());
        Serial.println(")");
      }
      delay(5000);
    }
  }
  client.loop();
}
