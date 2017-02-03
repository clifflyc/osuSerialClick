const byte INPUT_A = 8, INPUT_B = 9; //pin numbers for two input buttons
const byte LED_PINS [3] = {12, 11, 10}; //pin numbers for RGB led = {R,G,B}
const float LED_ALPHA = 20, LED_FADE_SPEED = 0.2; //led strength and fade out speed
bool pressedA = false, pressedB = false; //is button A/B currently pressed down already?
float currentAlpha [3] = {0, 0, 0}; //used to store current alpha amount for led rgb pins //{R,G,B}


void setup() {
  Serial.begin(9600);
  pinMode(INPUT_A, INPUT_PULLUP);
  pinMode(INPUT_B, INPUT_PULLUP);
  for (int i = 0; i < 3; i++) {
    pinMode(LED_PINS[i], HIGH);
  }
}

void loop() {
  bool pressingA = !digitalRead(INPUT_A);
  bool pressingB = !digitalRead(INPUT_B);

  if (pressingA && !pressedA) { //button A key down
    pressedA = true;
    lightLED(1);
    Serial.write('z');
  } else if (!pressingA && pressedA) { //button A key up
    pressedA = false;
    Serial.write('a');
  }
  if (pressingB && !pressedB) { //button B key down
    pressedB = true;
    lightLED(2);
    Serial.write('x');
  } else if (!pressingB && pressedB) { //button B key up
    pressedB = false;
    Serial.write('s');
  }
  updateAlpha(pressingA, pressingB);
  delay(1);
}

void lightLED(int index) {
  currentAlpha[index] += LED_ALPHA;
  if (currentAlpha[index] > 255) {
    currentAlpha[index] = 255;
  }
  setLED(index);
}
void updateAlpha(bool pressingA, bool pressingB) {
  if (!pressingA) {
    if (currentAlpha[1] > 0) {
      currentAlpha[1] -= LED_FADE_SPEED;
    }
    setLED(1);
  }
  if (!pressingB) {
    if (currentAlpha[2] > 0) {
      currentAlpha[2] -= LED_FADE_SPEED;
    }
    setLED(2);
  }

}

void setLED(int index) {
  float alpha = (int) currentAlpha[index];
  analogWrite(LED_PINS[index], alpha);
}

