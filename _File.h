#ifndef _FILE_H
#define _FILE_H


#include "SPIFFS.h"
 
void _Fileinit() {
    Serial.println("_Fileinit");
 
     if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  File root = SPIFFS.open("/");
 
  File file = root.openNextFile();
 
  while(file){
 
      Serial.print("FILE: ");
      Serial.println(file.name());
 
      file = root.openNextFile();
  }
}

String _readPassword(){
  String password = "";
  File file = SPIFFS.open("/password.txt");
  if(!file){
    Serial.println("Failed to open file for reading");
    return "";
  }

  
  Serial.println("Password file open:");
  while(file.available()){
    password+=(char)(file.read());
  }
  file.close();
  return password;
}

#endif
