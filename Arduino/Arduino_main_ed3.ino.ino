String cmd;
int pwm_L=0,pwm_R=0;

int ledA_R=2, ledA_G=3, ledA_B=4;   // left-front light, pwm pins
int ledB_R=5, ledB_G=6, ledB_B=7;   // right-front light, pwm pins
int ledC_R=100, ledC_G=100, ledC_B=100;  // left-rear light, digital pins
int ledD_R=100, ledD_G=100, ledD_B=100;  // right-rear light, digital pins

char pwmL[4],pwmR[4],sender,state,power,motion;
char ledA, ledB, ledC, ledD;
int Lf=10, Lb=11, Rf=12, Rb=13;  // Arduino mega pins
int leftDriftSpeed=60, rightDriftSpeed=60, intervalSpeed=60;
int leftDriftTime=5, rightDriftTime=5, intervalTime=5;

#include <LiquidCrystal_PCF8574.h>
#include "DHT.h"
#include <Ultrasonic.h>
#define DHTPIN 9 // 可變pin位
#define DHTTYPE DHT11

Ultrasonic ultrasonic(12,13); // Trig 12, Echo 13
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_PCF8574 lcd(0x27);
int distance;

void setup() {
  LED_output();
  
  LED_off('A');
  LED_off('B');
  Serial.begin(9600);
  runMotor(0,0);
  
  dht.begin();  //初始化DHT
  lcd.begin(16, 2); // 初始化LCD
  lcd.setBacklight(255);
  lcd.clear();
  lcd.setCursor(0, 0);  //設定游標位置 (字,行)
  lcd.print("Hello nice to meet");
  lcd.setCursor(0, 1);
  lcd.print("you.");
  lcd.setCursor(0, 2);
  lcd.print(" Love you !");
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
        }
        else if(motion=='2'){  // turn Right immediately
          runMotor(rightDriftSpeed,0);
          delay(rightDriftTime);
          runMotor(intervalSpeed,intervalSpeed);
          delay(intervalTime);
          runMotor(rightDriftSpeed,0);
          delay(rightDriftTime);
          runMotor(0,0);
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
     if(state=='3'){// CD關
        float t = dht.readTemperature();  //取得溫度C
        distance = ultrasonic.read(); // 不加參數就是 CM, INC 英吋
        lcd.clear();
        lcd.setCursor(0, 0);  //設定游標位置 (字,行)
        lcd.print("Depth:");  //Relative Humidity 相對濕度簡寫
        lcd.setCursor(7, 0);  
        lcd.print(distance);
        lcd.setCursor(9,0);
        lcd.print(" CM");
        lcd.setCursor(0, 1);  //設定游標位置 (字,行)
        lcd.print("Temp:");
        lcd.setCursor(7, 1);  
        lcd.print(t);
        lcd.setCursor(13, 1);
        lcd.print((char)223); //用特殊字元顯示符號的"度"
        lcd.setCursor(14, 1);
        lcd.print("C");  
     }
   }   
  }
  else{
      delay(100);
  }
}

void LED_output(){

  pinMode(ledA_R, OUTPUT);
  pinMode(ledA_G, OUTPUT);
  pinMode(ledA_B, OUTPUT);
  
  pinMode(ledB_R, OUTPUT);
  pinMode(ledB_G, OUTPUT);
  pinMode(ledB_B, OUTPUT);
  
  pinMode(ledC_R, OUTPUT);
  pinMode(ledC_G, OUTPUT);
  pinMode(ledC_B, OUTPUT);
  
  pinMode(ledD_R, OUTPUT);
  pinMode(ledD_G, OUTPUT);
  pinMode(ledD_B, OUTPUT);
 
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

int get_pins(char key){
    if(key=='A'){
        return ledA_R, ledA_G, ledA_B;
    }
    if(key=='B'){
        return ledB_R, ledB_G, ledB_B;
    }
    if(key=='C'){
        return ledC_R, ledC_G, ledC_B;
    }
    if(key=='D'){
        return ledD_R, ledD_G, ledD_B;
    }
    
}

void LED_off(char key){
  int R, G, B;
  R, G, B = get_pins(key);
  light(R,G,B,0,0,0);
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


 
 
