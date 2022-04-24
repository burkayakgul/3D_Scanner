
#include <math.h>
#include <Wire.h>
#include <VL53L0X.h>
#include <SPI.h>
#include <SD.h>
#include <LiquidCrystal_I2C.h>

#define xstepPin 2
#define xdirPin 3
#define ystepPin 4
#define ydirPin 5
#define xm1Pin 14
#define ym1Pin 16
#define xshutPin 7
#define csPin 10
#define full_distance 134
#define max_height 16000
#define max_distance 150
#define upButton 9
#define selectButton 8
#define downButton 6
int object_height = 0;
                         
float x_Axis = 0;
float y_Axis = 0;
float z_Axis = 0;
int menu = 1;
float RAD = 0.0;
float angle = 0.0;
VL53L0X sensor;
File file;
LiquidCrystal_I2C lcd(0x27,20,4);

void setup() {
  //step motor pins
  pinMode(xstepPin,OUTPUT);
  pinMode(xdirPin,OUTPUT);
  pinMode(ystepPin,OUTPUT);
  pinMode(ydirPin,OUTPUT);
  pinMode(xm1Pin,OUTPUT);
  pinMode(ym1Pin,OUTPUT);
  pinMode(csPin,OUTPUT);
  
  //sensor pin
  pinMode(xshutPin,INPUT_PULLUP);
  digitalWrite(xshutPin,HIGH);
  Serial.begin(9600);
  Wire.begin();
  
  sensor.init();
  sensor.setTimeout(500);

  RAD = (3.141592 / 180.0) * (360.0/800.0);
  
  lcd.init();
  lcd.init();
  lcd.backlight();
  //buttons
  pinMode(upButton,INPUT_PULLUP);
  pinMode(selectButton,INPUT_PULLUP);
  pinMode(downButton,INPUT_PULLUP);
  updateMenu();

  Serial.println("yukleme tamam");
}

void loop() {
  int down =  digitalRead(downButton);
  int up = digitalRead(upButton);
  int sel = digitalRead(selectButton);

  if(down ==  HIGH){
      menu++;
      updateMenu();
      delay(100);
      while(digitalRead(downButton)){}
  }

  if(up ==  HIGH){
      menu--;
      updateMenu();
      delay(100);
      while(digitalRead(upButton)){}
  }
  if(sel ==  HIGH){
      executeActions();
      menu = 1;
      updateMenu();
      delay(100);
      while(digitalRead(selectButton)){}
  }
  delay(100);
}


void scan()
{
  delay(100);
    int cancel = 0;
  if(SD.begin(csPin))
  {
    lcd.clear();
    lcd.print(" Scanning");
    lcd.setCursor(0,1);
    lcd.print(">Cancel");
    file = SD.open("scan.txt",FILE_WRITE);
    digitalWrite(xm1Pin,HIGH);
    digitalWrite(ym1Pin,HIGH);
    digitalWrite(xdirPin,HIGH);
    digitalWrite(ydirPin, HIGH);
    int nondefinedCount = 0;
    for(int i = 0 ; i < max_height; i++){
        int distance;
        for(int y = 0; y < 800; y++){
          distance = sensor.readRangeSingleMillimeters();
          if(digitalRead(selectButton)){cancel++;}
          if(cancel > 0){
            file.close();
            return;  
          }
     
            nondefinedCount = 0;
              
            x_Axis = angle*distance;
            y_Axis = i*0.0125;
            z_Axis = full_distance-distance;
            file.println((String)x_Axis+" "+y_Axis+" "+z_Axis);
            angle = (float)angle + (float)RAD;
          
          if(cancel > 0){
            return;  
          }
          digitalWrite(xstepPin, HIGH);
          delayMicroseconds(200);
          digitalWrite(xstepPin, LOW);
          delayMicroseconds(200);
          
        }

        if(nondefinedCount >= 800){
            object_height = i;
            returnBase(i);
            break;
        }

        for(int x = 0; x < 200;x++)
        {
          digitalWrite(ystepPin, HIGH);
          delayMicroseconds(200);
          digitalWrite(ystepPin, LOW);
          delayMicroseconds(200);
        }
    }
    if(nondefinedCount == 0 && object_height == 0 ){
        object_height = max_height;
        returnBase(max_height);
    }
    file.close();
  }else{
    Serial.println("dosya acilamadi hata olustu.");  
  }
  return;
}

void returnBase(int x){
    digitalWrite(ydirPin, LOW);
    for(int i = 0; i < x; i++)
    {
        digitalWrite(ystepPin, HIGH);
        delayMicroseconds(200);
        digitalWrite(ystepPin, LOW);
        delayMicroseconds(200);
    }
}


void updateMenu(){
  switch(menu){
    case 0:
      menu = 1;
      break;
    case 1:
      lcd.clear();
      lcd.print(">Start Scanning");
      lcd.setCursor(0,1);
      lcd.print(" Settings");  
      lcd.setCursor(0,2);
      lcd.print(" Calculate Time"); 
      break; 
    case 2:
      lcd.clear();
      lcd.print(" Start Scanning");
      lcd.setCursor(0,1);
      lcd.print(">Settings");  
      lcd.setCursor(0,2);
      lcd.print(" Calculate Time"); 
      break;
    case 3:
      lcd.clear();
      lcd.print(" Start Scanning");
      lcd.setCursor(0,1);
      lcd.print(" Settings");  
      lcd.setCursor(0,2);
      lcd.print(">Calculate Time");   
      break;
    case 4:
      menu = 3;
      break;
  } 
}

void executeActions(){
  switch(menu){
    case 1:
      scan();
      break;  
  }
  return;  
}
