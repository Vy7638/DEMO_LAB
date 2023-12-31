#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

//Wifi name
#define WLAN_SSID       "Redmi Note 11"
//Wifi password
#define WLAN_PASS       "531327aA"

//setup Adafruit
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
//fill your username                   
#define AIO_USERNAME    "Vy2908"
//fill your key
#define AIO_KEY         "aio_GTTY71H1z2XZ68beCkQHaRstguVv"

//setup MQTT
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

//setup publish
Adafruit_MQTT_Publish light_pub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/led");
Adafruit_MQTT_Publish lighttt_pub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/light");
Adafruit_MQTT_Publish hum_pub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humedity");
Adafruit_MQTT_Publish temp_pub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");
Adafruit_MQTT_Publish name_pub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/name");
//setup subcribe
Adafruit_MQTT_Subscribe light_sub = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/led", MQTT_QOS_1);

int led_counter = 0;
int led_status = HIGH;
int ch = 0;
float ct = 0.0;
String cname = "";

void lightcallback(char* value, uint16_t len){
  if(value[0] == '0') Serial.print('a');
  if(value[0] == '1') Serial.print('A');
}

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
  light_sub.setCallback(lightcallback);
  mqtt.subscribe(&light_sub);

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
    else if(msg == 'a') light_pub.publish(0);
    else if(msg == 'A'){
    light_pub.publish(1);
      char msgc = Serial.read();
      while(msgc != '\0'){
        cname += msgc;
        msgc = Serial.read();
      }
      name_pub.publish(cname.c_str());
      cname = "";
    }
    else if(msg == 'L'){
      msg = Serial.read();
      ch = msg*100;
      msg = Serial.read();
      ch += msg;
      lighttt_pub.publish(ch);
      ch = 0;
       
      msg = Serial.read();
      hum_pub.publish(msg);
      
      msg = Serial.read();
      ct = msg;
      msg = Serial.read();
      ct += msg/100.00;
      temp_pub.publish(ct);
      ct = 0.0;
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