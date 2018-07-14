#include <Servo.h>
Servo myservo1;
Servo myservo2;
Servo myservo3;

#define autopin 6
#define remotepin 5
#define steerin 4
#define thrin 3
#define mysin 2
int steer=0;
int thr=0;
int xx=0;
int servo1=50;
int servo2=50;
int servo3=50;
int servo2lim=120;
int gain1=.01;
int gain2=.01;
int gain3=.01;
int x=0;
int moves2[][3]={{50,120,50},
                 {10,120,10},
                 {10,10,10},
                 {50,10,50},
                 {50,10,50},
                 {50,10,50},
                 {50,10,50},
                 {50,10,50},
                 {50,10,50},
                 {30,30,30},
                 {50,10,50},
                 {50,10,50},
                 {50,10,50},
                 {50,10,50},
                 {50,10,50},
                 {50,10,50},
                 {50,10,50},
                 {50,10,50},
                 {50,10,50}};

void setup() {
  // put your setup code here, to run once:
  pinMode(steerin, INPUT);
  pinMode(thrin, INPUT);
  pinMode(mysin, INPUT);
  pinMode(remotepin, INPUT);
  pinMode(autopin, INPUT);
  //digitalWrite(remotepin, HIGH);
  //digitalWrite(autopin, HIGH);
  Serial.begin (9600);
  myservo1.attach(9);
  myservo2.attach(11);
  myservo3.attach(10);
  myservo1.write(servo1);
  myservo2.write(servo2);
  myservo3.write(servo3);
  

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(digitalRead(remotepin));
  if(digitalRead(remotepin)==1){
  readremote();
  servocalcremote();
  writeservo(servo1,servo2,servo3);
  x=0;
  delay(5);
  }
  else if(digitalRead(autopin)==1){
    if(x<5){
     servo1=(moves2[x][0]-servo1)/abs(moves2[x][0]-servo1)+servo1;
     servo2=(moves2[x][1]-servo2)/abs(moves2[x][1]-servo2)+servo2;
     servo3=(moves2[x][2]-servo3)/abs(moves2[x][2]-servo3)+servo3;
     writeservo(servo1,servo2,servo3);
     if(servo1==moves2[x][0] && servo2==moves2[x][1] && servo3==moves2[x][2]){
        x=x+1;
      }
     }
     delay(75);
     
  }
  else{
    x=0;
  }

}

void readremote() {
  steer = pulseIn(steerin, HIGH)-1470;
  thr = pulseIn(thrin, HIGH)-1470;
  
  
  }

void servocalcremote(){
  Serial.print(thr);
    Serial.println("thr");
     Serial.print(steer);
    Serial.println("steer");
  if(thr>50){
    if(steer>100){
      servo1=servo1+1;
      servo3=servo3+1;
    }
    if(steer<-100){
      servo1=servo1-1;
      servo3=servo3-1;
    }
    
    Serial.print(servo1);
    Serial.println("servo1");
    if(servo3<0){
      servo3=0;
    }
    if(servo3>175){
      servo3=175;
    }
    if(servo1<0){
      servo1=0;
      Serial.println("Whoops");
    }
    if(servo1>175){
      servo1=175;
    }
     Serial.print(servo1);
    Serial.println("servo1");
  }
  if(thr<-50){
    if(steer>100){
      servo2=servo2+1;
    }
    if(steer<-100){
      servo2=servo2-1;
    }
    if(servo2<0){
      servo2=0;
    }
    if(servo2>servo2lim){
      servo2=servo2lim;
    }
    Serial.print(servo2);
    Serial.println("servo2");
  }
}

void writeservo(int x,int y , int z){
  myservo1.write(x);
  if(y>servo2lim){
    y=servo2lim;
  }
  myservo2.write(y);
  myservo3.write(z);
  
  }
    

