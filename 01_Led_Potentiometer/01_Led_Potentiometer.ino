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

  //int val = analogRead(A0)/4; //가변저항의 입력값은 0~1024이므로 /4를 해주었다
 // Serial.println(val); 
  //analogWrite(5, val);
  //analogWrite(3, val);
  //analogWrite(6, val);

  int val = analogRead(A0);
  Serial.println(val); 
  digitalWrite(6, 1);
  digitalWrite(3, 0);
  delay(val);
  digitalWrite(6, 0);
  digitalWrite(3, 1);
  delay(val);

}
