//Librerias
#include <SoftwareSerial.h>


//Definiciones
#define Pir 4
#define Sensor A0
#define Umbral 250
#define Blanco 5
#define Color 6
#define Delay 50
#define Timeout 5000

#define Debug

//Variables
long Tiempo = 0;
int ValorSensor = 0;


//Constructores
SoftwareSerial RS485(7 , 8); // Rx-Tx para la comunicacion con el tacho grande.

void setup() {
#ifdef Debug
  Serial.begin(115200);
  Serial.println("Iniciando");
#endif
  RS485.begin(115200);
  pinMode(Pir , INPUT);
  pinMode(Blanco , OUTPUT);
  pinMode(Color , OUTPUT);
}

void loop() {
#ifdef Debug
  Serial.println("En el loop");
#endif


  RS485.println(0);
  if (digitalRead(Pir) == 0) {
    int PWM = 0;
    int SubeBaja = 0;
    while (digitalRead(Pir) == 0) {
      if (SubeBaja == 0) PWM++;
      if (SubeBaja == 1) PWM--;
      analogWrite(Blanco, PWM);
      delay(Delay);
      if (PWM == 255) SubeBaja = 1;
      if (PWM == 0) SubeBaja = 0;
#ifdef Debug
      Serial.println("En el while pir 0");
#endif

    }
    digitalWrite(Blanco, LOW);
  }
  if (digitalRead(Pir) == 1) {
    digitalWrite(Color, HIGH);
    Tiempo = millis();
    while (millis() - Tiempo < Timeout) {
      ValorSensor = analogRead(Sensor);
#ifdef Debug
      //Serial.println(ValorSensor);
#endif
      if (ValorSensor > Umbral) {
        Tiempo = millis();
        int i = map (ValorSensor, Umbral, 600 , 1 , 23);
        RS485.println(i);
#ifdef Debug
        Serial.println(ValorSensor);
        //Serial.println(i);
#endif
      }
    }
  }
  digitalWrite(Color, LOW);
}
