
/*
 * Copyright 2018 Pedro Azevedo (prgazevedo@gmail.com)
 * and other contributors as indicated by the @author tags.
 **********************************************************************
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include "_Definitions.h"
#include "_Global.h"
#include "images.h"
//OLED DEFINITIONS
//parameters: address,SDA,SCL 
SSD1306 display(0x3c, 4, 15); //display object 

int line=0;
void increaseLine(){
    if(line>40)line=0;
    else line+=10;
}


void OLED_clear(){
  display.clear();
}

void OLED_write(String toWrite){

    display.drawString(0, line, toWrite);
    increaseLine();
    //Serial.println("OLED_write called:"+toWrite);
    
    display.display(); //show
}

void OLED_write(int a, int b, String toWrite){
    display.drawString(a, b, toWrite);
    //Serial.println("OLED_write called:"+toWrite+ " at a:" +a+" b:"+b);
    display.display(); //show
}

void _OLED_print_data(){
  OLED_clear();
  if(_ROLE==0)OLED_write(0,0,"SenderV1." _SUBVERSION);
  else OLED_write(0,0,"RecvV1." _SUBVERSION "-"+gcurrent_ssid);
  OLED_write(78,0, "BAT:");
  OLED_write(108,0, sVBAT+"V");
  OLED_write(0,9,"Packet Id:");
  OLED_write(60,9,String(pdata.ID));
  OLED_write(0,18,"TimeStamp:");
  OLED_write(60,18,String(pdata.timeMillis));
  OLED_write(0,27,"Latitude:");
  OLED_write(60,27,gs_current_latitude);
  OLED_write(0,36,"Longitude: ");
  OLED_write(60,36,gs_current_longitude);
   if(_ROLE==0)
  { 
    OLED_write(0,45,"HDOP/SAT: ");
    OLED_write(60,45,gs_current_hdop+" / "+gs_current_satellites);
    OLED_write(0,54,"ALT./FIX: ");
    OLED_write(60,54,gs_current_altitude+" / "+ gs_current_fix_age);

  }

 if(_ROLE==1)
  {
    OLED_write(0,45,"RSSI/SNR: ");
    OLED_write(60,45,String(rssi_value)+"/");
    OLED_write(80,45,String(snr_value));
    OLED_write(0,54,"Connect to: ");
    OLED_write(60,54,global_wifi_IP);
  }

}

void _OLED_PUB_DATA(String pub){
    OLED_write(90,45,pub);

}

//LOGO only
void logo_sender(){
    OLED_write(0,50, "SENDER");
    /*
     display.drawXbm(21,0,FRAME_width,FRAME_height,frame1_bits);
    display.display();
   delay(FRAME_DELAY);
   display.clear(); 
   OLED_write(0,50, "SENDER");
   display.drawXbm(21,0,FRAME_width,FRAME_height,frame2_bits);
    display.display();
   delay(FRAME_DELAY);
   display.clear(); 
   OLED_write(0,50, "SENDER");
    display.drawXbm(21,0,FRAME_width,FRAME_height,frame3_bits);
     display.display();
    delay(FRAME_DELAY);
    display.clear(); 
    OLED_write(0,50, "SENDER");
    display.drawXbm(21,0,FRAME_width,FRAME_height,frame4_bits);
     display.display();
    delay(3*FRAME_DELAY); 
    */
    display.clear();
    
}

void logo_receiver(){
   OLED_write(0,50, "RECEIVER");
   /*
     display.drawXbm(21,0,FRAME_width,FRAME_height,frame4_bits);
    display.display();
   delay(FRAME_DELAY);
   display.clear(); 
   OLED_write(0,50, "RECEIVER");
   display.drawXbm(21,0,FRAME_width,FRAME_height,frame3_bits);
    display.display();
   delay(FRAME_DELAY);
   display.clear(); 
   OLED_write(0,50, "RECEIVER");
    display.drawXbm(21,0,FRAME_width,FRAME_height,frame2_bits);
     display.display();
    delay(FRAME_DELAY);
    display.clear(); 
    OLED_write(0,50, "RECEIVER");
    display.drawXbm(21,0,FRAME_width,FRAME_height,frame1_bits);
     display.display();
    delay(3*FRAME_DELAY); 
    */
    display.clear();
}

void logo(){

   display.clear();
   display.setFont(ArialMT_Plain_10); //large font
   OLED_write(0,54, "PWD Ver 1." _SUBVERSION " STARTING");
   //logo at "images.h"
   display.drawXbm(0,0,PWD_width,PWD_height,PWD_bits);
    display.display();
   delay(LOGO_DELAY);
   display.clear(); 
   if(_ROLE==0) logo_sender();
   else logo_receiver();
}



void _OledInit(){
  writeSerial("_OledInit");
  //OLED PIN OUT CFG
  pinMode(16,OUTPUT); //OLED PIN OUT
  digitalWrite(16, LOW);    // OLED RESET
  delay(50); 
  digitalWrite(16, HIGH); // With OLED ON, GPIO16 must be HIGH
  //INIT DISPLAY
  display.init();
  display.flipScreenVertically(); 
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10); //small font
  
  delay(1500);
  //LOGO STARTUP
  logo();  


}
