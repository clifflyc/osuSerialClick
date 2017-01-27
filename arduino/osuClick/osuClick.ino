const byte INPUT_A = 8, INPUT_B = 9; //pin numbers for two input buttons
//const byte LED_R = 13, LED_G = 12, LED_B = 11; //pin numbers for RGB led
bool pressedA = false, pressedB = false; //is button A/B currently pressed down already?
bool pressingA, pressingB; //is pressing button A/B currently?

void setup() {
  Serial.begin(9600);
  pinMode(INPUT_A, INPUT_PULLUP);
  pinMode(INPUT_B, INPUT_PULLUP);
  //pinMode(LED_R,OUTPUT);
  //pinMode(LED_G,OUTPUT);
  //pinMode(LED_B,OUTPUT);
}

void loop() {
  bool pressingA = !digitalRead(INPUT_A);
  bool pressingB = !digitalRead(INPUT_B);

  if (pressingA && !pressedA) { //button A key down
    pressedA = true;
    Serial.write("z");
  } else if (!pressingA && pressedA) { //button A key up
    pressedA = false;
    Serial.write("a");
  }
  if (pressingB && !pressedB) { //button B key down
    pressedB = true;
    Serial.write("x");
  } else if (!pressingB && pressedB) { //button B key up
    pressedB = false;
    Serial.write("s");
  }
  delay(1);
}
