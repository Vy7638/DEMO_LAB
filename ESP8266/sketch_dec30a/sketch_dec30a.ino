#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

//Wifi name
#define WLAN_SSID       "Deadline"
//Wifi password
#define WLAN_PASS       "tranngocvy"

//setup Adafruit
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
//fill your username                   
#define AIO_USERNAME    "Vy2908"
//fill your key
#define AIO_KEY         "aio_sEDK69C7olFoY0RNSQrEj4tCJm11"

//setup MQTT
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

//setup publish
// Adafruit_MQTT_Publish light_pub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/led");
Adafruit_MQTT_Publish live_pub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/live");
Adafruit_MQTT_Publish temperature_pub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");
Adafruit_MQTT_Publish humedity_pub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidity");
Adafruit_MQTT_Publish light_pub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/light");
//setup subcribe
// Adafruit_MQTT_Subscribe live_sub = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/live", MQTT_QOS_1);

int led_counter = 0;
int led_status = HIGH;

// void lightcallback(char* value, uint16_t len){
//   if(value[0] == '0') Serial.print('a');
//   if(value[0] == '1') Serial.print('A');
// }

void setup() {
  // put your setup code here, to run once:
  //set pin 2,5 as OUTPUT
  pinMode(2, OUTPUT);
  pinMode(5, OUTPUT);
  //set busy pin HIGH
  digitalWrite(5, HIGH);

  Serial.begin(115200);

  //connect Wifi
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  
  //subscribe light feed
  // light_sub.setCallback(lightcallback);
  // mqtt.subscribe(&live_sub);

  //connect MQTT
  while (mqtt.connect() != 0) { 
    mqtt.disconnect();
    delay(500);
  }

  //finish setup, set busy pin LOW
  digitalWrite(5, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:

  //receive packet
  mqtt.processPackets(1);
  
  //read serial
  if(Serial.available()){
    int msg = Serial.read();
    if(msg == 'o') Serial.print('O');
    else if (msg == 'A') live_pub.publish(1);
    else if (msg == 'a') live_pub.publish(0);
    else if (msg == 'L') {
      msg = Serial.read();
        light_pub.publish(msg);
      msg = Serial.read();
        humedity_pub.publish(msg);
      // msg = Serial.read();
      //   temperature_pub.publish(msg);     
    }
  }

  led_counter++;
  if(led_counter == 100){
    // every 1s
    led_counter = 0;
    //toggle LED
    if(led_status == HIGH) led_status = LOW;
    else led_status = HIGH;

    digitalWrite(2, led_status);
  }
  delay(10);
}