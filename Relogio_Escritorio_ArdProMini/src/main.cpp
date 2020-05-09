#include <Arduino.h>
// http://www.hobbytronics.co.uk/tutorials-code/arduino-tutorials/arduino-4digit-7segment
// http://www.thecustomgeek.com/files/_4_Seg_LED_Clock_2.pde
// Buzzer code https://blogmasterwalkershop.com.br/arduino/como-usar-com-arduino-buzzer-5v-ativo/

#define DISPLAY_BRIGHTNESS  500
#define DIGIT_ON  LOW
#define DIGIT_OFF  HIGH
#define SEGMENT_ON  HIGH
#define SEGMENT_OFF LOW

bool btn_set_va;
bool btn_hor_va; // botão para setar a hora
bool btn_min_va; // botão para setar a hora

int horas_max = 23;
int horas = 0;
int horas_alt = 0;
int min_max = 59;
int minutos = 0;
int min_alt = 0;
int minutos_decorridos = 0;
int segundos = 0;
unsigned long prevtime;

int oscila = 0;

int digit1 = 6; //PWM Display pin 1
int digit2 = 9; //PWM Display pin 2
int digit3 = 10; //PWM Display pin 6
int digit4 = 11; //PWM Display pin 8

//Pin mapping from Arduino to the ATmega DIP28 if you need it
//http://www.arduino.cc/en/Hacking/PinMapping
int segA = A0; //Display pin 14
int segB = A1; //Display pin 16
int segC = A2; //Display pin 13
int segD = A3; //Display pin 3
int segE = A4; //Display pin 5
int segF = A5; //Display pin 11
int segG = 2; //Display pin 15

int ledDiv = 3; // Pontos de divisão Hora Minutos
int led = 13;

int btn_set = 4; // botão para setar a hora
int btn_hora = 5; // botão para setar a hora
int btn_min = 7; // botão para setar a hora

const int pinoBuzzer = 8; //PINO DIGITAL UTILIZADO PELO BUZZER

int contador = 0; //VARIÁVEL DO TIPO INTEIRA
int comp_horas = 0;

int dados_recebidos; // variável que receberá os dados da porta Serial
int hora_recebida = 0;
int minutos_recebidos = 0;

int valor_para_display = 0;

// ---------------------------------------------------------------
void setup() {  

  Serial.begin(9600); 
  delay(2500);  

  pinMode(segA, OUTPUT);
  pinMode(segB, OUTPUT);
  pinMode(segC, OUTPUT);
  pinMode(segD, OUTPUT);
  pinMode(segE, OUTPUT);
  pinMode(segF, OUTPUT);
  pinMode(segG, OUTPUT);
  pinMode(ledDiv, OUTPUT);

  pinMode(digit1, OUTPUT);
  pinMode(digit2, OUTPUT);
  pinMode(digit3, OUTPUT);
  pinMode(digit4, OUTPUT);

  pinMode(led, OUTPUT);
  pinMode(pinoBuzzer, OUTPUT); //DEFINE O PINO COMO SAÍDA

  pinMode(btn_set, INPUT);
  pinMode(btn_hora, INPUT);
  pinMode(btn_min, INPUT);

}
//
// SUB ROTINAS --------------------------------------------------------
// temporização sem usar delay()

// Oscilação ---------------------------------------------------
/*
void oscilador(int tempo){
  digitalWrite(ledDiv, DIGIT_ON);
  delay(tempo / 2);
  digitalWrite(ledDiv, DIGIT_OFF);
  delay(tempo / 2);
}
*/

//
void lightNumber(int numberToDisplay) {
  switch (numberToDisplay){

  case 0:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case 1:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case 2:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 3:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 4:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 5:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 6:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 7:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case 8:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 9:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 10:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    break;
  }
}

void displayNumber(int toDisplay) {
  long beginTime = millis();

  for(int digit = 4 ; digit > 0 ; digit--) {

    //Turn on a digit for a short amount of time
    switch(digit) {
    case 1:
      digitalWrite(digit1, DIGIT_ON);
      break;
    case 2:
      digitalWrite(digit2, DIGIT_ON);
      break;
    case 3:
      digitalWrite(digit3, DIGIT_ON);
      break;
    case 4:
      digitalWrite(digit4, DIGIT_ON);
      break; 
    }

    //Turn on the right segments for this digit
    lightNumber(toDisplay % 10);
    toDisplay /= 10;

    delayMicroseconds(DISPLAY_BRIGHTNESS); 
    //Display digit for fraction of a second (1us to 5000us, 500 is pretty good)

    //Turn off all segments
    lightNumber(10); 

    //Turn off all digits
    digitalWrite(digit1, DIGIT_OFF);
    digitalWrite(digit2, DIGIT_OFF);
    digitalWrite(digit3, DIGIT_OFF);
    digitalWrite(digit4, DIGIT_OFF);

  }

  while( (millis() - beginTime) < 10) ; 
  //Wait for 20ms to pass before we paint the display again
}


//
// FIM - SUB ROTINAS --------------------------------------------------------
void loop() {

  unsigned long time = millis() - (minutos_decorridos * 60000);
  
  segundos = (time / 1000); 
  Serial.print("Serial disponível: ");
  Serial.print(Serial.available());
  Serial.print(" - ");
  Serial.print(segundos);
  Serial.print(" - ");
  Serial.println(valor_para_display);

  if (Serial.available()) {

    dados_recebidos = Serial.read();

    if (dados_recebidos != -1){

      if (dados_recebidos < 24){
        hora_recebida = dados_recebidos;
      } else if(dados_recebidos >= 100){
        minutos_recebidos = dados_recebidos - 100;
      }
      // Serial.print("Horário: ");
      // Serial.print(hora_recebida);
      // Serial.print(":");
      // Serial.println(minutos_recebidos);
    }
    if (hora_recebida != horas){
      horas = hora_recebida;
    }
    if (minutos_recebidos != minutos){
      minutos = minutos_recebidos;
    }
    valor_para_display = (hora_recebida * 100) + minutos_recebidos;

  }else{
    
    if (segundos > min_max) {
      segundos = 0;
      minutos++;
      minutos_decorridos++;
      if (minutos > min_max) {
        minutos = 0;
        horas++;
        if (horas > horas_max) {
          horas = 0;
        }
      }
    }

    valor_para_display = (horas * 100) + minutos;

    // botão alterar horário
    btn_set_va = digitalRead(btn_set); // hour set button
    if (btn_set_va == LOW){
      digitalWrite(ledDiv, HIGH);
      btn_hor_va = digitalRead(btn_hora); // hour set button
      if (btn_hor_va == LOW) {
        horas++;

        if (horas > horas_max) {
          horas = 0;
        }
      }
      btn_min_va = digitalRead(btn_min); // minute set button
      if (btn_min_va == LOW) {
        minutos++;
        if (minutos > min_max) {
          minutos = 0;
        }
      }
      delay(200);
    }
    else{ // if (btn_set_va == LOW){
      if (oscila == 1) {
        digitalWrite(ledDiv, HIGH);
      }
      else {
        digitalWrite(ledDiv, LOW);
      }
    }// if (btn_set_va == LOW){
  } // fim recebimento de dados

  displayNumber(valor_para_display);

  if (time - prevtime > 500) { // toggle a second flag to blink the colon
    prevtime = time;
    if (oscila == 1) {
      oscila = 0;
      return;
    }
    if (oscila == 0) {
      oscila = 1;
      return;
    }
  }
}// fim loop
