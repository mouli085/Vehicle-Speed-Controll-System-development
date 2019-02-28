 // Include RadioHead Amplitude Shift Keying Library
#include <RH_ASK.h>
// Include dependant SPI Library 
#include <SPI.h> 
#include <LiquidCrystal.h> // includes the LiquidCrystal Library 
LiquidCrystal lcd(A2, A3, 10, 5, 6, 7); // Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7) 

 RH_ASK rf_driver;
 int half_revolutions = 0;
const float Pi = 3.14159;
 int rpm = 0;

 unsigned long lastmillis = 0;
  unsigned long last = 0;
uint8_t buf[4];
    
    uint8_t buflen = sizeof(buf);   int a=10000; int count = 0;

    
 void setup(){
 rf_driver.init();
 attachInterrupt(0, rpm_fan, FALLING);
 lcd.begin(16,2);pinMode(9, OUTPUT);pinMode(13, OUTPUT);
 }

void get_msg(){
  
   if (rf_driver.recv(buf, &buflen))
    {
     
      // Message received with valid checksum
  
      a = atoi(buf);
      
       lcd.setCursor(0,1);
       lcd.print("received : "); 
       lcd.setCursor(11,1); // Sets the location at which subsequent text written to the LCD will be displayed 
       lcd.print(String(a)); // Prints "Arduino" on the LCD 
       delay(800); // 3 seconds delay    ; 
       
       lcd.clear();    
    }else
    delay(800);lcd.clear();
    
  }

 void loop(){

 if (millis() - lastmillis == 1000){ //Uptade every one second, this will be equal to reading frecuency (Hz).

        detachInterrupt(0);//Disable interrupt when calculating     
        rpm = half_revolutions * 2 * Pi * 0.1 ; // Convert frecuency to RPM, note: this works for one interruption per full rotation. For two interrups per full rotation use half_revolutions * 30.
        
        lcd.setCursor(0,0);lcd.print("speed(m/s):");lcd.setCursor(11,0);
        lcd.print(String(rpm));
        
        get_msg();
        if(count == 0){
            if(a<rpm){
                    lcd.setCursor(0,0);lcd.print("DECREASE SPEED ");digitalWrite(13,HIGH);
                    delay(5000); // 5 seconds delay; 
                    count = 1; lcd.clear();
            }
            else
              count = 0;
         }
         else if(count == 1){
            if(a<rpm){
               lcd.clear();
               lcd.setCursor(0,0);lcd.print("PLEASE STOP ");digitalWrite(13,LOW);digitalWrite(9,HIGH); delay(2000); lcd.clear();
            }
            {
              count=0;
              lcd.clear(); lcd.print("SPEED OK");digitalWrite(13,LOW);digitalWrite(9,LOW); lcd.clear();
            }
         }
       
         half_revolutions = 0; // Restart the RPM counter
         rpm = 0;
         lastmillis = millis(); // Uptade lasmillis
         attachInterrupt(0, rpm_fan, FALLING); //enable interrupt
         
        }
   

 }

 // this code will be executed every time the interrupt 0 (pin2) gets low.

 void rpm_fan(){
  half_revolutions++;
 }
