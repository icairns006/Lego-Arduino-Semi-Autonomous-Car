/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://www.arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */
#define thrin 4
#define steerin 5
#define remotepin 6
#define autopin 8
#define  fwddis A2  //Analog distance sensors
#define  rhtdis A1  // Analog distance sensor
#define  motorRightPWM        3
#define  motorRightDirection 12
#define  motorLeftPWM       11
#define  motorLeftDirection 13
int steer=0;
int thr = 0;
int timer=0;
int Rightdis=0;
int Forwarddis=0;
int rightlim=150;
int forwardlim=150;
int forwardtgt=300;
int An=0;
int pwm1=0;
int pwm2=0;
float test1=0;
int mode=1;
int Righttgt=343;
int  fwdtracker=0;
int revtimer=0;

float gainwall=30;
float gainwall3=10;
float gainwall2=.02;
boolean remotestatus =false;
boolean automode= false;
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(remotepin, OUTPUT);
  pinMode(autopin, OUTPUT);
  pinMode(motorRightPWM, OUTPUT);
  pinMode(motorRightDirection, OUTPUT);
  pinMode(motorLeftPWM, OUTPUT);
  pinMode(motorLeftDirection, OUTPUT);
  pinMode(steerin, INPUT);
  pinMode(thrin, INPUT);
  Serial.begin (9600);
  digitalWrite(remotepin, LOW);
  digitalWrite(autopin, LOW);
  pinMode(fwddis, INPUT);
  pinMode(rhtdis, INPUT);
}

// the loop function runs over and over again forever
void loop() {
 if(automode==true){
  Rightdis=analogRead(rhtdis);
  Forwarddis= analogRead(fwddis);
  //Serial.print(Rightdis);
  //Serial.println("Rightdis");
  Serial.print(Forwarddis);
  Serial.println("Forwarddis");
  if (mode==1){
    autofwd();
    //Serial.println("auto 1");
    
  }
  if (mode==2){
    fwdtracker=0;
    revtimer=0;
    pwm1=0;
    pwm2=0;
    writetomotors(pwm1, 1, pwm2, 1);
    Serial.println("auto 2");
    digitalWrite(autopin, HIGH);
    delay(26000);
    digitalWrite(autopin, LOW);
    mode=3;
  }
  if(mode==3){
    Serial.println("auto 3");
    autorev();
  }

  An=0;
  
  }
 else{
    
    Rightdis=analogRead(rhtdis);
    Serial.print(Rightdis);
    Serial.println("Rightdis");
    if (Rightdis< rightlim){
      An=0; 
    }
    else if(Rightdis> rightlim){
       //Serial.println("Rightdis");
      An=An+1;
    }
    if(An>100){
      automode=true;
      digitalWrite(remotepin, LOW);
    }
    readremote(); 
  if(remotestatus==false){//driving car
         //Serial.println("driving car");
       drivecarclc();
       if (thr<100 && thr>-100){
        writetomotors(0, 1, 0, 1);
        //Serial.println("stop");
       }
       else if(thr>100){
          writetomotors(pwm1, 1, pwm2, 1);
          //Serial.println("fwd");
        }
        else if(thr<-100){
          writetomotors(pwm1, 0, pwm2, 0);
          //Serial.println("rev");
        }
        
  }
  if(remotestatus==true){//nano is driving arm
    //Serial.println("driving car");
   writetomotors(0, 0, 0, 0);
  }
  if(thr<100 && thr>-100){
    if(steer>100){
      
      timer=timer+1;
      Serial.println(timer);
      delay(100);
      if (timer==30){
        Serial.println("its time??????????????????");
        if (remotestatus==false){
              remotestatus=true;
              digitalWrite(remotepin, HIGH);
            
            }
            else{
              remotestatus=false;
              digitalWrite(remotepin, LOW);
           }
     }
    }
    else{
    timer=0;
    
   }
  
  }
  else{
       timer=0;
     }
  }
  delay(5);
}





////////////////////////////////////////////////////////////////////
void readremote() {
  steer = pulseIn(steerin, HIGH)-1470;
  thr = pulseIn(thrin, HIGH)-1470;
  //Serial.print(steer);
  //Serial.println("steer");
  //Serial.print(thr);
  //Serial.println("thr");
}

