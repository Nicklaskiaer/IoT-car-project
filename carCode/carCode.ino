//www.elegoo.com
//2016.09.12

// #include <Wire.h>
// #include <Adafruit_Sensor.h>
// #include <Adafruit_ADXL345_U.h>

int ENA = 10;
int IN1 = 9;
int IN2 = 8;
int ENB = 5;
int IN3 = 7;
int IN4 = 6;
int mode = 0;

// #define RX_PIN 2
const int reflecR = 2;
const int reflectM = 4;
const int reflectL = 11;

const int trigPin = A0;  
const int echoPin = A1;

volatile uint8_t stops[3] = {1, 1, 1};
volatile uint8_t stop = 0;
// volatile uint16_t last_distance = 0;
volatile unsigned long last_time = millis();
// Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();

void setup() {
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  
  // pinMode(RX_PIN, INPUT);  // Set RX pin as input
  
  pinMode(reflecR, INPUT);
  pinMode(reflectM, INPUT);
  pinMode(reflectL, INPUT);

  pinMode(trigPin, OUTPUT);  
	pinMode(echoPin, INPUT);

}


float measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  float duration = pulseIn(echoPin, HIGH);
  float distance = (duration*.0343)/2;

  return distance;
}

// bool calcDeriv(uint16_t distance, unsigned long curr_time) {
// 	uint16_t diff = last_distance - distance;
//   unsigned long diff_time = curr_time - last_time;
//   float res = (float)diff / diff_time;
//   Serial.print("Deriv is: ");
//   Serial.println(res);
// 	return false;
// }

bool checkStopLine() {
  stops[0] = digitalRead(reflecR);
  stops[1] = digitalRead(reflectM);
  stops[2] = digitalRead(reflectL);

  uint8_t curr_stop = stops[0] + stops[1] + stops[2] > 1 ? 1 : 0; 

  if (curr_stop != stop) {
    stop = curr_stop;

    if (!curr_stop)
      return true;
  }

  return false;
}


void loop() {
  
  volatile bool res;
  uint16_t distance;

  while(1) {
    res = checkStopLine();
    distance = measureDistance();
    
    Serial.print("Data:");
    Serial.print(distance);
    Serial.print(",");
    Serial.println(stop);

    if (res) {
      last_time = millis();
      while (millis() - last_time < 5000) {
        distance = measureDistance();
        Serial.print("Data:");
        Serial.print(distance);
        Serial.print(",");
        Serial.println(stop);
      }
      continue;
    }

    if (distance < 20 && distance > 10) {
      continue;
    }

    // if (last_distance != 0) {	
    // bool res = calcDeriv(distance, time);
    //   if (res) {
    // 	  Serial.println("I should stop");
    //   }
    //   else {
    // 	  Serial.println("This is fine");
    //   }
    // }

    if (mode == 0) {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);  //left wheel holds still
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);  // right wheel holds still
    }
    
    if (mode == 1) {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);  // left wheel goes forward
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);  // right wheel goes forward
    }
    
    if (mode == 2) {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);  //left wheel is back up
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);  // right wheel is back up
    }
    
    if (mode == 3) {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);  //left wheel is back up
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);  // right wheel goes forward
    }
    
    if (mode == 4) {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);  // left wheel goes forward
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);  // right wheel is back up
    }
    
    if (Serial.available() > 0) {         // Check if data is available to read
      char receivedChar = Serial.read();  // Read the received character
      if (receivedChar == 'f') {          // Check if received character is '1'
        mode = 1;
      }
      if (receivedChar == 'b') {  // Check if received character is '1'
        mode = 2;
      }
      if (receivedChar == 'l') {  // Check if received character is '1'
        mode = 3;
      }
      if (receivedChar == 'r') {  // Check if received character is '1'
        mode = 4;
      }
      if (receivedChar == 'x') {
        mode = 0;
      }
    }
  }
}

/*
void loop()
{
  digitalWrite(IN1,LOW);      
  digitalWrite(IN2,HIGH);         // left wheel goes forward
  digitalWrite(IN3,LOW);      
  digitalWrite(IN4,HIGH);         // right wheel goes forward
  delay(500);
  digitalWrite(IN1,LOW);      
  digitalWrite(IN2,LOW);         //left wheel holds still
  digitalWrite(IN3,LOW);      
  digitalWrite(IN4,LOW);         // right wheel holds still
  delay(500);
  digitalWrite(IN1,HIGH);      
  digitalWrite(IN2,LOW);        //left wheel is back up
  digitalWrite(IN3,HIGH);      
  digitalWrite(IN4,LOW);        // right wheel is back up
  delay(500);
  digitalWrite(IN1,LOW);      
  digitalWrite(IN2,LOW);         // left wheel holds still
  digitalWrite(IN3,LOW);      
  digitalWrite(IN4,LOW);         // right wheel holds still
  delay(500);
}*/
