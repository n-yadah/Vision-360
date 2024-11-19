#include "IRremote.h"
#include "IR.h"

IRrecv irrecv(RECEIVER); 
decode_results results; 

void setup() {
  
  Serial.begin(115200);
  Serial.println("IR Receiver Button Decode");
  irrecv.enableIRIn();
  

}

void loop() {
  
  int tmpValue;

  
  if (irrecv.decode(&results)) // have we received an IR signal?
  {
    for (int i = 0; i < 23; i++)
    {
      if ((keyValue[i] == results.value) && (i<KEY_NUM))
      {
        Serial.println(keyBuf[i]);
        tmpValue = results.value;
        
      }
      else if((results.value==4294967295) && (REPEAT==i))
      {
        Serial.println("Repeat...");
      }
    }
    irrecv.resume(); // receive the next value
  }
  delay(400); //Delay to ensure that a repeat is not issued for every button press

}
