/*
  Tópicos Especiais - IoT - Formandos: Ariel Vieira 
  
   IoT - Sistema do Banheiro

  Conecta-se ao canal banheiro na plataforma Thingspeak, e recebe o ultimo valor de lampada postado na plataforma Tweeter, e conta com um sistema de alarme 
  a partir das 11 que envia um email ao proprietário.
  ESP8266 conectado a porta serial.
*/

// Utiliza-se a biblioteca SoftwareSerial para criar uma segunda porta serial, para fins de debug do código
#include <SoftwareSerial.h>

#define ESP_SPEED 115200                // Velocidade da placa ESP8266

// Dados a serem acessados
#define HOST "api.thingspeak.com"       // URL da plataforma thingspeak.com
#define HOST_PORT 80                    // Porta do Webservice
//#define HOST2 "iotts.herokuapp.com"     //URL da plataforma de email

//variaveis do meu sistema
String cmd;
int lamp;
const int PinoLed = 13;                 // lampada controlada a distancia
int valueSensor;                        //valor do sensor de presença lido
int sensor;
SoftwareSerial debugMonitor(0,1);     // Interface para Debug - RX, TX

String NET_SSID = "Alves";  // SSID da rede WiFi
String NET_PSWD = "07485199633"; // Senha da rede WiFi

// A função setup é executada uma única vez ao pressionar reset ou energizar o controlador
void setup() {
  Serial.begin(ESP_SPEED);              // Inicializa a porta serial na velocidade da placa ESP8266
  debugMonitor.begin(ESP_SPEED);             // Inicializa a porta de debug na velocidade de 9600 Baud
  SetupESP8266();                       // Esta função reinicializa o módulo ESP, muda para modo estação e conecta ao AP
  pinMode(PinoLed, OUTPUT);
}

// função loop 
void loop() {
  redeIn();
  lamp = onOffLamp();
  if(lamp == 1){
    digitalWrite(PinoLed,HIGH);
  }else{
    digitalWrite(PinoLed,LOW);
  }
  
  Serial.println("vai começar o banheiro");
  
  //sensor = presenceSensor();
  //delay(3000);
  
  //redeEmail();
  //sendMail(sensor);
  
  // Tempo de espera para atualizar os valores
  delay(6000);
} 

void SetupESP8266(){
  // Verifica a comunicação com o módulo ESP8266
  Serial.println("Verificando ESP8266");
  Serial.println("AT");               // Envia comando AT - "OK" é a resposta esperada
  delay(300);                         // Espera pela resposta
  if (Serial.find("OK"))
    Serial.println("Resposta OK recebida");
  else
    Serial.println("Falha de comunicação com o módulo ESP8266");
  
  // Muda o modo de operação do ESP8266
  Serial.println("Alterando o modo do ESP8266 para Estação");
  Serial.println("AT+CWMODE=1");      // Envia o comando AT para mudança de modo
  delay(300);
  if(Serial.find("OK"))
    Serial.println("Modo de operação ESP8266 alterado para Estação");
  else
    if(Serial.find("NO CHANGE"))
      Serial.println("ESP8266 já está em modo Estação");
    else
      Serial.println("Erro ao mudar modo de operação do ESP8266");

  // Connecta à rede WiFi
  Serial.print("Conectando à rede WiFi ");
  Serial.println(NET_SSID);
  
  // Monta o comando AT para conectar à rede
  String cmd = "AT+CWJAP=\"";
  cmd += NET_SSID;
  cmd += "\",\"";
  cmd += NET_PSWD;
  cmd += "\"";
  Serial.println(cmd);              // Envia o comando para conexão
  delay(5000);                      // Espera a operação ser realizada
  if(Serial.find("OK"))
    debugMonitor.println("Conectado ao WiFi");
  else
    debugMonitor.println("Erro durante a conexão ao WiFi");
  
  // Verifica o IP do dispositivo
  Serial.println("AT+CIFSR");
  String response = Serial.readString(); 
  response = response.substring(response.indexOf('"')+1,response.indexOf('"')+16); 
  response = response.substring(0,response.indexOf('"'));
  Serial.print("O IP do dispositivo é ");
  Serial.println(response);
}

