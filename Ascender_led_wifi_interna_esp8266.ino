#include <ESP8266WiFi.h> //inclusao da biblioteca wifi


const char* ssid = "SUA_REDE_WIFI"; //definicao de uma constante para ID da rede wifi
const char* password = "SUA_SENHA"; //definicao de uma constante para senha da rede

WiFiServer server(80); //porta de comunicacao (normalmente se utiliza a 80 ou 8080)

void setup() {
  Serial.begin(115200); //para o node mcu esp8266 a velocidade do monitor serial e 115200
  delay(10);

  // prepare GPIO7 (olhe o mapa de portas para entender)
  pinMode(13, OUTPUT); //porta D7 = 13. Define a porta 13 como saida
  digitalWrite(13, 0); //escreve 0 na porta 13 (apaga o led)

  pinMode(5, OUTPUT); //porta D1 = 5. Define a porta 5 como saida
  digitalWrite(5, 0); //escreve 0 na porta 5 (apaga o led)

  // mostra no monitor serial informacoes de conexao da rede
  Serial.println();
  Serial.println();
  Serial.print("conectando em ");
  Serial.println(ssid);

  WiFi.begin(ssid, password); //funcao que inicializa a conexao

  while (WiFi.status() != WL_CONNECTED) { //enquanto nao conseguir conectar
    delay(500);
    Serial.print("."); //imprime um ponto na tela (da ideia de que esta carregando)
  }

  Serial.println("");
  Serial.println("WiFi connectado"); //imprime wi fi conectado

  // inicializa o servidor (nesse caso o proprio esp8266)
  server.begin();
  Serial.println("Servidor inicializado");
  Serial.println(WiFi.localIP()); //mostra o IP do servidor
}

void loop() {
  WiFiClient client = server.available(); //guardar o status do servidor
  if (!client) { //se nao tiver cliente ja finaliza um ciclo do loop
    return;
  }

  Serial.println("novo cliente"); //quando tiver alguem acessando
  while(!client.available()){ //enquanto nao tiver cliente
    delay(1); //aguarda
  }
//comeca a escrever o codigo HTML para visualizacao do cliente:
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  String buf = "";
 
  buf += "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n";
  buf += "<html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/>\r\n";
  buf += "<title>ESP8266 Web Server</title>";
  buf += "<style>.c{text-align: center;} div,input{padding:5px;font-size:1em;} input{width:80%;} body{text-align: center;font-family:verdana;} button{border:0;border-radius:0.3rem;background-color:#1fa3ec;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;} .q{float: right;width: 64px;text-align: right;}</style>";
  buf += "</head>";
  buf += "<h3>ESP8266 Web Server - System Logs</h3>";
  buf += "<p>GPIO4 <a href=\"?function=led4_on\"><button>ON</button></a> <a href=\"?function=led4_off\"><button>OFF</button></a></p>";
  buf += "<p>GPIO5 <a href=\"?function=led5_on\"><button>ON</button></a> <a href=\"?function=led5_off\"><button>OFF</button></a></p>";
  buf += "<h4>Rafael Levi</h4>";
  buf += "</html>\n";

  client.print(buf);
  client.flush();
//Verifica se o cliente apertou o botao de ligar ou desligar algum led
  if (req.indexOf("led5_on") != -1) //se clicou em ligar led 1
    digitalWrite(5, 1);
  else if (req.indexOf("led5_off") != -1) //se clicou em desligar led1
    digitalWrite(5, 0);
  else if (req.indexOf("led4_on") != -1)//se clicou em ligar led 2
    digitalWrite(13, 1);
  else if (req.indexOf("led4_off") != -1)//se clicou em desligar led2
    digitalWrite(13, 0);
  else { //senao
    Serial.println("requisicao invalida"); 
    client.stop();
  }
  Serial.println("Cliente desconectado");
}
