//www.elegoo.com
//2016.09.12
int ENA = 10;
int IN1 = 9;
int IN2 = 8;
int ENB = 5;
int IN3 = 7;
int IN4 = 6;
int mode = 0;
#define RX_PIN 2
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
  pinMode(RX_PIN, INPUT);  // Set RX pin as input
}

void loop() {
  delay(50);
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
