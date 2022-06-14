// Motor X
int m1DirPin = 4;
int m1StepPin = 5;

// Motor Y
int m2DirPin = 7;
int m2StepPin = 6;

int M1en=8;  //Motor X enable pin
int M2en=12;  //Motor Y enable pin

unsigned int motorSpeed = 100 ;
unsigned long motorStep = 1 ;

void setup( ) {
  Serial.begin(9600);                                        // 시리얼 모니터 속도 정의
  
  pinMode(m1DirPin, OUTPUT);
  pinMode(m1StepPin, OUTPUT) ;
  pinMode(m2DirPin, OUTPUT); 
  pinMode(m2StepPin, OUTPUT) ;
  pinMode(M1en,OUTPUT);
  pinMode(M2en,OUTPUT);
  digitalWrite(m1DirPin, 0); 
  digitalWrite(m2DirPin, 0) ;
  digitalWrite(m1StepPin, 0); 
  digitalWrite(m2StepPin, 0) ;
}

void loop( ) {
    digitalWrite(M1en,HIGH);
    digitalWrite(M2en,HIGH);
  for(int i=0; i<200; i++) {
    motorStart();
    delayMicroseconds(2000); 
  }
  delay(1000);
  digitalWrite(M1en,LOW);// Low Level Enable
  digitalWrite(M2en,LOW);// Low Level Enable
  for(int i=0; i<200; i++) {
    motorStop();
    delayMicroseconds(2000); 
  }
  delay(1000);
}

void motorStop() {
   digitalWrite(m1DirPin, LOW); 
   digitalWrite(m2DirPin, LOW);
   digitalWrite(m1StepPin, LOW); 
   digitalWrite(m2StepPin, LOW) ;
}

void motorStart() {
  digitalWrite(m1StepPin, 1); 
  digitalWrite(m2StepPin, 1) ;
//      delayMicroseconds(motorSpeed); 
  delay(1);
  digitalWrite(m1StepPin, 0) ;
  digitalWrite(m2StepPin, 0); 
//      delayMicroseconds(motorSpeed) ;
  delay(1); 
}
