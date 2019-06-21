
#ifndef _LED_H
#define _LED_H

void _LEDBlink(){
  digitalWrite(25, HIGH);   // LED ON
  delay(10);                       // wait Xms
  digitalWrite(25, LOW);    // LED OFF
  delay(5);                       // wait Xms
}

#endif
