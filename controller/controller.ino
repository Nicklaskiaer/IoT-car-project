
#define VRX_PIN  2 // ESP32 pin GPIO36 (ADC0) connected to VRX pin
#define VRY_PIN  4 // ESP32 pin GPIO39 (ADC0) connected to VRY pin

int valueX = 0; // to store the X-axis value
int valueY = 0; // to store the Y-axis value
char direction;

char getCharDirection(int x, int y){
  if (x > 2000 && (y < 3000 && y > 1000)){
    return 'f';
  }
  if (x < 1000 && (y < 3000 && y > 1000)){
    return 'b';
  }
  if (y < 1100 && (x < 2000 && x > 1000)){
    return 'r';
  }
  if (y > 2500 && (x < 2000 && x > 1000)){
    return 'l';
  }
  return 'X';
}

bool sendDirectionOnESPNow(char direction){

}


void setup() {
  Serial.begin(9600) ;
}

void loop() {
  // read X and Y analog values
  valueX = analogRead(VRX_PIN);
  valueY = analogRead(VRY_PIN);

  direction = getCharDirection(valueX, valueY);
  Serial.println(direction);

  sendDirectionOnESPNow(direction);


  delay(200);
}
