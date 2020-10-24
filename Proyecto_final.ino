// Librerias
#include <IRremote.h>
#define Boton_1 0xFD00FF
#define Boton_2 0xFD8877
#define Boton_3 0xFD48B7

// Declaración de pin de entrada y salida
const int botonBaul = 9;
const int botonPuerta = 10;
const int botonCapot = 11;
const int buzzer = 13;
const int led = 12;
const int ledRojo = 5;
const int ledAzul = 4;
const int ledVerde = 3;
const int sensorIR = 8;  // Pin de sensor
int trig = 7;
int eco = 6;
int distancia;
int duracion;

// Declaro variables
const int tiempoAntirebote = 10;
int activarAlarma = false;
int estadoAlarma = LOW;
int estadobotonBaul;
int estadobotonPuerta;
int estadobotonCapot;
int estadoBotonAnteriorBaul;
int estadoBotonAnteriorPuerta;
int estadoBotonAnteriorCapot;

// Codigo para sensor IR
IRrecv irrecv(sensorIR);  // Creamos un objeto para recibir inflarojo
decode_results codigo;  // Creamos otro objeto 


boolean antirebote(int pin) {
  
  int contador = 0;
  boolean estado;
  boolean estadoAnterior;
  
  do {
    estado = digitalRead(pin);
    if(estado != estadoAnterior) {
      contador = 0;
      estadoAnterior = estado;
    }
    else {
      contador = contador + 1;
    }
    delay(1);
  } while(contador < tiempoAntirebote);
  
  return estado;
}

void alarmaActiva(){
  estadobotonBaul = digitalRead(botonBaul);
  if(estadobotonBaul !=estadoBotonAnteriorBaul) 
  {
    if(antirebote(botonBaul)) {
      estadoAlarma = HIGH;
      Serial.println("Baul Abierto");
    }
  }
  estadoBotonAnteriorBaul = estadobotonBaul;
  
  estadobotonPuerta = digitalRead(botonPuerta);
  if(estadobotonPuerta !=estadoBotonAnteriorPuerta) {
    if(antirebote(botonPuerta)) {
      estadoAlarma = HIGH;
      Serial.println("Puerta Abierto");
      }
    }
  estadoBotonAnteriorPuerta = estadobotonPuerta;
  
  estadobotonCapot = digitalRead(botonCapot);
  if(estadobotonCapot !=estadoBotonAnteriorCapot) {
    if(antirebote(botonCapot)) {
      estadoAlarma = HIGH;
      Serial.println("Capot Abierto");
      }
    }
  estadoBotonAnteriorCapot = estadobotonCapot;
}

void encenderColorRojo(){
  digitalWrite(ledRojo, HIGH); // Ensendemos led Rojo
  digitalWrite(ledAzul, LOW); // Apagamos led Rojo
  digitalWrite(ledVerde, LOW); // Apagamos led Rojo
}

void encenderColorAzul(){
  digitalWrite(ledRojo, LOW); // Apagamos led Rojo
  digitalWrite(ledAzul, HIGH); // Ensendemos led Rojo
  digitalWrite(ledVerde, LOW); // Apagamos led Rojo

}

void encenderColorVerde(){
  digitalWrite(ledRojo, LOW); // Apagamos led Rojo
  digitalWrite(ledAzul, LOW); // Apagamos led Rojo
  digitalWrite(ledVerde, HIGH); // Ensendemos led Rojo

}

void setup() {
  // put your setup code here, to run once:
  pinMode(botonBaul, INPUT);  
  pinMode(botonPuerta, INPUT);
  pinMode(botonCapot, INPUT);
  pinMode(led, OUTPUT);
  pinMode(ledRojo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAzul, OUTPUT);
  pinMode(buzzer, OUTPUT);  // Delcaramos al zumbador como salida
  pinMode(trig, OUTPUT);
  pinMode(eco, INPUT);
  Serial.begin(9600);  // Iniciamos el monitor serial
  irrecv.enableIRIn();  // Inicializamos la comunicación con el modulo
}

void loop() {
  
  // CODIGO DE SENSOR IR
  if(irrecv.decode(&codigo)){
    // Mostramos el codigo recibido, HEX define que el valor sera hexadecimal
    Serial.println(codigo.value, HEX);  
    if(codigo.value == Boton_1){
      activarAlarma = !activarAlarma;
      estadoAlarma = LOW;
    }
    irrecv.resume();  //  Sirve para que este listo el objeto para recibir otro codigo
  }

  if(activarAlarma == true){
    alarmaActiva();
    Serial.println(activarAlarma); 
  }
  

  if (estadoAlarma == HIGH)
  {  
    encenderColorRojo();
    digitalWrite(buzzer, estadoAlarma); // Ensendemos zumbador
    digitalWrite(led, estadoAlarma); // Ensendemos zumbador
    delay(500);  // Esperamos 500ms
    digitalWrite(buzzer, LOW); // Apagamos zumbador
    digitalWrite(led, LOW); // Apagamos led
  }
  else
  {
    encenderColorVerde();
    digitalWrite(buzzer, LOW); // Ensendemos zumbador
    digitalWrite(led, LOW); // Ensendemos zumbador
  }
  
}
  
