#include <WiFi.h>
#include <PubSubClient.h>
#include <DMD32.h> 
#include "fonts/SystemFont5x7.h"
#include "fonts/Arial_Black_16_ISO_8859_1.h"



//Fire up the DMD library as dmd
#define DISPLAYS_ACROSS 1
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);
   //Timer setup : create a hardware timer  of ESP32
hw_timer_t * timer = NULL;

//------WIFI AZA---
const char* ssid = "Alias FredoX_2.4GHz@unifi";  
const char* password = "0135335045";
const char *topic = "citra/exam"; // SET TOPIC !!!

WiFiClient espClient;                                                                          
PubSubClient client(espClient); //MQTT
const char* mqtt_server = "broker.hivemq.com";

//----------------------------------- end -------------------------------------------------------------

//---- PEMBOLEHUBAH DATA---------------------------------------------------------------------------------
long lastMsg = 0;
char msg[50];
int value = 0;
//----------------------------------- end -------------------------------------------------------------

void IRAM_ATTR triggerScan()
{
  dmd.scanDisplayBySPI();
}

String myName;
String MessageTemp = "KK Seberang Jaya --- KIK Horizon Baharu";

void setup(void)
{
  Serial.begin(115200);
  delay(100);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  delay(100);  
  
// return the clock speed of the CPU
  uint8_t cpuClock = ESP.getCpuFreqMHz();

//-----------------DMD setup---------------------
  // Use 1st timer of 4 : devide cpu clock speed on its speed value by MHz to get 1us for each signal  of the timer
  timer = timerBegin(0, cpuClock, true);                 
  timerAttachInterrupt(timer, &triggerScan, true);     // Attach triggerScan function to our timer
  timerAlarmWrite(timer, 300, true);                   // Set alarm to call triggerScan function . Repeat the alarm (third parameter) 
  
  timerAlarmEnable(timer);// Start an alarm 
  //clear/init the DMD pixels held in RAM
   dmd.clearScreen( true );   //true is normal (all pixels off), false is negative (all pixels on)
//-----------------DMD setup END--------------------- 
  
  
}
void loop(void)
{
   if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();

  if (now - lastMsg > 5000) {  
    lastMsg = now;
  
   drawText(MessageTemp);
  }
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
