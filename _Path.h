#include <SimpleList.h>
#include "_Global.h"

SimpleList<gpsPath> myList;


void _addCoordsToPath(String lat, String lng){
  gpsPath gPath;
  gPath.lat = lat;
  gPath.lng = lng;
  myList.push_back(gPath);
  Serial.println("_addCoordsToPath added: lat: "+lat+" lng: "+lng);
  if(bFirst)
  {
    bFirst=false;
    gs_rcv_latitude=lat;
    gs_rcv_longitude=lng;
  }
}

void _encodePath(){
  
  gspath="";
  for (SimpleList<gpsPath>::iterator itr = myList.begin(); itr != myList.end(); )
  {
    gspath+=("{lat: "+((gpsPath)(*itr)).lat+", lng: "+((gpsPath)(*itr)).lng+"}");
    ++itr;
    if(itr != myList.end()){gspath+=",";}
  }
  Serial.println("_encodePath has Path:"+gspath);
      
}
