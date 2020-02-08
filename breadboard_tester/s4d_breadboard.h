
#include <Arduino.h>
#include <U8g2lib.h>
void initializeOLED(bool enable = true);


/* * * * * * * * * * * * * * * * * * *
   
    Below the pin names for the
    S4D board.
    Use these pin names with
    Arduino commands like
    analogRead(), digitalRead(),
    analogWrite() and digitalWrite()
                                    
 * * * * * * * * * * * * * * * * * * */

const int VOLUMESENSOR = 0;    // analog input
const int MAGNETSENSOR = 1;    // analog input
const int POTENTIOMETER = 2;   // analog input
const int LIGHTSENSOR = 3;     // analog input

const int BUTTON1 = 4;         // digital input
const int BUTTON2 = 3;         // digital input

const int BUZZER = 6;          // digital output

const int LED_BLUE = 5;        // analog and digital output
const int LED_GREEN = 9;       // analog and digital output
const int LED_YELLOW = 10;     // analog and digital output
const int LED_RED = 11;        // analog and digital output

/* * * * * * * * * * * * * * * * * * *

  The following code initializes 
  all pins for use with the breadboard.

  Always call initializeBreadboard()
  in your setup() function when using
  the breadboard

* * * * * * * * * * * * * * * * * * * */

void initializeBreadboard() {

  pinMode(BUZZER, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);

  pinMode(VOLUMESENSOR, INPUT);
  pinMode(MAGNETSENSOR, INPUT);
  pinMode(POTENTIOMETER, INPUT);
  pinMode(LIGHTSENSOR, INPUT);
      
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_RED, LOW);

  Serial.begin(9600);
  initializeOLED();
}


/* * * * * * * * * * * * * * * * * * *

  The code below creates the following
  commands:
    
    OLED.clear()
    
    OLED.print( value )  
    OLED.print( string, value )       // you can add a label or message before the value;

    OLED.printTop( value )            // prints a smaller line in the upper half of the screen;
    OLED.printTop( string, value )    // you can add a label or message before the value;

    OLED.printBottom( value )         // prints a smaller line in the lower half of the screen;
    OLED.printBottom( string, value ) // you can add a label or message before the value;

  The value for OLED.print() can be 
  a text or a number.
    
 * * * * * * * * * * * * * * * * * * */

#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

// This is some ugly code, required to get the display to work.
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0,
    /* reset=*/ U8X8_PIN_NONE,
    /* clock=*/ SCL,
    /* data=*/  SDA);

class OledClass {

  public:
    bool enabled = true;
    bool lastPrintWasSmall = false; // if last print was half screen, next small print should clear only half the screen.
  
    OledClass() {
    }
    void print(String text) {
      if(!enabled) {
        return;
      }
      lastPrintWasSmall = false;
      char tempCharBuffer[20];
      text.toCharArray(tempCharBuffer, 20);
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_helvR14_tr);
      u8g2.drawStr(0, 20, tempCharBuffer);
      u8g2.sendBuffer();
    }
    void print(int number ) {
      String text = String(number);
      print(text);
    }
    void print(double number ) {
      String text = String(number);
      print(text);
    }
    void print(String label, long number) {
      String text = String(label) + " " + String(number);
      print(text);
    }
    void print(String label, double number) {
      String text = String(label) + " " + String(number);
      print(text);
    }
    void printSmallLine(String text, int line) {
      char tempCharBuffer[30];
      text.toCharArray(tempCharBuffer, 30);
      if( ! lastPrintWasSmall ) {   
        // clear the entire screen
        u8g2.clearBuffer();
      } else {
        // just clear the line we're going to print on
        u8g2.setDrawColor(0);
        u8g2.drawBox( 0, 16*line, 128, 16);
        u8g2.setDrawColor(1);
      }
      u8g2.setFont(u8g2_font_helvR10_tr);
      u8g2.drawStr(0, 16*(line+1)-1, tempCharBuffer);
      u8g2.sendBuffer();
      lastPrintWasSmall = true; 
    }
    void printTop(String text) {
      printSmallLine( text, 0 );
    }
    void printTop(int number) {
      String text = String(number);
      printTop(text);
    }
    void printTop(double number ) {
      String text = String(number);
      printTop(text);
    }
    void printTop(String label, int number) {
      String text = String(label) + " " + String(number);
      printTop(text);
    }
    void printTop(String label, double number) {
      String text = String(label) + " " + String(number);
      printTop(text);
    }
    void printBottom(String text) {
      printSmallLine( text, 1 );
    }
    void printBottom(int number ) {
      String text = String(number);
      printBottom(text);
    }
    void printBottom(double number ) {
      String text = String(number);
      printBottom(text);
    }
    void printBottom(String label, int number) {
      String text = String(label) + " " + String(number);
      printBottom(text);
    }
    void printBottom(String label, double number) {
      String text = String(label) + " " + String(number);
      printBottom(text);
    }
    void clear() {
      print("");
    }
};


OledClass OLED;

// enable argument is optional.
// default value for enable = true (see declaration at top of this file)
void initializeOLED(bool enable) {
  if(!enable) {
    OLED.enabled = false;
    return;
  }
  u8g2.begin();
  OLED.clear();
}

/* * * * * * * * * * * * * * * * * * *

  The code below creates the following
  command:
  
  VolumeSensor.read()
  
  Normal background noise should result in 
  about 48db. If VolumeSensor.read()
  returns a different value for silence
  (say, 60), you can adjust it like this:

    VolumeSensor.calibrationOffset = -12 

  (this would adjust value 60 to 48)
  
 * * * * * * * * * * * * * * * * * * */


class VolumeSensorClass {

  public:
    int micSamples = 512; 
    int calibrationOffset = 0; 


    int getAverageDB() {
      long totalSoundReadings = 0;

      for (int i = 0; i < micSamples; i++) {
        int reading = analogRead(VOLUMESENSOR);
        int amp = abs(reading - 512);
        totalSoundReadings += ((long)amp * amp);
      }

      int averageSoundReadings = totalSoundReadings / micSamples;
      float soundVolumeRMS = sqrt(averageSoundReadings); //rms = root mean squared sound pressure
      int dB = 20.0 * log10(soundVolumeRMS / 512) + 100 + calibrationOffset;
      return dB;
    }

    //Method returns volume. 
    int read() {
      analogReference(EXTERNAL);
      int averageDB = getAverageDB();
      if (averageDB != 0) {
        analogReference(INTERNAL);
        return averageDB;
      } else {
        return read();
      }
    }
};

VolumeSensorClass VolumeSensor;