//função que conecta a rede
void redeIn(){
  // Abre a conexão TCP
  debugMonitor.println("Iniciando conexão TCP");
  
  // Monta a string com o comando e inicializa a conexão
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += HOST;
  cmd += "\",80";
  Serial.println(cmd);
  if (Serial.find("CONNECT")){
    if(Serial.find("OK"))
      Serial.println("Conexão TCP estabelecida redein");
  }
  else
    Serial.println("Falha na conexão TCP redein");
}
void redeEmail(){
  // Abre a conexão TCP
  Serial.println("Iniciando conexão TCP rerokku");
  
  // Monta a string com o comando e inicializa a conexão
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  //cmd += HOST2;
  cmd += "\",80";
  delay(800);
  Serial.println(cmd);
  if (Serial.find("CONNECT")){
    if(Serial.find("OK"))
      Serial.println("Conexão TCP estabelecida heroku");
  }
  else
    Serial.println("Falha na conexão TCP heroku");

}

//função que verifica se é para ligar/desligar a lampada
int onOffLamp(){
  // Envia requisição para receber os dados
  String GET = "GET /channels/";
  GET += "530528";
  GET += "/fields/1/last.txt?api_key=IYDI776EEOFLDMSO \r\n";

  // Envia comando AT para transmissão de dados
  Serial.print("AT+CIPSEND=");
  Serial.println(GET.length());
  delay(100);                           // aguarda o processamento
  if(Serial.find(">")){                  // Dados prontos para o envio
    Serial.print(GET);
  }
  delay(3000);
  if(Serial.find("SEND OK"))
    Serial.println("Requisição enviada com sucesso");
  else 
    Serial.println("Falha na comunicação com o servidor");
 
  // Recebe os dados e os salva
  Serial.find(":");                     // Lê o buffer serial até encontrar um ':' 
  String response = Serial.readString();  
  response = response.substring(0,response.indexOf("CLOSE")); // remove os dados restantes
  Serial.print("Dado recebido: ");
  Serial.println(response);
  return response.toInt();
}

//função que envia se tem presença dentro do banheiro para o thinker e para a api de email
int presenceSensor(){
  //ler valor do sensor quando comprar
  valueSensor = 1;
   // prepare GET string;
    String getStrThinker = "GET /update?api_key=";
    getStrThinker += "GV6RG2GWOMCQCB5O";
    getStrThinker +="&field1=";
    getStrThinker += valueSensor;
    getStrThinker += "\r\n\r\n";

    // send data length
    cmd = "AT+CIPSEND=";
    cmd += getStrThinker.length();
    Serial.println(cmd);
    delay(2500);
    //Serial.print(getStrThinker);
    if(Serial.find(">")){
      Serial.print(getStrThinker);
    }
    else{
      //Serial.println("AT+CIPCLOSE");
      // alert user
      //Serial.println("AT+CIPCLOSE");
    }
    return valueSensor;
}
void sendMail(int value){
  if(value == 1){
      //preparando a string de email
      String getStrEmail = "GET / HTTP/1.1";
      // send data length
      cmd = "AT+CIPSEND=";
      cmd += getStrEmail.length();
      Serial.println(cmd);
      delay(3000);
      //Serial.println(getStrEmail);
      if(Serial.find(">")){
        Serial.println(getStrEmail);
        //Serial.println("agora");
      }
      else{
        Serial.println("Quem sabe da próxima");
        //Serial.println("AT+CIPCLOSE");
        // alert user
        //Serial.println("AT+CIPCLOSE");
      }
      
      Serial.find(":");                     // Lê o buffer serial até encontrar um ':' 
      String response = Serial.readString();  
      response = response.substring(0,response.indexOf("CLOSE")); // remove os dados restantes
      Serial.print("Dado recebido: ");
      Serial.println(response);  
    }
}
