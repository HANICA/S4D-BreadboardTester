/* * * * * * * * * * * * * * * * * * * * *
    
  How to test the breadboard:

  - turn the potmeter to dim the LEDs
  - press buttons to change the sensor 
    whose values are shown on the display:
  - press both buttons to set the sensor 
    to the magnet sensor.

 * * * * * * * * * * * * * * * * * * * * */

#include "s4d_breadboard.h"


void setup() {
  initializeBreadboard();
  Serial.println("setup");
  OLED.print("Setup");
  delay(300);
  simpleMelody();
  delay(300);
}

void loop() {
  static int currentSensor = MAGNETSENSOR;
  int selectedSensor = sensorSelectedByButton();
  if (selectedSensor && selectedSensor != currentSensor) {
    currentSensor = selectedSensor;
    writeCurrentSensorType(currentSensor);
    buzzerTest(30,1000);
    delay(300);
  }
  performActionBasedOn(currentSensor);
  mapPotToLEDBrightness();
  // printSensorsToSerial();
}

void printSensorsToSerial() {
  Serial.print("pm: "); print4chars( analogRead(POTENTIOMETER) ); Serial.print( ", " );
  Serial.print("b1: "); Serial.print( digitalRead(BUTTON1) );     Serial.print( ", " );
  Serial.print("b2: "); Serial.print( digitalRead(BUTTON2) );     Serial.print( ", " );
  Serial.print("ls: "); print4chars( analogRead(LIGHTSENSOR) );   Serial.print( ", " );
  Serial.print("vs: "); print4chars( analogRead(VOLUMESENSOR) );  Serial.print( ", " );
  Serial.print("ms: "); print4chars( analogRead(MAGNETSENSOR) );  Serial.println();
}

void print4chars(int value) {
  String result = String(value);
  if(value < 10)   { Serial.print(" "); } 
  if(value < 100)  { Serial.print(" "); } 
  if(value < 1000) { Serial.print(" "); } 
  Serial.print(result);
}

// Maps actions to buttons (not pinnumbers!)
void performActionBasedOn(int sensorPin) {
  switch (sensorPin) {
    case VOLUMESENSOR:
      OLED.printBottom("volume", VolumeSensor.read());
      break;
    case LIGHTSENSOR:
      OLED.printBottom("light", analogRead(LIGHTSENSOR));
      break;
    case MAGNETSENSOR:
      OLED.printBottom("magnet", analogRead(MAGNETSENSOR));
      break;
  }
}

void writeCurrentSensorType(int sensorPin) {
  switch (sensorPin) {
    case VOLUMESENSOR:
      OLED.print("Volume sensor");
      break;
    case LIGHTSENSOR:
      OLED.print("Light sensor");
      break;
    case MAGNETSENSOR:
      OLED.print("Magnet sensor");
      break;
  }
}

// Returns a number corresponding with the button that was touched.
// 0 = left, 1 = middle, 2 = right, -1 on no touch
int sensorSelectedByButton() {
  int selectedSensor = 0;
  if (testButton(BUTTON2) == 1 && testButton(BUTTON1) == 1) {
    Serial.println("Both buttons pressed.");
    selectedSensor = VOLUMESENSOR;
  }
  else if (testButton(BUTTON1) == 1) {
    Serial.println("First button pressed.");
    selectedSensor = LIGHTSENSOR;
  }
  else if (testButton(BUTTON2) == 1) {
    Serial.println("Second button pressed.");
    selectedSensor = MAGNETSENSOR;
  }
  return selectedSensor;
}

void switchToLED( int ledPin ) {
  analogWrite(LED_GREEN,  ledPin == LED_GREEN ?  analogRead(POTENTIOMETER) / 4 : 0 );
  analogWrite(LED_BLUE,   ledPin == LED_BLUE ?   analogRead(POTENTIOMETER) / 4 : 0 );
  analogWrite(LED_YELLOW, ledPin == LED_YELLOW ? analogRead(POTENTIOMETER) / 4 : 0 );
  analogWrite(LED_RED,    ledPin == LED_RED ?    analogRead(POTENTIOMETER) / 4 : 0 );
}

// Adjusts the brightness of the LED based on the potentiometer
void mapPotToLEDBrightness() {
  static int prevPhase = 0;
  long phase = (millis() / 150) % 6;
  if( phase == prevPhase ) {
    return;
  }
  OLED.printTop("potentiometer", analogRead(POTENTIOMETER));
  prevPhase = phase;
  switch(phase) {
    case 0:
      switchToLED(LED_GREEN);   
      break;
    case 1:
    case 5:
      switchToLED(LED_BLUE);
      break;
    case 2:
    case 4:
      switchToLED(LED_YELLOW);
      break;
    case 3:
      switchToLED(LED_RED);
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

// Function that returns the status of a button
int testButton(int buttonPin) {
  int status = digitalRead(buttonPin);
  return status;
}

void playTone(int tone, int duration) {
  if(tone == 0) {  // PAUSE
    delay(duration);
    Serial.print("pause!");
    Serial.println(duration);
  } else {
    for (long i = 0; i < duration * 1000L - 10000; i += tone * 2) {
      digitalWrite(BUZZER, HIGH);
      delayMicroseconds(tone);
      digitalWrite(BUZZER, LOW);
      delayMicroseconds(tone);
    }
    delayMicroseconds(10000);  // very short pause so the listener can tell notes apart.
  }
}

void simpleMelody() {
  playTone( 1911,   100 ); // play middle-C for 100 milliseconds;
  playTone( 1517,   100 ); // play E for 100 milliseconds;
  playTone( 1276,   100 ); // play G for 100 milliseconds;
  playTone( 1517,   100 ); // play E for 100 milliseconds;
  playTone( 1911,   200 ); // play C for 200 milliseconds;
}
