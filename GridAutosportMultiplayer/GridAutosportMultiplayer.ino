//Exercise bike configured for Grid Autosport Multiplayer.
/* ESP32 Gamepad library, https://github.com/lemmingDev/ESP32-BLE-Gamepad */
#include <BleGamepad.h>

BleGamepad bleGamepad;

/*count variable declared as volatile, as it is being accessed by both interrupt service routine and main program. 
So the compiler is directed to load variable from RAM instead of registers. */
volatile int count = 0;
volatile int frequency;

/*pin connected to exercise bike magnetic switch*/
#define PEDALINPUTPIN 23

/*variable to calculate gamepad trigger value from pedal count*/
unsigned long triggerVal;
/*variable to keep track of the mainloop*/
int loopcount = 0;

/*var of portMUX_TYPE takes care of synchronization between main loop and interrupt service routine*/
portMUX_TYPE synch = portMUX_INITIALIZER_UNLOCKED;

/*Interrupt Service Routine, IRAM_ATTR specifies isr should be in RAM, it is faster than flash*/
void IRAM_ATTR isr() {
  portENTER_CRITICAL(&synch); /*macros to disable further interrupts or change priority*/
  count++;           /*Increment count*/
  portEXIT_CRITICAL(&synch);
}


void setup() {
  
  pinMode(PEDALINPUTPIN, INPUT_PULLUP); /*The pedal input pin attached to the magnetic switch is pulled up.*/
  attachInterrupt(PEDALINPUTPIN,isr,FALLING); /*Calls the ISR everytime a falling edge on the pedal signal is detected*/ 

  pinMode(27, INPUT); /*The touch modules are connected to the following ESP32 GPIO pins*/
  pinMode(14, INPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);
  
  //Serial.begin(115200);  //Serial communication used for debuging code.
  bleGamepad.begin();
}

void loop(){
  if(bleGamepad.isConnected()) 
  { 
    if (digitalRead(12) == HIGH) //Touchpad1
    {
      bleGamepad.press(BUTTON_1); //xbox button a
      //Serial.println("Touchpad1 pressed");
    }
    else
    {
      bleGamepad.release(BUTTON_1); 
    }
    if (digitalRead(13) == HIGH)  //Touchpad2.
    {
      bleGamepad.setX(-32767); //Left stick set to left most position.
      //Serial.println("Touchpad2 pressed");
    }
    else if (digitalRead(14) == HIGH)//Touchpad4.
    { 
      bleGamepad.setX(32767); //Left stick set to right most position.
      //Serial.println("Touchpad4 pressed"); 
    }
    else
    {
      bleGamepad.setX(0);  //Default center position.
    }
    if (digitalRead(27) == HIGH)//Touchpad3.
    {
      bleGamepad.press(BUTTON_2); //xbox b button
      //Serial.println("Touchpad3 pressed");
    }
    else
    {
      bleGamepad.press(BUTTON_2);
    } 
    
    if (frequency > 1){ /*Accelerate even when a single pulse is detected, in android the trigger acts a button, 
                          pressing the button longer increases the acceleration.*/
      bleGamepad.press(BUTTON_9); 
      //Serial.println("Accelerate!");
    }
    delay(50);
    bleGamepad.release(BUTTON_9); /*Release the trigger every 50 milliseconds.*/
    loopcount++;
    if (loopcount == 13){ /*Reset the pedal count every 650 milisecond. This ensures that the trigger is not immediately released.*/
     frequency = count;
     count = 0;
     loopcount = 0;
    }
  }
} 
