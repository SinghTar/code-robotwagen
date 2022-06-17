#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "XXXXXXXXXXXXXXXXXXXX";
const char* password = "XXXXXXXXXXXXXXXXXXXX";
const char* mqtt_server  = "XXXXXXXXXXXXXXXXXXXX";
const int mqtt_port  = 1883;
const char* mqttUser = "XXXXXXXXXXXXXXXXXXXX";
const char* mqttPassword = "XXXXXXXXXXXXXXXXXXXX";

WiFiClient espClient;
PubSubClient client(espClient);

String TOPIC;
String bericht;

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client";
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqttUser, mqttPassword)) {
      Serial.println("connected");
      client.subscribe("robotwagen/automatischeMode");
      client.subscribe("robotwagen/manueleMode");
      client.subscribe("Farmlab/1/licht/aanUitKnop/level/3");
      client.subscribe("robotwagen/slinks");
      client.subscribe("robotwagen/vooruit");
      client.subscribe("robotwagen/srechts");
      client.subscribe("robotwagen/links");
      client.subscribe("robotwagen/stop");
      client.subscribe("robotwagen/rechts");
      client.subscribe("robotwagen/slachteruit");
      client.subscribe("robotwagen/achteruit");
      client.subscribe("robotwagen/srachteruit");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}


void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  bericht = "";
  TOPIC = topic;
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    bericht += (char)message[i];
  }
  Serial.println();
}


void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  reconnect();
}


void loop() {
  client.loop();
}
