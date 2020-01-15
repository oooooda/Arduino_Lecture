int i=0;
int sw=0;
void setup() {
  // put your setup code here, to run once:
  attachInterrupt(sw,count, INPUT_PULLUP);
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
    Serial.println(i);
   switch(i){
    case(1) :
      digitalWrite(6, 1);
      digitalWrite(5, 0);
      digitalWrite(3, 0);
      Serial.println("blue");
      break;
    case(2) :
      digitalWrite(6, 0);
      digitalWrite(5, 1);
      digitalWrite(3, 0);
      Serial.println("green");
      break;
    case(3) :
      digitalWrite(6, 0);
      digitalWrite(5, 0);
      digitalWrite(3, 1);
      Serial.println("red");
      break;
    default :
      break;
  }
  
}
void count(){
  if(i>=3)
  {
    i=1;
  }
  else i++;
}
