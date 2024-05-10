#include <WiFi.h>
#include <PubSubClient.h>

#define pin_Pot 33

// WiFi
const char *ssid = "alterwin";
const char *password = "123456788";

// MQTT Broker
const char *mqtt_broker = "free.mqtt.iyoti.id";
const char *topic_pub = "erwincl/ldr";
//const char *topic_sub = "robotikid/sub";
//const char *mqtt_username = "RobotikID";
//const char *mqtt_password = "1234567";
const int mqtt_port = 1883;

String client_id = "RobotikID_publisher";
int waktu_sebelum, waktu_interval = 1*1000;
String data_pot_sebelum="";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
   Serial.begin(115200);
  
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) {
       delay(500);
       Serial.println("Menghubungkan ke WiFi..");
   }
   Serial.println("Terhubung ke WiFi");

   //Menghubungkan ke mqtt broker
   client.setServer(mqtt_broker, mqtt_port);
   while (!client.connected()) {
       if (client.connect(client_id.c_str())) {
           Serial.println("Terhubung ke mqtt broker");
       } else {
           Serial.print("Gagal Terhubung ");
           Serial.print(client.state());
           delay(2000);
       }
   }
   
   // publish and subscribe
   client.publish(topic_pub, "Hi, ESP32");
   waktu_sebelum = millis();
}

void loop() {
  client.loop();
  int analog_pot = analogRead(pin_Pot);
  String data_pot = String(analog_pot);
  
  if(millis() - waktu_sebelum >= waktu_interval){
    client.publish(topic_pub, data_pot.c_str()); 
    Serial.println(data_pot);
    
    waktu_sebelum = millis();
    data_pot_sebelum = data_pot;
  }
}