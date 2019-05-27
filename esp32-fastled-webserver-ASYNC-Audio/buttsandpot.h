#define MAX_BRIGHTNESS 255      // Thats full on, watch the power!
#define MIN_BRIGHTNESS 0       // set to a minimum of 25%

int potpin=36;
//int buttonpin=A0;
int potval=0;
//int buttonval=0;
int writeval=0;
int potvalfix=0;
int rawValue;
int oldValue;
byte potPercentage;
byte oldPercentage;
// Setup a new OneButton on pin A1.  
OneButton button(39,true);
//OneButton button2(A3,true);


void readvals(){
// buttonval=analogRead(buttonpin);
   potval=analogRead(potpin);
  if (12>potval){
     FastLED.setBrightness(constrain(0, MIN_BRIGHTNESS, MAX_BRIGHTNESS));
     Serial.println("off");
  }

   if (potval < (oldValue - 50) || potval > (oldValue + 50)) {
    oldValue = potval;
    // convert to percentage
    potPercentage = map(oldValue, 0, 1023, 0, 100); // percentage of the brightness!
    potvalfix = map(potval, 0, 1023, 0, 255); // value of 
    // Only print if %value changes
    if (oldPercentage != potPercentage) {
      Serial.print("Pot percentage is: ");
      Serial.print(potPercentage);
      Serial.println(" %");
      oldPercentage = potPercentage;
    }
//  Serial.println(constrain(potvalfix, MIN_BRIGHTNESS, MAX_BRIGHTNESS));
  FastLED.setBrightness(constrain(potvalfix, MIN_BRIGHTNESS, MAX_BRIGHTNESS));
  Serial.println(potval);
  }
//  Serial.println(buttonval);
 

}
