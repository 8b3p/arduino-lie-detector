#include <LiquidCrystal_I2C.h>
#include <Ds1302.h> 
#include <Wire.h>
#include <SD.h>

//<-- this is the RTC setup -->//
Ds1302 rtc(5, 2, 3);
//<-- screen setup END -->//



//<-- this is the screen setup -->//
LiquidCrystal_I2C lcd(0x27,16,2);
//<-- screen setup END -->//



//<-- this is the SD setup -->//
File myFile;
//<-- SD setup END -->//

//<-- this is the Detector Wire setup -->//
int Input = A1;
//<-- setup END -->//




void setup(){
  lcd.begin(16, 2);                                  // initialize the lcd 
  lcd.backlight();
  Serial.begin(9600);
  Serial.println("Initializing SD card ..");   //initializing the SD
  pinMode(10, OUTPUT);
  if(!SD.begin(4)) {
    Serial.println("failed lol");
//    return;
  } else {
    Serial.println("SD initialized");
  }
  SD.remove("datalog.txt");
  rtc.init();                            //initializing the RTC
}

void loop(){
  Ds1302::DateTime now;
  rtc.getDateTime(&now);
  lcd.clear();
  delay(50);
  lcd.setCursor(0, 1);
  lcd.print(now.year);
  if(now.year < 10) {
    lcd.print(0);
  }
  lcd.print("/");
  if(now.month < 10) {
    lcd.print(0);
  }
  lcd.print(now.month);    
  lcd.print("/");
  if(now.day < 10) {
    lcd.print(0);
  }
  lcd.print(now.day);
  if(now.hour < 10) {
    lcd.print(0);
  }
  lcd.setCursor(9 ,1);
  lcd.print(now.hour);
  lcd.print(":");
  if(now.minute < 10) {
    lcd.print(0);
  }
  lcd.print(now.minute);
  if (analogRead(Input) > 25) {
    lcd.setCursor(3, 0);
    lcd.print("defo lying!");
    Serial.println("writing to file");
    myFile = SD.open("datalog.txt", FILE_WRITE);
    if(myFile) {
      myFile.print("a lie detected : ");
      myFile.print(now.hour);
      myFile.print(":");
                 myFile.print(now.minute);
myFile.print(":");
            myFile.println(now.second);
      myFile.close();
      Serial.println("done writing");
    } else {
      Serial.println("a problem writing to the file");
    }
    delay(200);
    lcd.setCursor(3, 0);
    lcd.print("defo lying!");
    delay(200);
  } else if (analogRead(Input) > 18) {
    lcd.setCursor(3, 0);
    lcd.print("a lie (:!");
    Serial.println("writing to file");
    myFile = SD.open("datalog.txt", FILE_WRITE);
    if(myFile) {
      myFile.println("a lie detected");
            myFile.print("a lie detected : ");
      myFile.print(now.hour);
      myFile.print(":");
                 myFile.print(now.minute);
myFile.print(":");
            myFile.println(now.second);
      myFile.close();
      Serial.println("done writing");
    } else {
      Serial.println("a problem writing to the file");
    }
    delay(200);
    lcd.setCursor(3, 0);
    lcd.print("a lie (:!");
    delay(200);
  } else if (analogRead(Input) <= 18) {
    lcd.setCursor(1, 0);
    lcd.print("you're good");
  }
  
  myFile = SD.open("datalog.txt");
  if(myFile) {
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  }
  Serial.println(analogRead(Input));
  delay(200);
}
