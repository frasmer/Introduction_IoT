// Potentiometer is connected to GPIO 34 (Analog ADC1_CH6) 
const int potPin = 34;

//variable for storing the potentiometer value
int potValue = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Reading potentiometer value
  potValue = analogRead(potPin);
  Serial.println(potValue);
  delay(500);
}