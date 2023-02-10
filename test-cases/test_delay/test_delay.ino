#include "constants.h"
#include "delayHelper.h"

DelayHelper delayHelper(1000);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(SERIAL_BAUDRATE);
  Serial.println("Hello motherfucker");

  Serial.println(millis());
  delayHelper.startTimer();

}

void loop() {
  // put your main code here, to run repeatedly
  delay(20);
  Serial.print(millis());
  Serial.print(": ");
  Serial.print(delayHelper.checkTimer());
  Serial.print("\n");

  long fuckyou = millis();
  if(fuckyou > 1450 && fuckyou < 1550){
    delayHelper.startTimer();
  }
}
