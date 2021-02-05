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
  Serial.begin(9600);
  Serial.println("setup");

  initializeBreadboard();
  Serial.println("setup");
  OLED.print("running \"setup\"");
  delay(750);
  OLED.printTop("Press button 1 to");
  OLED.printBottom("turn LEDs on.");
  simpleMelody();
  delay(200);
  OLED.print("starting \"loop\"");
  delay(750);
}

void loop() {
  showButtonStates();
  showSensorValues();
  animateLEDs();
  printSensorsToSerial();
}

void showSensorValues() {
  OLED.printTop("potentiometer:", analogRead(POTENTIOMETER));
  OLED.printBottom("magnet sensor:", analogRead(MAGNETSENSOR));
}

void printSensorsToSerial() {
  Serial.print("potmeter: "); print4chars( analogRead(POTENTIOMETER) );                       Serial.print( ", " );
  Serial.print("button-1: "); Serial.print( digitalRead(BUTTON1) == HIGH ? "HIGH" : "LOW " ); Serial.print( ", " );
  Serial.print("button-2: "); Serial.print( digitalRead(BUTTON2) == HIGH ? "HIGH" : "LOW " ); Serial.print( ", " );
  Serial.print("magnet-sensor: "); print4chars( analogRead(MAGNETSENSOR) );                   Serial.println();
  delay(50);
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

int showButtonStates() {
  if( areBothButtonsPressed() ) {
    playTone(NOTE_G ,20);
    OLED.print("Both buttons");
    delay(20);
    // wait for any button release
    while( areBothButtonsPressed() ) { 
      /* do nothing except: */ 
      printSensorsToSerial();
    }
  }
  else if( isPressed(BUTTON1) ) {
    playTone(NOTE_G, 20);
    LEDsRunning = true;
    animateLEDs();
    OLED.print("LEDs on");
    delay(20);
    // wait for button 1 release
    while( isPressed(BUTTON1) ) { 
      /* do nothing except: */ 
      printSensorsToSerial();
    }
    playTone(NOTE_C ,20);
  }
  else if( isPressed(BUTTON2) ) {
    playTone(NOTE_G, 20);
    LEDsRunning = false;
    OLED.print("LEDs off");
    animateLEDs();
    delay(20);
    // wait for button 2 release
    while( isPressed(BUTTON2) ) { 
      /* do nothing except: */ 
      printSensorsToSerial();
    }
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
