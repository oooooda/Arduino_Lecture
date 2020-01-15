void setup() {
  // put your setup code here, to run once:
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i=0;i<255;i++)
  {
    analogWrite(5, i);
    analogWrite(3, i);
    analogWrite(6, i);
    delay (100);
  }
  for(int j=255;j>=0;j--)
  {
    analogWrite(5, j);
    analogWrite(3, j);
    analogWrite(6, j);
    delay (100);
  }
 delay(1000);
}
 
