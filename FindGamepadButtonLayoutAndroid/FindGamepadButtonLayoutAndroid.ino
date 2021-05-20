//A program to figure out Gamepad button layout in android games by sending keyboard keystrokes to press gamepad buttons.
//Keyboard commands are sent via serial communication thus the esp32 needs to connected to the laptop through a USB cable.
#include <BleGamepad.h>

BleGamepad bleGamepad;

char ch;

void setup() {
  
  Serial.begin(115200);
  bleGamepad.begin();
}

void loop() {
  
     if(Serial.available()>0)
    {
      ch=Serial.read();
      switch(ch)
    {
      case 'w': bleGamepad.setY(32767);
                Serial.println("Left thumb UP");
                break;
      case 'a': bleGamepad.setX(-32767);
                Serial.println("Left thumb LEFT");
                break;
      case 's': bleGamepad.setY(-32767);
                Serial.println("Left thumb DOWN");
                break;          
      case 'd': bleGamepad.setX(32767);
                Serial.println("Left thumb RIGHT");
                break; 
      case 'v': bleGamepad.setRZ(32767);
                Serial.println("Right thumb RIGHT");
                break;
      case 'x': bleGamepad.setRZ(32767);
                Serial.println("Right thumb LEFT");
                break;
      case 'f': bleGamepad.setZ(32767);
                Serial.println("Right thumb UP");
                break;
      case 'c': bleGamepad.setZ(-32767);
                Serial.println("Right thumb DOWN");
                break; 
      case 'j': bleGamepad.setHats(DPAD_UP,DPAD_UP);
                Serial.println("DPAD UP");
                break;
      case 'n': bleGamepad.setHats(DPAD_DOWN,DPAD_DOWN);
                Serial.println("DPAD DOWN");
                break;  
      case 'm': bleGamepad.setHats(DPAD_RIGHT,DPAD_RIGHT);
                Serial.println("DPAD RIGHT");
                break; 
      case 'b': bleGamepad.setHats(DPAD_LEFT,DPAD_LEFT);
                Serial.println("DPAD LEFT");
                break;                                        
      case '1': bleGamepad.press(BUTTON_1); //A
                Serial.println("Button 1 pressed");
                break;
      case '2': bleGamepad.press(BUTTON_2); //B
                Serial.println("Button 2 pressed");
                break;
      case '4': bleGamepad.press(BUTTON_4); //right bottom //X
                Serial.println("Button 4 pressed");
                break;     
      case '5': bleGamepad.press(BUTTON_5); //right top //Y
                Serial.println("Button 5 pressed");
                break;         
      case '7': bleGamepad.press(BUTTON_7); //L1
                Serial.println("Button 7 pressed");
                break; 
      case '8': bleGamepad.press(BUTTON_8); //R1
                Serial.println("Button 8 pressed");
                break;
      case '9': bleGamepad.press(BUTTON_9); //L2
                Serial.println("Button 9 pressed");
                break;
      case '0': bleGamepad.press(BUTTON_10); //R2
                Serial.println("Button 10 pressed");
                break;          
      case 't':bleGamepad.press(BUTTON_11); //Menu
               Serial.println("Button 11 pressed");
               break;
      case 'y':bleGamepad.press(BUTTON_12); //Pause
               Serial.println("Button 12 pressed");
               break;    
      case 'i':bleGamepad.press(BUTTON_14); //LS Press
               Serial.println("Button 14 pressed");
               break;    
      case 'o':bleGamepad.press(BUTTON_15); //RS Press
               Serial.println("Button 15 pressed");
               break;                                                                                                 
      default: break;                               
    }
    }
    delay(50);
    bleGamepad.setAxes(DPAD_CENTERED);
    bleGamepad.release(BUTTON_1);
    bleGamepad.release(BUTTON_2);
    bleGamepad.release(BUTTON_4);
    bleGamepad.release(BUTTON_5);
    bleGamepad.release(BUTTON_7);
    bleGamepad.release(BUTTON_8);
    bleGamepad.release(BUTTON_9);
    bleGamepad.release(BUTTON_11);
    bleGamepad.release(BUTTON_12);
    bleGamepad.release(BUTTON_14);
    bleGamepad.release(BUTTON_15);
    bleGamepad.setX(0);
    bleGamepad.setY(0);
    bleGamepad.setZ(0);
    bleGamepad.setRZ(0);  
    bleGamepad.setHats(DPAD_CENTERED,DPAD_CENTERED);
}
