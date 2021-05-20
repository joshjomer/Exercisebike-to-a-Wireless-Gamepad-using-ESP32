//Exercise bike configured for Rocket League with OLED display added.
/* ESP32 Gamepad library, https://github.com/lemmingDev/ESP32-BLE-Gamepad */
#include <BleGamepad.h>
#include <Wire.h> //For I2C Serial communiction.
#include <Adafruit_GFX.h>  //To display graphics on the OLED display
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

BleGamepad bleGamepad;

/*count variable declared as volatile, as it is being accessed by both interrupt service routine and main program. 
So the compiler is directed to load variable from RAM instead of registers. */
volatile int count = 0;
volatile int frequency;

//variable to keep track of distance
float dist = 0;

//three variables for seconds, minutes and hours pedaled.
int s = 0;
int m = 0;
int h = 0;

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
  attachInterrupt(PEDALINPUTPIN,isr,FALLING);  /*Calls the ISR everytime a falling edge on the pedal signal is detected*/

  pinMode(27, INPUT); /*The touch modules are connected to the following ESP32 GPIO pins*/
  pinMode(14, INPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);
  
  //Serial.begin(115200); //Serial communication used for debuging code.
  bleGamepad.begin();
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS); 
}

void loop(){
  //if(bleGamepad.isConnected()){
  /*commented the above line as we want the display to be updated even without being used as a gamepad.*/ 
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
                                              
    if (triggerVal > 65535){ /*Limit the trigger value if more than 10 pulses received.*/
      triggerVal = 65535;
    }  
    
    //use the below lines to figure out the right trigger value from the frequency pulse count.
    /*Serial.print(frequency);
    Serial.print(" , ");
    Serial.println(triggerVal);*/
    
    bleGamepad.setRightTrigger(triggerVal); //Set the right trigger value.
    
    delay(50);
    loopcount++;
    if (loopcount == 20){
     frequency = count;
     count = 0;
     loopcount = 0;
     dist = dist + 0.001*frequency; /*just made up this formula, 1 pulse is considered as a meter.*/
     display.display(); 
     display.clearDisplay();  /*Below lines are to display distance.*/
     display.setTextSize(1);
     display.setTextColor(WHITE);
     display.setCursor(1,1);
     display.print("Dist ");
     display.setTextSize(2);
     display.print(dist);
     display.setTextSize(1);
     display.println("km");
     display.setTextSize(2);
     display.println(" ");
     display.println(" ");
     if (frequency > 0){  /*Calculate time and display it*/
      s = s + 1;
     }
     if (s == 60){
      m = m + 1;
      s = 0;
     }
     if (m == 60){
      h = h + 1;
      m = 0;
     }
     display.setTextSize(1);
     display.print("Time ");
     display.setTextSize(2);
     display.print(h);
     display.print(":");
     display.print(m);
     display.print(":");
     display.println(s);
    }
 //}
} 
