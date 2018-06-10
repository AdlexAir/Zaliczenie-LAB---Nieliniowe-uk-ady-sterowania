#define X_MIN_PIN           3
#ifndef X_MAX_PIN
#define X_MAX_PIN           2
#endif
#define Y_MIN_PIN          14
#define Y_MAX_PIN          15
#define Z_MIN_PIN          18
#define Z_MAX_PIN          19
#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#define X_CS_PIN           53

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#define Y_CS_PIN           49

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
#define Z_CS_PIN           40

// OD HEATERA

#define TEMP 13
#define a -0.0000005818693
#define b 0.0009986724979
#define c -0.6637052325871
#define d 256.9020320981620

#define HEATER0 10
#define FAN 44

#include <LiquidCrystal.h>

LiquidCrystal lcd(16,17,23,25,27,29);

int temp, ustTemp;
double y;
double i;
double u;
double e;
int licznik=0;
int x_max;
int y_max;
int z_max;
int krok_x=0;
int krok_y=0;
int krok_z=0;
int SetPoint = 0;
bool Ok_sended = 1, start_seq = 1, grzanie = 0;
String s,pack="N";
bool x_kier, y_kier, z_kier;
int send_temp = 0;

void setup() 
{
  pinMode(HEATER0,OUTPUT);
  pinMode(FAN,OUTPUT);
  digitalWrite(HEATER0,LOW);
  digitalWrite(FAN,HIGH);
  i=0;
  pinMode(TEMP,INPUT);
  pinMode(X_MAX_PIN,INPUT_PULLUP);
  pinMode(Y_MAX_PIN,INPUT_PULLUP);
  pinMode(Z_MAX_PIN,INPUT_PULLUP);
  
  Serial.begin(57600);
  pinMode(X_ENABLE_PIN,OUTPUT);
  digitalWrite(X_ENABLE_PIN,LOW);
  pinMode(Y_ENABLE_PIN,OUTPUT);
  digitalWrite(Y_ENABLE_PIN,LOW);
  pinMode(Z_ENABLE_PIN,OUTPUT);
  digitalWrite(Z_ENABLE_PIN,LOW);
  pinMode(X_DIR_PIN,OUTPUT);
  digitalWrite(X_DIR_PIN,LOW);
  pinMode(Y_DIR_PIN,OUTPUT);
  digitalWrite(Y_DIR_PIN,LOW);
  pinMode(Z_DIR_PIN,OUTPUT);
  digitalWrite(Z_DIR_PIN,LOW);
  pinMode(X_STEP_PIN,OUTPUT);
  pinMode(Y_STEP_PIN,OUTPUT);
  pinMode(Z_STEP_PIN,OUTPUT);

  lcd.begin(20,4);
}

void loop() 
{
  
  while(Serial.available()>0)
      {
        s=Serial.readStringUntil('\n');
        sscanf(s.c_str(),"%d %d %d %d",&krok_x,&krok_y,&krok_z,&send_temp);        
        Ok_sended = 1;

        if(send_temp + 10  > y && send_temp-10 < y)
        {
          pack = "Y";
        }
        else
        {
          pack="N";
        }
        
        if(krok_x>0)
        {
          digitalWrite(X_DIR_PIN,LOW);
          x_kier = true;
        }
        else
        {
          digitalWrite(X_DIR_PIN,HIGH);
          krok_x=-krok_x;
          x_kier = false;
        }
        if(krok_y>0)
        {
          digitalWrite(Y_DIR_PIN,LOW);
          y_kier = true;
        }
        else
        {
          digitalWrite(Y_DIR_PIN,HIGH);
          krok_y=-krok_y;
          y_kier = false;
        }
        if(krok_z>0)
        {
          digitalWrite(Z_DIR_PIN,LOW);
          z_kier = true;
        }
        else
        {
          digitalWrite(Z_DIR_PIN,HIGH);
          krok_z=-krok_z;
          z_kier = false;
        }        
      }
      
      x_max=digitalRead(X_MAX_PIN);
      y_max=digitalRead(Y_MAX_PIN);
      z_max=digitalRead(Z_MAX_PIN);
      digitalWrite(X_STEP_PIN,LOW);
      digitalWrite(Y_STEP_PIN,LOW);
      digitalWrite(Z_STEP_PIN,LOW);
      delayMicroseconds(50);
      if((x_max==0 && krok_x>0)||(x_kier== false && krok_x > 0))
      {
        digitalWrite(X_STEP_PIN,HIGH);
        krok_x--;
      }
      if((y_max==0 && krok_y>0)||(y_kier== false && krok_y > 0))
      {
        digitalWrite(Y_STEP_PIN,HIGH);
        krok_y--;
      }
      if((z_max==0 && krok_z>0)||(z_kier== false && krok_z > 0))
      {
        digitalWrite(Z_STEP_PIN,HIGH);
        krok_z--;
      }
      if(krok_x==0 && krok_y==0 && krok_z==0 && Ok_sended == 1)
      {
        pack = pack + "N";
        Serial.println(pack);
        
        Ok_sended = 0;
      }
      if(x_max == 1 && y_max == 1 && z_max == 1 && Ok_sended == 1)
        {
          Ok_sended = 0;
          pack = pack + "M";
          Serial.println(pack);
        }      
        //Serial.print(Ok_sended);
        //Serial.print(x_max);
        //Serial.print(y_max);
        //Serial.println(z_max);
      delayMicroseconds(50);
        
      temp=analogRead(TEMP);
      ustTemp = map(temp, 0, 1023, 0, 300);
      y=a*(double)pow(temp,3)+b*(double)pow(temp,2)+c*temp+d;
     
      //Regulator PI do 180 stopni
        SetPoint = send_temp;
        e=(SetPoint-y);
        i=i+e;
        u=255*e/(SetPoint-20)+i/70;
        if(u>255)
          {
            u=255;
          }  
          if(u<0)
          {
            u=0;
          }  
        if(y<SetPoint)
        {
                
          analogWrite(HEATER0,floor(u));
          i++;
        }
        else
        {
          digitalWrite(HEATER0,LOW);
        }

        if(licznik>1000)
        {
        lcd.setCursor(0, 0);
        lcd.setCursor(0, 0);
        lcd.print("TEMP: ");
        lcd.print(y); 
      
        lcd.setCursor(0, 1);
        lcd.print("U: ");
        lcd.print(u);
              
        lcd.setCursor(0, 2);
        lcd.print("I: ");
        lcd.print(i);
        licznik=0;
}       
licznik++;
}