void autorev(){
    //Serial.println(1);
    if(rightlim<Rightdis){
    pwm1=-180+((Rightdis-Righttgt)*gainwall3);
    if(pwm1<-255){
              pwm1=-255;
            //  Serial.print("yes");
            }
            if(pwm1>-100){
              pwm1=-100;
            }
    pwm2=-180;
    writetomotors(pwm1, 1, pwm2, 1);
    }
    else{
      revtimer=revtimer+1;
      if (revtimer==40){
        automode=false;
        mode=1;
        pwm1=0;
        pwm2=pwm1;
        writetomotors(pwm1, 1, pwm2, 1);
      }
    }
    
    
  }


void autofwd(){
  if(forwardlim>Forwarddis){
    //Serial.println(1);
    if(rightlim<Rightdis){
    pwm1=180-((Rightdis-Righttgt)*gainwall);
    if(pwm1>255){
              pwm1=255;
            //  Serial.print("yes");
            }
            if(pwm1<100){
              pwm1=100;
            }
    pwm2=180;
    writetomotors(pwm1, 1, pwm2, 1);
    }
    
    
  }
  else{
    
    double distance=(forwardtgt-Forwarddis);
    //Serial.print(distance);
    //Serial.println("distance");
    
    double dist2=distance*gainwall2;
    //Serial.print(dist2);
    //Serial.println("dist2");
    double pwm1111=dist2*180;
    //Serial.print(pwm1111);
    //Serial.println("pwm1111");
    
    
    pwm1=pwm1111;
    if (Forwarddis>forwardtgt-15 && Forwarddis<forwardtgt+15){
      fwdtracker=fwdtracker+1;
      Serial.println(fwdtracker);
      if (fwdtracker==50){
        mode=2;
        pwm1111=0;
        writetomotors(pwm1, 1, pwm2, 1);
      }
    
    }
    else{
      fwdtracker=0;
     
    }
    if(pwm1111>180){
              pwm1111=180;
            //  Serial.print("yes");
            }
            if(pwm1111<-180){
              pwm1111=-180;
            }
    pwm1=pwm1111;
    pwm2=pwm1;
    Serial.print(pwm1);
    Serial.println(" pwm1");
    writetomotors(pwm1, 1, pwm2, 1);
  
  
  }
  
}


  
void drivecarclc() {
  float test1=0;
  float test2=0;
  float turn1=0;
  float turn2=0;
    test1= (thr/1.2);
    turn1= (steer/.5);
    pwm1=test1+turn1;
    pwm2=test1-turn1;
   // Serial.print(thr);
    //Serial.println("thr2");
    //Serial.print(steer);
    //Serial.println("steer2");
    //Serial.print(test1);
    //Serial.println("test1");
    //Serial.print(turn1);
    //Serial.println("turn1");
    //Serial.print(pwm1);
    //Serial.println("pwm1");
    
    //test2= (255*thr/500)
    //(steer/1.5);
    //pwm2=test2;
    //Serial.print(pwm2);
    //Serial.println("pwm2");
    if (pwm1>255){
      pwm1=255;
    }
    if (pwm1<-255){
      pwm1=-255;
    }
    if (pwm2>255){
      pwm2=255;
    }
    if (pwm2<-255){
      pwm2=-255;
    }
  
 
}


void writetomotors(int pwm11,int d1, int pwm22, int d2){
  int pwm12, pwm21;
  if (pwm11< 0){
    digitalWrite(motorLeftDirection, LOW);
  }
  else{
    digitalWrite(motorLeftDirection, HIGH);
  }
  pwm12=abs(pwm11);
  analogWrite(motorLeftPWM, pwm12);
  if (pwm22<0){
    digitalWrite(motorRightDirection, LOW);
  }
  else{
    digitalWrite(motorRightDirection, HIGH);
  }
  pwm21=abs(pwm22);
  analogWrite(motorRightPWM, pwm21);
  //Serial.print(pwm12);
    //Serial.println("pwm1atwheel");
    //Serial.print(pwm21);
    //Serial.println("pwm2atwheel");
  
}

