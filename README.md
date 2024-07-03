# S4D-ScriptingVoorDesigners

# Contact 
Minor Scripting Voor Designers (richard.holleman@han.nl)

# Folders
The folder breadboard_tester contains an Arduino test project for hardware with only a magnet sensor. 
The folder breadboard_tester_with_vol contains an Arduino test project for hardware with a magnet and volume sensor.

# s4d_breadboard header documentation

## Different hardware and simulation support

The s4d_breadboard.h is compatible  with multiple hardware versions and with a simulation. To switch between the different options, there are two macros: SIMULATION and HW_VERSION2.

For the first version of hardware comment out both macros, so it looks-like:
```
//#define SIMULATION
//#define HW_VERSION2
```
For the second version of hardware uncomment the HW_VERSION2 macro, so it looks-like:
```
//#define SIMULATION
#define HW_VERSION2
```
For the simulation uncomment the SIMULATION macro, so it looks-like:
```
#define SIMULATION
//#define HW_VERSION2
```
## Using the header

### Pin names
Below the pin names for the S4D board. Use these pin names with Arduino commands like analogRead(), digitalRead(), analogWrite() and digitalWrite().

- `MAGNETSENSOR`
- `POTENTIOMETER`
- `LIGHTSENSOR //only in simulation`
- `BUZZER`
- `LED_BLUE`
- `LED_GREEN`
- `LED_YELLOW`
- `LED_RED`

### Functions
Use the following function to initialize the breadboard. Always call this in your setup() function when using the breadboard or simulation.

- `initializeBreadboard()`


Use the following functions to check of a button is pressed.

-   `Button.Button1Pressed()`
-   `Button.Button2Pressed()`

Use the following function to get the sound volume in decibel.

-   `VolumeSensor.read()`

Use the following function to play a tone on the buzzer.

-   ` playTone( frequency, duration ) // duration is in milliseconds`

Use the following functions to print text to the OLED display or serial monitor (default in simulation).

- `OLED.clear()                      // Removes all pixels from the screen.`
- `OLED.copyToSerial()               // Everything printed to OLED will also be sent to the Serial Monitor.`
- `LED.print( value )                // Shows a value (text, number) on the screen.`
- `OLED.print( string, value )       // You can add a label or message before the value.`
- `OLED.printTop( value )            // Prints a smaller line in the upper half of the screen.`
- `OLED.printTop( string, value )    // You can add a label or message before the value.`
- `OLED.printBottom( value )         // Prints a smaller line in the lower half of the screen.`
- `OLED.printBottom( string, value ) // You can add a label or message before the value.`
