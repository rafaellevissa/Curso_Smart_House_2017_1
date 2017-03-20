#define led 13 //define a constante led como valor 13

// A funcao setup roda uma vez quando inicializado o arduino
void setup() {
  //inicializa o pino digital como saida (representado por uma constante)
  pinMode(led, OUTPUT);
}
// a funcao loop roda infinitamente
void loop() {
  digitalWrite(led, HIGH);   // liga o led
  delay(1000);               // aguarda 1 segundo (1000 ms)
  digitalWrite(led, LOW);    // desliga o led
  delay(1000);               // aguarda 1 segundo (1000 ms)
}
