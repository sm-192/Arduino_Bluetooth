// Código para o kit robótica com app Bluetooth

#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial btSerial(2, 3);  // RX, TX do módulo Bluetooth

void processaMensagem(int f, int t, int d, int e) {
    Serial.print("Frente: "); Serial.print(f);
    Serial.print(" | Trás: "); Serial.print(t);
    Serial.print(" | Direita: "); Serial.print(d);
    Serial.print(" | Esquerda: "); Serial.println(e);

    // Aqui você pode adicionar o controle dos motores usando os valores f, t, d, e
}

void setup() {
    btSerial.begin(9600);  
    Serial.begin(9600);
}

void loop() {
    if (btSerial.available()) {
        String mensagem = btSerial.readStringUntil('\n');  // Lê a mensagem inteira até a quebra de linha

        // Verifica se a mensagem contém os prefixos esperados
        int fIndex = mensagem.indexOf('F');
        int tIndex = mensagem.indexOf('T');
        int dIndex = mensagem.indexOf('D');
        int eIndex = mensagem.indexOf('E');

        if (fIndex != -1 && tIndex != -1 && dIndex != -1 && eIndex != -1) {
            int f = mensagem.substring(fIndex + 1, tIndex).toInt();
            int t = mensagem.substring(tIndex + 1, dIndex).toInt();
            int d = mensagem.substring(dIndex + 1, eIndex).toInt();
            int e = mensagem.substring(eIndex + 1).toInt();

            processaMensagem(f, t, d, e);
        } else {
            Serial.println("Erro: Formato da mensagem inválido!");
        }
    }
}