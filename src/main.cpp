// Código para o kit robótica com app Bluetooth

#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial btSerial(2, 3);  // RX, TX do módulo Bluetooth

#define IN1 4   // Motor Esquerdo (sentido)
#define IN2 5   // Motor Esquerdo (sentido)
#define IN3 6   // Motor Direito (sentido)
#define IN4 7   // Motor Direito (sentido)
#define ENA 9   // PWMA
#define ENB 10  // PWMB

unsigned long ultimoComando = 0;
const unsigned long tempoDesconexao = 1500;
bool emMovimento = false;

void moverMotores(bool in1, bool in2, bool in3, bool in4, int ena, int enb, const char* descricao);
void calcularPWMProporcional(int pwmA, int pwmB, int &saidaA, int &saidaB);

void setup() {
    btSerial.begin(9600);  
    Serial.begin(9600);

    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
}

void loop() {
  if (btSerial.available()) {
    String mensagem = btSerial.readStringUntil('\n');
    Serial.println(mensagem);

    ultimoComando = millis();

    int fIndex = mensagem.indexOf('F');
    int tIndex = mensagem.indexOf('T');
    int dIndex = mensagem.indexOf('D');
    int eIndex = mensagem.indexOf('E');

    if (fIndex != -1 && tIndex != -1 && dIndex != -1 && eIndex != -1) {
      int f = mensagem.substring(fIndex + 1, tIndex).toInt();
      int t = mensagem.substring(tIndex + 1, dIndex).toInt();
      int d = mensagem.substring(dIndex + 1, eIndex).toInt();
      int e = mensagem.substring(eIndex + 1).toInt();

      if (f > 0 && d > 0) {
        moverMotores(0, 1, 0, 1, f * 0.4, d, "Frente à Direita");
      } else if (f > 0 && e > 0) {
        moverMotores(0, 1, 1, 0, f, e * 0.4, "Frente à Esquerda");
      } else if (t > 0 && d > 0) {
        moverMotores(1, 0, 1, 0, t * 0.4, d, "Trás à Direita");
      } else if (t > 0 && e > 0) {
        moverMotores(1, 0, 1, 0, t, e * 0.4, "Trás à Esquerda");
      } else if (f > 0) {
        moverMotores(0, 1, 0, 1, f, f, "Frente");
      } else if (t > 0) {
        moverMotores(1, 0, 1, 0, t, t, "Trás");
      } else if (d > 0) {
        moverMotores(1, 0, 0, 1, d * 0.7, d, "Direita");
      } else if (e > 0) {
        moverMotores(0, 1, 1, 0, e, e * 0.7, "Esquerda");
      } else {
        moverMotores(0, 0, 0, 0, 0, 0, "Parado");
      } 
    }
  }
  if (millis() - ultimoComando > tempoDesconexao && emMovimento) {
    moverMotores(0, 0, 0, 0, 0, 0, "Parado por desconexão");
    emMovimento = false;
  }
}

void moverMotores(bool in1, bool in2, bool in3, bool in4, int ena, int enb, const char* descricao) {
  digitalWrite(IN1, in1);
  digitalWrite(IN2, in2);
  digitalWrite(IN3, in3);
  digitalWrite(IN4, in4);
  analogWrite(ENA, ena);
  analogWrite(ENB, enb);

  Serial.print(in1);
  Serial.print(in2);
  Serial.print(in3);
  Serial.print(in4);
  Serial.print(" - ");
  Serial.println(descricao);

  emMovimento = (ena > 0 || enb > 0);
}

// Função para calcular PWM proporcional
void calcularPWMProporcional(int pwmA, int pwmB, int &saidaA, int &saidaB) {
  int soma = pwmA + pwmB;

  if (soma == 0) {
    saidaA = 0;
    saidaB = 0;
    return;
  }

  int maxPWM = 255;
  saidaA = (float)pwmA / soma * maxPWM;
  saidaB = (float)pwmB / soma * maxPWM;
}
