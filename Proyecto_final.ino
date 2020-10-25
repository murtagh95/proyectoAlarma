// Librerias
#include <IRremote.h>
#define Boton_1 0xFD00FF

// Declaración de pin de entrada y salida
const int buzzer = 13;
const int led = 12;
const int botonCapot = 11;
const int botonPuerta = 10;
const int botonBaul = 9;
const int sensorIR = 8;  // Pin de sensor
int trig = 7;
int eco = 6;
const int ledRojo = 5;
const int ledAzul = 4;
const int ledVerde = 3;

// Declaro variables
int distancia;
int duracion;
int contadorDuracionAlarma = 0;
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

void controlBotones(){
  // Boton Baul
  estadobotonBaul = digitalRead(botonBaul);
  if(estadobotonBaul !=estadoBotonAnteriorBaul) 
  {
    if(antirebote(botonBaul)) {
      estadoAlarma = HIGH;
      Serial.println("Baul Abierto");
    }
  }
  estadoBotonAnteriorBaul = estadobotonBaul;
  
  // Boton Puertas
  estadobotonPuerta = digitalRead(botonPuerta);
  if(estadobotonPuerta !=estadoBotonAnteriorPuerta) {
    if(antirebote(botonPuerta)) {
      estadoAlarma = HIGH;
      Serial.println("Puerta Abierto");
      }
    }
  estadoBotonAnteriorPuerta = estadobotonPuerta;

  // Boton Capot
  estadobotonCapot = digitalRead(botonCapot);
  if(estadobotonCapot !=estadoBotonAnteriorCapot) {
    if(antirebote(botonCapot)) {
      estadoAlarma = HIGH;
      Serial.println("Capot Abierto");
      }
    }
  estadoBotonAnteriorCapot = estadobotonCapot;
}

void controlUltrasonido(){
  // Largamos un pulso en el sonsor SR04
  digitalWrite(trig, HIGH);
  delay(1);
  digitalWrite(trig, LOW);
  
  duracion = pulseIn(eco, HIGH);  // Leemos la duración del pulso
  distancia = duracion / 58.2;  // Transformamos la duración en centimetros
  Serial.println(distancia);  // Mandamos al monitor serial la distancia

  if(distancia < 30 && distancia >= 0){  // Si la distancia es menos a 30cm entonces:
    estadoAlarma = HIGH;
    Serial.println("Activacion por sensor Ultrasonido");
  }
}

void alarmaActiva(){
  controlBotones();
  controlUltrasonido();
}

void encenderColorRojo(){
  digitalWrite(ledRojo, HIGH); // Ensendemos led Rojo
  digitalWrite(ledAzul, LOW); // Apagamos led Azul
  digitalWrite(ledVerde, LOW); // Apagamos led Verde
}

void encenderColorAzul(){
  digitalWrite(ledRojo, LOW); // Apagamos led Rojo
  digitalWrite(ledAzul, HIGH); // Ensendemos led Azul
  digitalWrite(ledVerde, LOW); // Apagamos led Verde

}

void encenderColorVerde(){
  digitalWrite(ledRojo, LOW); // Apagamos led Rojo
  digitalWrite(ledAzul, LOW); // Apagamos led Azul
  digitalWrite(ledVerde, HIGH); // Ensendemos led Verde

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
    if(codigo.value == Boton_1){  // Si precionamos el primer boton
      encenderColorRojo();
      activarAlarma = !activarAlarma;
      estadoAlarma = LOW;
    }
    irrecv.resume();  //  Sirve para que este listo el objeto para recibir otro codigo
  }

  // Verificamos si la alarma esta prendida poniendo los sensores a la escucha
  if(activarAlarma == true){
    alarmaActiva();
  }
  // Verificamos si la alarma esta apagada
  else{
  	encenderColorVerde();
    contadorDuracionAlarma = 0;
  }
  

  if (estadoAlarma == HIGH)
  {  
    if (contadorDuracionAlarma < 10)
    {
      digitalWrite(buzzer, estadoAlarma); // Ensendemos zumbador
      digitalWrite(led, estadoAlarma); // Ensendemos led
      delay(500);  // Esperamos 500ms
      digitalWrite(buzzer, LOW); // Apagamos zumbador
      digitalWrite(led, LOW); // Apagamos led
      delay(500);  // Esperamos 500ms
      
      // Incremento el contador para que la alarma no suene infinitamente 
      contadorDuracionAlarma += 1;
      Serial.println(contadorDuracionAlarma); 
    }
    // Si la alarma sono demaciado tiempo la apago
    else
    {
      estadoAlarma = LOW;
      contadorDuracionAlarma = 0;
    }
    
    
  }
  else  // Si la alarma no esta activa apagamos los actuadores de aviso
  {
    digitalWrite(buzzer, LOW); // Apagamos zumbador
    digitalWrite(led, LOW); // Apagamos led
  }
  
}
  
