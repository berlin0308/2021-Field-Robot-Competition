String cmd;
int pwm_L=0,pwm_R=0;
int ledA_R=2, ledA_G=3, ledA_B=4;
int ledB_R=5, ledB_G=6, ledB_B=7;
char pwmL[4],pwmR[4],sender,state,power,motion;
char ledA, ledB, ledC, ledD;
int Lf=10, Lb=11, Rf=12, Rb=13;  // Arduino mega pins
int leftDriftSpeed=60, rightDriftSpeed=60, intervalSpeed=60;
int leftDriftTime=5, rightDriftTime=5, intervalTime=5;


void setup() {
  pinMode(ledA_R, OUTPUT);
  pinMode(ledA_G, OUTPUT);  
  pinMode(ledA_B, OUTPUT);
  pinMode(ledB_R, OUTPUT);
  pinMode(ledB_G, OUTPUT);  
  pinMode(ledB_B, OUTPUT);
  turnoffLED(ledA_R,ledA_G,ledA_B);
  turnoffLED(ledB_R,ledB_G,ledB_B);
  Serial.begin(9600);
  runMotor(0,0);
}

void loop() {
  if (Serial.available()) {
    Serial.println("serial available");
    // 讀取傳入的字串直到'e'結尾 不包括e
    cmd = Serial.readStringUntil('e');
    //Serial.println(cmd); 

    sender = cmd[0];
    //Serial.println(sender);
    
    state = cmd[1];
    //Serial.println(state);
    
    power = cmd[2];
    //Serial.println(power);
 
    motion = cmd[3];
    //Serial.println(motion);
    
    if(sender=='1'){   // the signal is from python
      
      if(state=='1'){  // 甲關
          
        ledA = cmd[10];
        ledB = cmd[11];
        ledC = cmd[12];
        ledD = cmd[13];
        
        igniteLED(ledA_R,ledA_G,ledA_B,ledA);  // ledA ignited
        igniteLED(ledB_R,ledB_G,ledB_B,ledB);  // ledB ignited
        
        if(motion=='1'){  // turn Left immediately
          runMotor(0,leftDriftSpeed);  // to Left, right wheel is faster
          delay(leftDriftTime);
          runMotor(intervalSpeed,intervalSpeed);
          delay(intervalTime);
          runMotor(0,leftDriftSpeed);
          delay(leftDriftTime);
          runMotor(0,0);
          turnoffLED(ledA_R,ledA_G,ledA_B);
          turnoffLED(ledB_R,ledB_G,ledB_B);
        }
        else if(motion=='2'){  // turn Right immediately
          runMotor(rightDriftSpeed,0);
          delay(rightDriftTime);
          runMotor(intervalSpeed,intervalSpeed);
          delay(intervalTime);
          runMotor(rightDriftSpeed,0);
          delay(rightDriftTime);
          runMotor(0,0);
          turnoffLED(ledA_R,ledA_G,ledA_B);
          turnoffLED(ledB_R,ledB_G,ledB_B);
        }
        else{
        
          if(power=='1'){  // power on, read pwm for motor
            pwmL[0] = cmd[4];
            pwmL[1] = cmd[5];
            pwmL[2] = cmd[6];
            pwmL[3] = '\0';
            pwm_L = atoi(pwmL);
            //Serial.println((String)pwm_L);
            Serial.println("pwm L:"+(String)pwm_L);
            
            pwmR[0] = cmd[7];
            pwmR[1] = cmd[8];
            pwmR[2] = cmd[9];
            pwmR[3] = '\0';
            pwm_R = atoi(pwmR);
            //Serial.println(pwm_R);
            Serial.println("pwm R:"+(String)pwm_R);
          }
          else{     // power off
            pwm_L = 0;
            pwm_R = 0;
          }

          runMotor(pwm_L,pwm_R);
          delay(100);
          
        }
        
     } 
   }   
  }
  else{
      delay(100);
  }
}

void igniteLED(int R,int G,int B,char choice){
  switch(choice){
    case '0':
      light(R,G,B,0,0,0);  // light off
      break;
      
    case '1':
      light(R,G,B,255,255,255);  // white
      break;
 
    case '2':
      light(R,G,B,255,0,0);  // Red
      break;

    case '3':
      light(R,G,B,0,255,0);  // Green
      break;
     
    case '4':
      light(R,G,B,0,0,255);  // Blue
      break;
    
    case '5':
      light(R,G,B,0,255,255);  // Cyan
      break;
      
    case '6':
      light(R,G,B,255,255,0);  // Yellow
      break;
      
    case '7':
      light(R,G,B,255,0,255);  // Megenta
      break;
      
  }
}

void turnoffLED(int R,int G,int B){
  pinMode(R,INPUT);
  pinMode(G,INPUT);
  pinMode(B,INPUT);
}

void runMotor(int pwm_L, int pwm_R){
  analogWrite(Lf,pwm_L);
  analogWrite(Lb,0);
  analogWrite(Rf,pwm_R);
  analogWrite(Rb,0);
}

void light(int R,int G,int B,int r,int g,int b){
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  analogWrite(R,255-r);
  analogWrite(G,255-g);
  analogWrite(B,255-b);
}

 
 
