
#include "Arduino.h"
#include "BTHC05.h"
#include "DS18B20.h"
#include "LiquidCrystal.h"



#define BTHC05_PIN_TXD 11
#define BTHC05_PIN_RXD 10
#define DS18B20_PIN_DQ 2
#define LCD_PIN_RS 8
#define LCD_PIN_E 7
#define LCD_PIN_DB4 3
#define LCD_PIN_DB5 4
#define LCD_PIN_DB6 5
#define LCD_PIN_DB7 6





BTHC05 bthc05(BTHC05_PIN_RXD, BTHC05_PIN_TXD);
DS18B20 ds18b20(DS18B20_PIN_DQ);
LiquidCrystal lcd(LCD_PIN_RS, LCD_PIN_E, LCD_PIN_DB4, LCD_PIN_DB5, LCD_PIN_DB6, LCD_PIN_DB7);


const int timeout = 30000; 
char menuOption = 0;
long time0;


float temperatureC = 0; 
const int maxDataPoints = 100; 
float temperatureData[maxDataPoints]; 
int dataCount = 0; 

void stopFunction();
char getUserInput();



void setup()
{
 
  Serial.begin(9600);
  while (!Serial)
    ; 
  Serial.println("start");


  bthc05.begin(9600);
 
  
  lcd.begin(16, 2);
  menuOption = getUserInput(); 
}


void loop()
{
  while (true)  
  {
    if (menuOption == '1')
    {
      
      String bthc05Str = "";


     
      if (bthc05.available())
      {
        
        bthc05Str = bthc05.readStringUntil('\n');
        
        Serial.print("BT Raw Data: ");
        Serial.println(bthc05Str);
      }


      
      bthc05.println("PUT YOUR SENSOR DATA HERE");
    }
    else if (menuOption == '2')
    {
      
      float ds18b20TempC = ds18b20.readTempC();


      Serial.print(F("Temp: "));
      Serial.print(ds18b20TempC);
      Serial.println(F(" [C]"));
      lcd.print("Temp: ");
      lcd.print(ds18b20TempC);
      lcd.println(F(" [C]"));


     
      temperatureC = ds18b20TempC;


      String tempData = "Temperature: " + String(ds18b20TempC) + " [C]";
      bthc05.println(tempData);


    
      lcd.setCursor(0, 0);
      lcd.print("Temp: ");
      lcd.print(ds18b20TempC);
      lcd.print(F(" [C]"));


      delay(1000); 


      if (dataCount < maxDataPoints)
      {
        temperatureData[dataCount] = ds18b20TempC;
        dataCount++;
      }


    
      temperature();
    }
   
   
    if (millis() - time0 > timeout)
    {
      menuOption = getUserInput(); 
    }
  }
}


void temperature()
{
  lcd.setCursor(0, 1);
  if (temperatureC >= 36 && temperatureC <= 37)
  {
    lcd.print("Normal body temperature");
    bthc05.println("Normal body temperature");
  }
  else
  {
    if (temperatureC > 37)
    {
      lcd.print("Alert, High body temperature");
      bthc05.println("Alert, High body temperature");
    }
    else
    {
      lcd.print("Alert, Low body temperature");
      bthc05.println("Alert, Low body temperature");
    }


    stopFunction();
  }
}


void stopFunction()
{
 
  Serial.println("If the user requires to continue? Press 2 if Yes, 3 if No");
  lcd.print(" If the user requires to continue? Press 2 if Yes, 3 if No");
  bthc05.println(" If the user requires to continue? Press 2 if Yes, 3 if No");


  char c = 0;  
  while (Serial.available() == 0 && bthc05.available() == 0);


  if (Serial.available() > 0) {
    c = Serial.read();
    Serial.print("Serial Input: ");
    Serial.println(c);
  }
  else if (bthc05.available() > 0) {
    c = bthc05.read();
    Serial.print("Bluetooth Input: ");
    Serial.println(c);
  }


  if (c == '2') 
  {
    Serial.println("Continuing...");
    bthc05.println("Continuing...");
    // Continue by calling the loop function
    loop();
  }
  else if (c == '3')  
  {
    Serial.println("Stopping...");
    bthc05.println("Stopping...");
    menuOption = getUserInput(); 
    
  }
}


char getUserInput()
{ 
  Serial.println(F("\n Hi there. Would you like to start monitoring your body temperature? Press 1, 2, or 3 "));
  Serial.println(F("(1) Setting HC - 05 Bluetooth Serial Module"));
  Serial.println(F("(2) Monitor the real-time body temperature"));
  Serial.println(F("(3) Display stored temperature data"));
  Serial.println(F("(menu) send anything else or press the onboard reset button\n"));

  bthc05.println(F("\n Hi there. Would you like to start monitoring your body temperature? Press 1, 2, or 3 "));
  bthc05.println(F("(1) Setting HC - 05 Bluetooth Serial Module"));
  bthc05.println(F("(2) Monitor the real-time body temperature"));
  bthc05.println(F("(3) Display stored temperature data"));
  bthc05.println(F("(menu) send anything else or press the onboard reset button\n"));

  
  char c = 0;
  while (Serial.available() == 0 && bthc05.available() == 0);

  if (Serial.available() > 0) {
    c = Serial.read();
    Serial.print("Serial Input: ");
    Serial.println(c);
  } 
  else if (bthc05.available() > 0) {
    c = bthc05.read();
    Serial.print("Bluetooth Input: ");
    Serial.println(c);
  }

  if (isAlphaNumeric(c))
  {
    if (c == '1')
    {
      Serial.println(F("Now Setting up Bluetooth Communication"));
      bthc05.println(F("Now Setting up Bluetooth Communication"));
    }
    else if (c == '2')
    {
      Serial.println(F("Now Start to monitor the real-time body temperature"));
      bthc05.println(F("Now Start to monitor the real-time body temperature"));
    }
    else if (c == '3')
    {
      // Display stored temperature data
      Serial.println(F("Displaying stored temperature data:"));
      bthc05.println(F("Displaying stored temperature data:"));

      for (int i = 0; i < dataCount; i++)
      {
        Serial.print("Data Point ");
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(temperatureData[i]);
        Serial.println(" [C]");

        bthc05.print("Data Point ");
        bthc05.print(i + 1);
        bthc05.print(": ");
        bthc05.print(temperatureData[i]);
        bthc05.println(" [C]");
      }
    }
    else
    {
      Serial.println(F("Illegal input!"));
      bthc05.println(F("Illegal input!"));
      return 0;
    }

    time0 = millis();
    return c;
  }

  return 0;
}

