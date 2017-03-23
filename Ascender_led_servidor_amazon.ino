#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h> 


const char* SSID          = "SEU ID DA REDE";
const char* PASSWORD      = "SUA SENHA DA REDE";


const char* MQTT_SERVER   = "52.54.175.171";
const int MQTT_PORT       =  1883;
const char* MQTT_USER     = "levi";
const char* MQTT_PASSWORD = "@levi2017";
const char* MQTT_CLIENT   = "ESP8266-Client";

const char* TOPICO_ESCUTA  = "topicoEscuta";
const char* TOPICO_ENVIO   = "topicoEnvio";

const int ACIONAMENTO_LED = 1000;

// GPIO 5 (D1)
const int PIN_LED_1 = 13;
// GPIO 13 (D7)
const int PIN_LED_2 = 15;

String leitura;
char l[3]; 

WiFiClient espClient;
PubSubClient MQTT(espClient);


void acionaLed() {
   digitalWrite(PIN_LED_1, LOW);
   digitalWrite(PIN_LED_2, LOW);
   delay(ACIONAMENTO_LED);
   digitalWrite(PIN_LED_1, HIGH);
   delay(ACIONAMENTO_LED);
   digitalWrite(PIN_LED_1, LOW);


   digitalWrite(PIN_LED_2, HIGH);
   delay(ACIONAMENTO_LED);
   digitalWrite(PIN_LED_2, LOW);
   delay(ACIONAMENTO_LED);
   digitalWrite(PIN_LED_2, HIGH);
   delay(ACIONAMENTO_LED);
   digitalWrite(PIN_LED_2, LOW);
   delay(ACIONAMENTO_LED);
   digitalWrite(PIN_LED_2, HIGH);
   delay(ACIONAMENTO_LED*3);
   digitalWrite(PIN_LED_2, LOW);
   delay(ACIONAMENTO_LED);
   
}

void reconnectWiFi() {
  if (WiFi.status() == WL_CONNECTED)
     return;
     
  Serial.println("Conectando-se na rede: " + String(SSID));
  Serial.println("Aguarde");
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Endereco IP: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  String msg;

  for(int i = 0; i < length; i++) 
  {
     char c = (char)payload[i];
     msg += c;
  }
  
  Serial.print("Mensagem [");
  Serial.print(topic);
  Serial.print("]: " + msg);
  Serial.println();

  if (msg.equals("1")) {    
    acionaLed();
  }
}

void initMQTT() {
  MQTT.setServer(MQTT_SERVER, MQTT_PORT);
  MQTT.setCallback(callback);
}


void reconnectMQTT() {
   while (!MQTT.connected()) {
     // Serial.print("Tentado conectar ao broker MQTT: " + String(MQTT_SERVER) + ":" + String(MQTT_PORT) + " :");
     if (MQTT.connect(MQTT_CLIENT, MQTT_USER, MQTT_PASSWORD)) {
       // Serial.println("conectado");
       // MQTT.publish(TOPICO_ENVIO, "Cliente conectado ao broker");
       MQTT.subscribe(TOPICO_ESCUTA);
     } 
     else {
       Serial.print("falha, rc=");
       Serial.print(MQTT.state());
       Serial.println(" tenta novamente após 5 segundos");
       delay(5000);
    }
  }
}

void setup() {
  String id;
  Serial.begin(115200);
  pinMode(PIN_LED_1, OUTPUT);
  pinMode(PIN_LED_2, OUTPUT);
  digitalWrite(PIN_LED_1,LOW);
  digitalWrite(PIN_LED_2,LOW);
  reconnectWiFi();
  initMQTT();
  
}


void loop() {
  
  reconnectWiFi();

  if (!MQTT.connected()) {
    reconnectMQTT();
  } 
  
  MQTT.loop();
  while (Serial.available()>0) {
    Serial.println(F("======================================================")); 
    Serial.println(F("Digite seu ID (3 digitos):"));
    leitura = Serial.readString();
    if (leitura != ""){
      Serial.print("id: ");
      leitura.toCharArray(l,4);
      Serial.print(l[0]);
      Serial.print(l[1]);
      Serial.print(l[2]);
      Serial.println();
      MQTT.publish(TOPICO_ENVIO, l);
    }
  }
  
  

}
