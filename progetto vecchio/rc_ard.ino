#include <PID_v1.h>

double lSetpoint,lInput,lOutput;
double lKp=10, lKi=2, lKd=0;

double rSetpoint,rInput,rOutput;
double rKp=10, rKi=2, rKd=0;

unsigned long pid_t = 20;

PID lPID(&lInput,&lOutput,&lSetpoint,lKp,lKi,lKd,DIRECT);
PID rPID(&rInput,&rOutput,&rSetpoint,lKp,lKi,lKd,DIRECT);

#define EN 9
#define PIN1 8
#define PIN2 7


#define EN2 5
#define PIN4 4
#define PIN3 3

#define YL 18
#define YR 19

#define BL 34
#define BR 35

#define ECHO1 31
#define TRIG1 30

#define ECHO2 33
#define TRIG2 32

volatile long lDegrees;
volatile long rDegrees;

void lcountDegrees(){
 if(digitalRead(YL) != digitalRead(BL))
  lDegrees++;
 else
  lDegrees--;
}

void rcountDegrees(){
 if(digitalRead(YR) != digitalRead(BR))
  rDegrees++;
 else
  rDegrees--;
}

void setup() {
  Serial.begin(9600);
  while(!Serial)

  pinMode(EN,OUTPUT);
  pinMode(PIN1,OUTPUT);
  pinMode(PIN2,OUTPUT);

  pinMode(EN2,OUTPUT);
  pinMode(PIN4,OUTPUT);
  pinMode(PIN3,OUTPUT);
  
  pinMode(YL,INPUT);
  pinMode(YR,INPUT);

  pinMode(BL,INPUT);
  pinMode(BR,INPUT);

  lDegrees = 0;  
  rDegrees = 0;

  attachInterrupt(digitalPinToInterrupt(YR),lcountDegrees,RISING);
  attachInterrupt(digitalPinToInterrupt(YL),rcountDegrees,RISING);

  lPID.SetSampleTime(pid_t);
  lPID.SetOutputLimits(0,255);
  lPID.SetMode(AUTOMATIC);

  pinMode(TRIG1,OUTPUT);
  pinMode(ECHO1,INPUT);

  pinMode(TRIG2,OUTPUT);
  pinMode(ECHO2,INPUT);

}

String cmd;
unsigned long rt,lt;
int lDeg_p,rDeg_p,lDeg_c,rDeg_c;
int pwm = 100; 
long p1,p2,distanza;

void loop() {

  digitalWrite(TRIG1,LOW);

  digitalWrite(TRIG1,HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG1,LOW);

  p1 = pulseIn(ECHO1,HIGH);
  distanza = p1/58.31;

  Serial.println(distanza);
  
  if(distanza < 4 && distanza > 0)
    ostacolo();
  
  delay(1000);
  
  if((cmd = Serial.available()) != "")
    if(cmd == "DRITTO")
      dritto();
    else if(cmd == "DESTRA")
      destra();
    else if(cmd == "SINISTRA")
      sinistra();
    else if(cmd == "GIRA")
      gira();
    else if(cmd == "FERMO")
      fermo();
}

void lSetMotorSpeed(int pwm){
  lSetpoint = (double)pwm;
  if(pwm > 100) pwm=100;
  else if(pwm < -100) pwm=-100;

  analogWrite(EN,int(abs(pwm)*2.55));

  if((millis()-lt)>pid_t){
    lDeg_c = lDegrees - lDeg_p;
    lDeg_p = lDegrees;
      
    lInput = lDeg_c;
    lPID.Compute();
    pwm = (int)lOutput;
     
    lt = millis();  
  }

  if(pwm >= 0){
      digitalWrite(PIN1,HIGH);
      digitalWrite(PIN2,LOW);  
  }else{
      digitalWrite(PIN1,LOW);
      digitalWrite(PIN2,HIGH);  
  }
}

void rSetMotorSpeed(int pwm){
  rSetpoint = (double)pwm;
  if(pwm > 100) pwm=100;
  else if(pwm < -100) pwm=-100;

  analogWrite(EN2,int(abs(pwm)*2.55));

  if((millis()-rt)>pid_t){
    rDeg_c = rDegrees - rDeg_p;
    rDeg_p = rDegrees;
      
    rInput = rDeg_c;
    rPID.Compute();
    pwm = (int)rOutput;
    
    rt = millis();  
  }

  if(pwm >= 0){
      digitalWrite(PIN4,HIGH);
      digitalWrite(PIN3,LOW);  
  }else{
      digitalWrite(PIN4,LOW);
      digitalWrite(PIN3,HIGH);  
  }
}

void dritto(){
  rSetMotorSpeed(40);
  lSetMotorSpeed(40);
}

void sinistra(){
  while((cmd = Serial.available()) == "SINISTRA"){ 
    rSetMotorSpeed(40);
    lSetMotorSpeed(-25);  
  }
}

void destra(){
  while((cmd = Serial.available()) == "DESTRA"){ 
    rSetMotorSpeed(-25);
    lSetMotorSpeed(40);  
  }
}

void gira(){
  while((cmd = Serial.available()) != ""){  
    rSetMotorSpeed(30);
    lSetMotorSpeed(-30);
  }
}

void fermo(){
  rSetMotorSpeed(0);
  lSetMotorSpeed(0);    
}

double secondo_us(){
  digitalWrite(TRIG2,LOW);
  digitalWrite(TRIG2,HIGH);
  delay(10);
  digitalWrite(TRIG2,LOW);
      
  p2 = pulseIn(ECHO2,HIGH);  
  return p2/58.31;
}

void ostacolo(){
  fermo();
  while((cmd = Serial.available()) == ""){
    while(secondo_us() < 6)
      dritto();
    
    if((cmd = Serial.available()) == "")
      while(secondo_us() < 6)
        lSetMotorSpeed(25);
  }
}
