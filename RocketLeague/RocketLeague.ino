//Exercise bike configured for rocket league.
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
  count++;                    /*Increment count*/ 
  portEXIT_CRITICAL(&synch);
}


void setup() {
 
  pinMode(PEDALINPUTPIN, INPUT_PULLUP);  /*The pedal input pin attached to the magnetic switch is pulled up.*/  
  attachInterrupt(digitalPinToInterrupt(PEDALINPUTPIN),isr,FALLING);  /*Calls the ISR everytime a falling edge on the pedal signal is detected*/ 

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
    if (digitalRead(12) == HIGH) //Touchpad1, jump button.
    {
      bleGamepad.press(BUTTON_1);
      //Serial.println("Touchpad1 pressed");
    }
    else
    {
      bleGamepad.release(BUTTON_1); 
    }
    if (digitalRead(13) == HIGH) //Touchpad2, steer left.
    {
      bleGamepad.press(BUTTON_2);
      //Serial.println("Touchpad2 pressed");
    }
    else
    {
      bleGamepad.release(BUTTON_2);
    }
    if (digitalRead(27) == HIGH) //Touchpad3, Rocket boost
    {
      bleGamepad.press(BUTTON_5);
      //Serial.println("Touchpad3 pressed");
    }
    else
    {
      bleGamepad.release(BUTTON_5);
    }
    if (digitalRead(14) == HIGH) //Touchpad4, Steer right
    { 
      bleGamepad.press(BUTTON_4);
      //Serial.println("Touchpad4 pressed"); 
    }
    else
    {
      bleGamepad.release(BUTTON_4); 
    }
    
    

    
    triggerVal = map(frequency,0,10,0,65535); /*maps pedaling pulses to gamepad trigger value. 
                                              10 pulses (change as needed) corresponds to a to full press of the trigger.*/
    
    if (triggerVal > 65535){   /*Limit the trigger value if more than 10 pulses received.*/
      triggerVal = 65535;
    }  

    //use the below lines to figure out the right trigger value from the frequency pulse count.
    /*Serial.println(frequency);
    Serial.print(" , ");
    Serial.println(triggerVal);*/
    
    bleGamepad.setRightTrigger(triggerVal); //Set the right trigger value.
    

    delay(50);
    loopcount++;
    if (loopcount == 20){ /*Reset the pedal count every 1 second. This ensures that the trigger value does not immediately change.*/
     frequency = count;
     count = 0;
     loopcount = 0;
    }
   }
} 
