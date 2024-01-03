void setup() {
  // put your setup code here, to run once:

}

void loop() {#include <ESP32Servo.h>
#include "BluetoothSerial.h"

//Ultrasonico
int trig = 13;
int echo = 12;
long duration;
int distance;
int i;

Servo servo_radar;
int pin_radar = 2; // servo-radar
double t_1 = 0.0;
boolean state = false;


Servo servo_timon;
int pin_timon = 4;
int numeros[] = {49, 50, 51, 52, 53}; //Botones Bluetooth
int direccion_grad[] = {50, 70, 90, 110, 130}; 

// // Motor A
int motorAPin1 = 18; 
int motorAPin2 = 19; 
int enableAPin = 5;

// Motor B
int motorBPin3 = 27; 
int motorBPin4 = 26; 
int enableBPin = 14; 

BluetoothSerial BT; //Objeto Bluetooth
//Condición para habilitar el bluetooth
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

void setup() {
  Serial.begin(9600);
  pinMode(trig, OUTPUT); 
  pinMode(echo, INPUT);
  
  servo_radar.attach(pin_radar);
  servo_timon.attach(pin_timon);

  pinMode(motorAPin1, OUTPUT);
  pinMode(motorAPin2, OUTPUT);
  pinMode(enableAPin, OUTPUT);
  pinMode(motorBPin3, OUTPUT);
  pinMode(motorBPin4, OUTPUT);
  pinMode(enableBPin, OUTPUT);
  
  BT.begin("Lord_Comandante_S"); //Nombre de su dispositivo Bluetooth y en modo esclavo
  Serial.println("El dispositivo Bluetooth está listo para emparejarse");

}
void loop() {

  
for(int i=0;i<=180;i++){          
    servo_radar.write(i);
    //Ahora controlamos el timon con otro servo;v mediante Bluetooth
    if (BT.available()){ //Mientras haya datos por recibir
        Serial.print("Dato recibido: ");
        int val = BT.read();
        for (int j = 0; j <= 5; j++){
          if (val == numeros[j] ) {
            servo_timon.write(direccion_grad[j]);
            }
          else if(val == 54){
            // Hacia adelante
            digitalWrite(enableAPin, HIGH);
            digitalWrite(enableBPin, HIGH);
            Serial.println("Adelante");
            
            digitalWrite(motorAPin1, LOW);
            digitalWrite(motorAPin2, HIGH);
            
            digitalWrite(motorBPin3, LOW);
            digitalWrite(motorBPin4, HIGH);
           }
           else if(val == 55){
            // Freno
            digitalWrite(enableAPin, LOW);
            digitalWrite(enableBPin, LOW);
            Serial.println("Freno");
            digitalWrite(motorAPin1, LOW);
            digitalWrite(motorAPin2, LOW);
            
            digitalWrite(motorBPin3, LOW);
            digitalWrite(motorBPin4, LOW);
           }
            else if(val == 56){
            // Hacia atras
            digitalWrite(enableAPin, HIGH);
            digitalWrite(enableBPin, HIGH);
            Serial.println("Reversa");
            digitalWrite(motorAPin1, HIGH);
            digitalWrite(motorAPin2, LOW);
            
            digitalWrite(motorBPin3, HIGH);
            digitalWrite(motorBPin4, LOW); 
           }
        }
      }
    delay(20);
    distance = lectura_dist();
    Serial.print(i); 
    Serial.print(","); 
    Serial.print(distance);
    Serial.print(".");
  }
   
  for(int i=180;i>0;i--){  
    servo_radar.write(i);
    //Ahora controlamos el timon con otro servo;v mediante Bluetooth
    if (BT.available()){ //Mientras haya datos por recibir
        Serial.print("Dato recibido: ");
        int val = BT.read();
        for (int j = 0; j <= 5; j++){
          if (val == numeros[j] ) {
            servo_timon.write(direccion_grad[j]);
            }
          else if(val == 54){
          // Hacia adelante
            digitalWrite(enableAPin, HIGH);
            digitalWrite(enableBPin, HIGH);
            Serial.println("Adelante");
            
            digitalWrite(motorAPin1, LOW);
            digitalWrite(motorAPin2, HIGH);
            
            digitalWrite(motorBPin3, LOW);
            digitalWrite(motorBPin4, HIGH);
         }
         else if(val == 55){
          // Freno
            digitalWrite(enableAPin, LOW);
            digitalWrite(enableBPin, LOW);
            Serial.println("Freno");
            digitalWrite(motorAPin1, LOW);
            digitalWrite(motorAPin2, LOW);
            
            digitalWrite(motorBPin3, LOW);
            digitalWrite(motorBPin4, LOW);
         }
          else if(val == 56){
          // Hacia atras
            digitalWrite(enableBPin, HIGH);
            Serial.println("Reversa");
            digitalWrite(motorAPin1, HIGH);
            digitalWrite(motorAPin2, LOW);
            
            digitalWrite(motorBPin3, HIGH);
            digitalWrite(motorBPin4, LOW); 
         }
         else if(val == 57){
          state = true;
          Serial.println("Alarma activa");
          BT.print("Alarma encedida");
         }
         else if(val == 58){
          state = false;
          Serial.println("Alarma apagada");
          BT.print("Alarma apagada");
         }
          
       }
    }
    delay(20);
    distance = lectura_dist();
    Serial.print(i);
    Serial.print(",");
    Serial.print(distance);
    Serial.print(".");
    }

  if ((state == true) && (lectura_dist() < 10 & lectura_dist() > 3)){
    Serial.println("Vamoos a chocaaaar");
    Serial.print(lectura_dist());
    Serial.print("cm");
    delay(500);
 } 
    
}


   
int lectura_dist(){
  digitalWrite(trig, LOW); 
  delayMicroseconds(2);
  digitalWrite(trig, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  t_1 = duration/(2*1000000.0); //tiempo de ida en seg
  distance= sqrt(116964 * pow(t_1, 2) - 9 / 62500.0)*100;
  return distance;
}
  // put your main code here, to run repeatedly:

}
