const int read1=11;
const int read2=12;
boolean pressed1=false;
boolean pressed2=false;


void setup() {
  Serial.begin(9600);
  //set up both read pins (with pull-up resistors)
  pinMode(read1,INPUT);
  pinMode(read2,INPUT);
  digitalWrite(read1,HIGH);
  digitalWrite(read2,HIGH);  
}

void loop() {
  if(digitalRead(read1)==LOW && !pressed1){
    pressed1=true;
    Serial.write("z");
  } else if(digitalRead(read1)==HIGH && pressed1){
    pressed1=false;
    Serial.write("a");
  }
  if(digitalRead(read2)==LOW && !pressed2){
    pressed2=true;
    Serial.write("x");
  } else if(digitalRead(read2)==HIGH && pressed2){
    pressed2=false;
    Serial.write("s");
  }
}
