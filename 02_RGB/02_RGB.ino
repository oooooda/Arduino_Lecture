void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(A0, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int val = analogRead(A0);
  Serial.println(val); 
  digitalWrite(6, 1);
  digitalWrite(5, 0);
  digitalWrite(3, 0);
  delay(val);
  digitalWrite(6, 0);
  digitalWrite(5, 1);
  digitalWrite(3, 0);
  delay(val);
  digitalWrite(6, 1);
  digitalWrite(5, 0);
  digitalWrite(3, 1);
  delay(val);
}
