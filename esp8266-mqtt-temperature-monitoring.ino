#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

// what pin we're connected to
#define DHTPIN  D4
#define LEDBLUE D5
#define LEDRED  D6

// wifi credential
#define wifi_ssid "somewifiname"
#define wifi_password "somewifipassword"

// MQTT server credentials(you may have username and password too)
#define mqtt_server "192.168.100.2"

// MQTT topics(channels) that we work with
#define temperature_topic "/temperature"
#define action_topic "/action"
#define monitor_topic "/monitor"

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11

WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);

float temp = 0.0;
String lastMessage = "";

void setup() {
  pinMode(LEDRED, OUTPUT);
  pinMode(LEDBLUE, OUTPUT);
  delay(100);
  
  digitalWrite(LEDRED, LOW);
  digitalWrite(LEDBLUE, LOW);
  
  Serial.begin(115200);
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  dht.begin();
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change the next line
    if (client.connect("changeMe")) {
      Serial.println("connected");
      delay(100);
      client.subscribe(action_topic);
      client.publish(monitor_topic, "* * * ESP8266 restarded * * *");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// callback function that gets executed on each incoming message
void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Action arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Action:");
  
  String message = ""; 
  for (int i = 0; i < length; i++) {
      message += (char)payload[i];
  }

  Serial.print(message);
  Serial.println();
  Serial.println("-----------------------");
  
  String logMessage = "";
  
  if(message == "-1"){
    digitalWrite(LEDRED, HIGH);
    digitalWrite(LEDBLUE, LOW);
    logMessage = "Heater is ON, Cooler is OFF";
  }else if(message == "0"){
    digitalWrite(LEDRED, LOW);
    digitalWrite(LEDBLUE, LOW);
    logMessage = "Heater is OFF, Cooler is OFF";
  }else if(message == "1"){
    digitalWrite(LEDRED, LOW);
    digitalWrite(LEDBLUE, HIGH);
    logMessage = "Heater is OFF, Cooler is ON";
  }

  // publish the ack of the action to the server
  if(logMessage != lastMessage){
    client.publish(monitor_topic, logMessage.c_str());
  }
  lastMessage = logMessage;
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)

  // Read temperature as Celcius
  float newTemp = dht.readTemperature();
  temp = newTemp;
  Serial.print("New temperature:");
  Serial.println(String(temp).c_str());
  client.publish(temperature_topic, String(temp).c_str(), true);

  client.loop();
  
  // Wait a few seconds between measurements(10 seconds).
  delay(10000);
}
