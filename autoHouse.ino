#include <stdlib.h>

#include <SoftwareSerial.h>
#define RX 0
#define TX 1
/*
#include "ESP8266.h"
SoftwareSerial minhaSerial(2, 3);

ESP8266 wifi(minhaSerial);*/
SoftwareSerial minhaSerial(RX, TX);
const int PinoLed = 13;
String GET = "";
String cmd;
//Variaveis conecxão
String ssid     = "Alves";  //nome da rede e senha
String password = "07485199633"; 
String apiKey = "KNXEZ7VAJGYAU9X3";
// Thingspeak data
String HOST = "api.thingspeak.com"; 
const int HOST_PORT   = 80;


void setup()
{
/*Serial.begin(115200);
minhaSerial.begin(9600);

minhaSerial.println("AT + CWMODE = 1");
minhaSerial.println("AT + CWJAP = ssid, password");*/
  pinMode(PinoLed, OUTPUT);
//Ativa wifi
  Serial.begin(115200); 
  minhaSerial.begin(9200); 
  delay(3000);
  Serial.println("AT+RST");   
  /*delay(3000);       
  if (!Serial.find("OK"));*/
  
  Serial.println("AT+CWMODE=1");      // Send ChangeMode AT Command
  delay(3000);       
  // Connect to 123D Circuits Simulator Wifi
  Serial.println("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"");
  delay(6000);       
  if (!Serial.find("OK"))
    digitalWrite(PinoLed,HIGH);  
     delay(2500);
     digitalWrite(PinoLed,LOW);
  // Open TCP connection to the host:
  /*Serial.println("AT+CIPSTART=\"TCP\",\"" + HOST + "\"," + HOST_PORT);
  delay(2550);       
  if (Serial.find("OK"));*/
}

void loop()
{


 //Open TCP connection to the host:
  /*Serial.println("AT+CIPSTART=\"TCP\",\"" + HOST + "\"," + HOST_PORT);
  delay(2550);       
  if (Serial.find("OK"))*/

  /*String GET = "GET /channels/527166/fields/1.json?api_key=6V9SNPC0SRPFSMOQ&results=2";
    GET += "\r\n";
    
    Serial.print("AT+CIPSEND=");
    Serial.println(GET.length());
    delay(1000);       
    // Wait send processing
    //Serial.print(GET);
    if(Serial.find(">"))                  // Data ready to send
      Serial.print(GET);
    delay(4000);
      if(Serial.find("SEND OK")){
        //digitalWrite(pinoLedTomada2, HIGH);
        //delay(1500);
    //digitalWrite(pinoLedTomada2, LOW);*/
    
    //String GET = "GET /update?api_key=KNXEZ7VAJGYAU9X3&field1=1000";
    //GET += "\r\n";

    cmd = "AT+CIPSTART=\"TCP\",\"";
    cmd += "api.thingspeak.com"; // api.thingspeak.com
    cmd += "\",80";
    Serial.println(cmd);

    if(minhaSerial.find("Error")){
      Serial.println("AT+CIPSTART error");
      return;
    }
    
    // prepare GET string
    String getStr = "GET /update?api_key=";
    getStr += apiKey;
    getStr +="&field1=";
    getStr +="-250";
    getStr += "\r\n\r\n";
  
    // send data length
    cmd = "AT+CIPSEND=";
    cmd += getStr.length();
    Serial.println(cmd);
    delay(2500);
    Serial.print(getStr);
    if(minhaSerial.find(">")){
      Serial.print(getStr);
    }
    else{
      minhaSerial.println("AT+CIPCLOSE");
      // alert user
      Serial.println("AT+CIPCLOSE");
    }
  digitalWrite(PinoLed,HIGH);
    delay(30000);
    digitalWrite(PinoLed,LOW);
    /*Serial.print("AT+CIPSEND=");
    Serial.println(GET.length());
    delay(1000);       
    // Wait send processing
    Serial.print(GET);
    if(Serial.find(">"))                  // Data ready to send
    Serial.print(GET);
    delay(4000);
      if(Serial.find("SEND OK")){
        digitalWrite(PinoLed, HIGH);
        delay(1500);
    }else{digitalWrite(PinoLed, LOW);}*/


}
