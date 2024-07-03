#include "s4d_breadboard.h"

int buttonThatWasLastTouched = 3;

void setup() {
  Serial.begin(9600);
  Serial.println("setup");

  initializeBreadboard();

  simpleMelody();
}

void loop() {
  int currentButton = theButtonThatWasTouched();
  if (currentButton != -1) {
    buzzerTest(30,1000);
    writeCurrentSensorType(currentButton);
    buttonThatWasLastTouched = currentButton;
  }
  performActionBasedOn(buttonThatWasLastTouched);
  mapPotToLEDBrightness();
  printSensorsToSerial();
}

void printSensorsToSerial() {
  Serial.print("pm: "); print4chars( analogRead(POTENTIOMETER) ); Serial.print( ", " );
  Serial.print("b1: "); Serial.print( Button.button1Pressed() ); Serial.print( ", " );
  Serial.print("b2: "); Serial.print(Button.button2Pressed()  ); Serial.print( ", " );
  Serial.print("ls: "); print4chars( analogRead(LIGHTSENSOR) ); Serial.print( ", " );
  Serial.print("vs: "); print4chars( VolumeSensor.read() ); Serial.print( ", " );
  Serial.print("ms: "); print4chars( analogRead(MAGNETSENSOR) ); Serial.println();
}

void print4chars(int value) {
  String result = String(value);
  if(value < 10)   { Serial.print(" "); } 
  if(value < 100)  { Serial.print(" "); } 
  if(value < 1000) { Serial.print(" "); } 
  Serial.print(result);
}



// Maps actions to buttons (not pinnumbers!)
void performActionBasedOn(int button) {
  switch (button) {
    case 0:
      OLED.print("volume", VolumeSensor.read());
      break;
    case 1:
      OLED.print("light", analogRead(LIGHTSENSOR));
      break;
    case 3:
      OLED.print("magnet", analogRead(MAGNETSENSOR));
      break;
  }
}

void writeCurrentSensorType(int button) {
  switch (button) {
    case 0:
      OLED.print("Volume sensor");
      break;
    case 1:
      OLED.print("Light sensor");
      break;
    case 3:
      OLED.print("Magnet sensor");
      break;
  }
  delay(300);
}

// Returns a number corresponding with the button that was touched.
// 0 = left, 1 = middle, 2 = right, -1 on no touch
int theButtonThatWasTouched() {
  int buttonTouched = -1;
  if (Button.button2Pressed() == 1 && Button.button1Pressed() == 1) {
    Serial.println("both were Pressed");
    buttonTouched = 3;
  }
  else if (Button.button1Pressed() == 1) {
    Serial.println("first button Pressed");
    buttonTouched = 0;
  }
  else if (Button.button2Pressed() == 1) {
    Serial.println("second button Pressed");
    buttonTouched = 1;
  }

  return buttonTouched;
}

// Adjusts the brightness of the LED based on the potentiometer
int prevPhase = 0;
void mapPotToLEDBrightness() {
  long phase = (millis() / 150) % 6;
  if( phase == prevPhase ) {
    return;
  }
  prevPhase = phase;
  switch(phase) {
    case 0:
      analogWrite(LED_BLUE, analogRead(POTENTIOMETER) / 4 );
      digitalWrite(LED_GREEN, LOW );   
      digitalWrite(LED_YELLOW, LOW );
      digitalWrite(LED_RED, LOW );
      break;
    case 1:
    case 5:
      digitalWrite(LED_BLUE, LOW );
      analogWrite(LED_GREEN, analogRead(POTENTIOMETER) / 4 );   
      digitalWrite(LED_YELLOW, LOW );
      digitalWrite(LED_RED, LOW );
      break;
    case 2:
    case 4:
      digitalWrite(LED_BLUE, LOW );
      digitalWrite(LED_GREEN, LOW );   
      analogWrite(LED_YELLOW, analogRead(POTENTIOMETER) / 4 );
      digitalWrite(LED_RED, LOW );
      break;
    case 3:
      digitalWrite(LED_BLUE, LOW );
      digitalWrite(LED_GREEN, LOW );   
      digitalWrite(LED_YELLOW, LOW );
      analogWrite(LED_RED, analogRead(POTENTIOMETER) / 4 );
      break;
    default:
      analogWrite(LED_BLUE, analogRead(POTENTIOMETER) / 4 );
      analogWrite(LED_GREEN, analogRead(POTENTIOMETER) / 4 );   
      analogWrite(LED_YELLOW, analogRead(POTENTIOMETER) / 4 );
      analogWrite(LED_RED, analogRead(POTENTIOMETER) / 4 );
  }
}

// Procedure to test the buzzer
// duration in milliseconds
void buzzerTest(int duration, int tone) {
  Serial.println("Buzzer...");
  for (int i = 0; i < duration; i++) {
    digitalWrite(BUZZER, HIGH);
    delayMicroseconds(tone);
    digitalWrite(BUZZER, LOW);
    delayMicroseconds(tone);
  }
}

void simpleMelody() {
  playTone( 1911,   100 ); // play middle-C for 100 milliseconds;
  playTone( 1517,   100 ); // play E for 100 milliseconds;
  playTone( 1276,   100 ); // play G for 100 milliseconds;
  playTone( 1517,   100 ); // play E for 100 milliseconds;
  playTone( 1911,   200 ); // play C for 200 milliseconds;
}
