#include <DMD32.h> 
#include "fonts/SystemFont5x7.h"
#include "fonts/Arial_Black_16_ISO_8859_1.h"


//Fire up the DMD library as dmd
#define DISPLAYS_ACROSS 1
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);
   //Timer setup : create a hardware timer  of ESP32
hw_timer_t * timer = NULL;

//----------------------------------- end -------------------------------------------------------------

void IRAM_ATTR triggerScan()
{
  dmd.scanDisplayBySPI();
}

String myName="KiK KOLEJ KOMUNITI SEBERANG JAYA PULAU PINANG";

void setup(void)
{
     
// return the clock speed of the CPU
  uint8_t cpuClock = ESP.getCpuFreqMHz();

//-----------------DMD setup---------------------
  // Use 1st timer of 4 
  // devide cpu clock speed on its speed value by MHz to get 1us for each signal  of the timer
  timer = timerBegin(0, cpuClock, true);
  // Attach triggerScan function to our timer 
  timerAttachInterrupt(timer, &triggerScan, true);
  // Set alarm to call triggerScan function  
  // Repeat the alarm (third parameter) 
  timerAlarmWrite(timer, 300, true);
  // Start an alarm 
  timerAlarmEnable(timer);
   //clear/init the DMD pixels held in RAM
   dmd.clearScreen( true );   //true is normal (all pixels off), false is negative (all pixels on)
//-----------------DMD setup END--------------------- 
  
  Serial.begin(115200);
}

void loop(void)
{
   if (Serial.available()>0) {  
      Serial.println("Please enter your String: "); //Prompt User for input
       myName=Serial.readString(); 
  }
 String textToScroll = myName;
 drawText(textToScroll);  
}

void drawText(String dispString)
{
  dmd.clearScreen( true );
  dmd.selectFont(Arial_Black_16_ISO_8859_1);
  char newString[256];
  int sLength = dispString.length();
  dispString.toCharArray( newString, sLength+1 );
  dmd.drawMarquee(newString,sLength,( 32*DISPLAYS_ACROSS )-1 , 0 );
  long start=millis();
  long timer=start;
  long timer2=start;
  boolean ret=false;
  while(!ret){
    if ( ( timer+50 ) < millis() ) {
      ret=dmd.stepMarquee( -1 , 0 );
      timer=millis(); }
  }
}
