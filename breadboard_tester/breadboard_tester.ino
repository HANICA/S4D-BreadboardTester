/* * * * * * * * * * * * * * * * * * * * *
    
  How to test the breadboard:

  - turn the potmeter to dim the LEDs
  - press buttons to change the sensor 
    whose values are shown on the display:
  - press both buttons to set the sensor 
    to the magnet sensor.

 * * * * * * * * * * * * * * * * * * * * */

#include "s4d_breadboard.h"

// frequencies of musical notes taken from: https://pages.mtu.edu/~suits/notefreqs.html
const int REST = 0; 
const int NOTE_G_low = 196; 
const int NOTE_C = 262;
const int NOTE_E = 330;
const int NOTE_G = 392;

bool LEDsRunning = false;

void setup() {
  initializeBreadboard();
  Serial.println("setup");
  OLED.print("Setup");
  delay(300);
  simpleMelody();
  delay(300);
}

void loop() {
  showButtonStates();
  showSensorValues();
  animateLEDs();
  printSensorsToSerial();
}

void printSensorsToSerial() {
  Serial.print("potm: "); print4chars( analogRead(POTENTIOMETER) ); Serial.print( ", " );
  Serial.print("but1: "); Serial.print( digitalRead(BUTTON1) );     Serial.print( ", " );
  Serial.print("but2: "); Serial.print( digitalRead(BUTTON2) );     Serial.print( ", " );
  Serial.print("magn: "); print4chars( analogRead(MAGNETSENSOR) );  Serial.println();
}

void print4chars(int value) {
  String result = String(value);
  if(value < 10)   { Serial.print(" "); } 
  if(value < 100)  { Serial.print(" "); } 
  if(value < 1000) { Serial.print(" "); } 
  Serial.print(result);
}

void showSensorValues() {
  OLED.printTop("potentiometer:", analogRead(POTENTIOMETER));
  OLED.printBottom("magnet sensor:", analogRead(MAGNETSENSOR));
}


int showButtonStates() {
  if( areBothButtonsPressed() ) {
    playTone(NOTE_G ,20);
    OLED.print("Both buttons");
    Serial.println("Both buttons pressed.");
    delay(20);
    // wait for any button release
    while( areBothButtonsPressed() ) { /* do nothing */ }
    playTone(NOTE_E ,20);
  }
  else if( isPressed(BUTTON1) ) {
    playTone(NOTE_G, 20);
    LEDsRunning = true;
    animateLEDs();
    OLED.print("LEDs on");
    Serial.println("First button pressed.");
    delay(20);
    // wait for button 1 release
    while( isPressed(BUTTON1) ) { /* do nothing */ }
    playTone(NOTE_C ,20);
  }
  else if( isPressed(BUTTON2) ) {
    playTone(NOTE_G, 20);
    LEDsRunning = false;
    OLED.print("LEDs off");
    animateLEDs();
    Serial.println("Second button pressed.");
    delay(20);
    // wait for button 2 release
    while( isPressed(BUTTON2) ) { /* do nothing */ }
    playTone(NOTE_C ,20);
  }
}

bool isPressed(int buttonPin) {
  return digitalRead(buttonPin) == HIGH;
}

bool areBothButtonsPressed() {
  return isPressed(BUTTON1) && isPressed(BUTTON2);
}


const int LED_ALL  = 100;
const int LED_NONE = 101;

void switchToLED( int ledPin ) {
  analogWrite(LED_GREEN,  ledPin==LED_GREEN  || ledPin==LED_ALL ? analogRead(POTENTIOMETER) / 4 : 0 );
  analogWrite(LED_BLUE,   ledPin==LED_BLUE   || ledPin==LED_ALL ? analogRead(POTENTIOMETER) / 4 : 0 );
  analogWrite(LED_YELLOW, ledPin==LED_YELLOW || ledPin==LED_ALL ? analogRead(POTENTIOMETER) / 4 : 0 );
  analogWrite(LED_RED,    ledPin==LED_RED    || ledPin==LED_ALL ? analogRead(POTENTIOMETER) / 4 : 0 );
}

void animateLEDs() {
  if( !LEDsRunning ) {
    switchToLED(LED_NONE);
    return;
  }
  static int prevPhase = 0;
  long phase = (millis() / 150) % 6;
  if( phase == prevPhase ) {
    return;
  }
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
      switchToLED(LED_ALL);
  }
}

void simpleMelody() {
  playTone( NOTE_G_low, 120 );
  playTone( REST,        30 );
  playTone( NOTE_C,     120 );
  playTone( REST,        30 );
  playTone( NOTE_E,     150 );
  playTone( NOTE_G,     300 );
  playTone( REST,        30 );
  playTone( NOTE_E,     120 );
  playTone( NOTE_G,     750 );
}
