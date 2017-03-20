 

char leitura;

#define rele 8

void setup() {
  //Inicializa comunicação Serial
  Serial.begin(9600);
  //Seta o pino indicado por rele como saída
  pinMode(rele, OUTPUT);
  //Mantem rele desligado assim que iniciar o programa
  digitalWrite(rele,LOW);
}
void loop() {
  //Verifica se há conexão com a serial
  while (Serial.available() > 0) {
    //Lê o dado vindo da Serial e armazena na variável leitura
    leitura = Serial.read();
    //Se a variável leitura for igual a 'd' ou 'D' ela Desliga rele
    if (leitura == 'd' || leitura =='D'){// As duas || é a operação booleana OU
      digitalWrite(rele,LOW);
    }
    /*Senão verifica se a variável leitura é
     igual a 'l' ou 'L' ela Liga rele */
    else if (leitura == 'l' || leitura =='L'){
      digitalWrite(rele,HIGH);
      delay  (1000);
  }
    Serial.println(leitura);
  }
}
