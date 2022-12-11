
/*
   "Azeron clone" Game Pad controller
   A custom made keyboard/joystick controller using HID on Arduino Pro Micro
   https://www.instructables.com/id/Azeron-Game-Pad-DIY-Under-35/
   JerkWagon & Anon Engineering @Discord 9/2020
   Rev. 091620vs

   Forked by Mano82 20/01/22
   * added 8-way code for joystick
   * tweaked some pinout
   * cleared serial print and debug code
*/


#include <Keyboard.h>
#define KEY_BEGIN() Keyboard.begin()
#define KEY_PRESS(x) Keyboard.press(x)
#define KEY_RELEASE(x) Keyboard.release(x)
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)


//Globals
const int xCenter = 241;    // Tweak for your D-pad's center values, should be 250
const int yCenter = 257;
const int xDeadband = 100;  // Sets the size of the center deadband
const int yDeadband = 100;

typedef struct {
  int pin;
  char cmd;
  bool wasPressed;
} dKey;

/////////////////////////////////////////////////////////////////////////////

// Switch pins, {PIN, KEY, False}
// generic map
dKey dSwitch[] = {
  {3, '4', false}, // near little finger
  {2, KEY_ESC, false}, // mid little finger
  {4, 'z', false}, // far little finger
  {5, '3', false}, // near ring finger
  {6, 'x', false}, // mid ring finger
  {7, 'c', false}, // far ring finger
  {8, '2', false}, // near middle finger
  {9, 'q', false}, // mid middle finger
  {10, KEY_LEFT_CTRL, false}, // far middle finger
  {15, '1', false}, // near forefinger
  {14, 'e', false}, // mid forefinger
  {16, KEY_LEFT_SHIFT, false}, //far forefinger
  {21, ' ', false}, // Pin 19 = A1 // Joystick btn
};
// D-pad: UP = w, RIGHT = d, DOWN = s, LEFT = a, DPadNone is "center value"
const char dPadUp = 'w';
const char dPadRt = 'd';
const char dPadDn = 's';
const char dPadLt = 'a';
const char dPadNone = '0';


/////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600);
  KEY_BEGIN();
  // Pin Mode setup
  for (int i = 0; i <= 12; i++) {
    pinMode(dSwitch[i].pin, INPUT_PULLUP);
  }
  }

void loop() {
  readJoystick();
  readKeys();
}

void readJoystick() {
  float angle = 0.0;
  static char lastKeyPressed = dPadNone;
  int joyX = analogRead(20);  // Or A3, rotates 0 angle (0 degrees is full right by default)
  int joyY = analogRead(19);  // Or A2, rotates 0 angle
  double mapJoyX = map(joyX, 0, 1023, 0, 500);
  double mapJoyY = map(joyY, 0, 1023, 0, 500);
  
  // Determine if joystick is centered...
 
  if ((mapJoyX <= xCenter + xDeadband && mapJoyX >= xCenter - xDeadband) && (mapJoyY <= yCenter + yDeadband && mapJoyY >= yCenter - yDeadband))  {
    
    angle = 1000.0;
    if (lastKeyPressed != dPadNone)  {
      KEY_RELEASE(lastKeyPressed);
      Keyboard.releaseAll();
      
      lastKeyPressed = dPadNone;
    }
  }
  else  { // Else determine its angle
    angle = atan2(mapJoyY - yCenter , mapJoyX - xCenter) * 57.2957795;
  }
  
  if ((angle >= 67 && angle <= 112)) 
  {  // UP
    if (lastKeyPressed != dPadUp)  
    {
      if (lastKeyPressed != dPadNone)  
      {
        Keyboard.releaseAll();
      }
      KEY_PRESS(dPadUp);
      lastKeyPressed = dPadUp;
    }
  }
  if (angle<67.5 && angle > 22.5)
  {
    //UP LEFT
    if (lastKeyPressed == dPadUp and lastKeyPressed != dPadLt)
    {
      KEY_PRESS(dPadUp);
      KEY_PRESS(dPadLt);
      lastKeyPressed = dPadLt;
    }
    else if (lastKeyPressed == dPadLt and lastKeyPressed != dPadUp)
    {
      KEY_PRESS(dPadLt);
      KEY_PRESS(dPadUp);
      lastKeyPressed = dPadUp;
    }
    else
    {
      Keyboard.releaseAll();
    }
  }
  if (angle>112.5 && angle <135)
  {
    //UP RIGHT
    if (lastKeyPressed == dPadUp and lastKeyPressed != dPadRt)
    {
      
      KEY_PRESS(dPadUp);
      KEY_PRESS(dPadRt);
      lastKeyPressed = dPadRt;
    }
    else if (lastKeyPressed == dPadRt and lastKeyPressed != dPadUp)
    {
      
      KEY_PRESS(dPadRt);
      KEY_PRESS(dPadUp);
      lastKeyPressed = dPadUp;
    }
    else
    {
      Keyboard.releaseAll();
    }
  }
  if (angle<22.5 && angle >-22.5)
  {
    // LEFT
    if (lastKeyPressed != dPadLt)
    {
      if (lastKeyPressed!=dPadNone)
      {
        Keyboard.releaseAll();
      }
      KEY_PRESS(dPadLt);
      lastKeyPressed = dPadLt;
    }
  }
  if (angle<-22.5 && angle >-112.5)
  {
    //DOWN LEFT
    if (lastKeyPressed == dPadDn and lastKeyPressed != dPadLt)
    {
      
      KEY_PRESS(dPadDn);
      KEY_PRESS(dPadLt);
      lastKeyPressed = dPadLt;
    }
    else if (lastKeyPressed == dPadLt and lastKeyPressed != dPadDn)
    {
      
      KEY_PRESS(dPadLt);
      KEY_PRESS(dPadDn);
      lastKeyPressed = dPadDn;
    }
    else
    {
      Keyboard.releaseAll();
    }
  }
  if (angle<-112.5 && angle >-157.5)
  {
    //DOWN RIGHT
    if (lastKeyPressed == dPadDn and lastKeyPressed != dPadRt)
    {
      
      KEY_PRESS(dPadDn);
      KEY_PRESS(dPadRt);
      lastKeyPressed = dPadRt;
    }
    else if (lastKeyPressed == dPadRt and lastKeyPressed != dPadDn)
    {
      
      KEY_PRESS(dPadRt);
      KEY_PRESS(dPadDn);
      lastKeyPressed = dPadDn;
    }
    else
    {
      Keyboard.releaseAll();
    }
  }
  if (angle<-67.5 && angle >-112.5)
  {
    // DOWN
    if (lastKeyPressed != dPadDn)
    {
      if (lastKeyPressed!=dPadNone)
      {
        Keyboard.releaseAll();
      }
      KEY_PRESS(dPadDn);
      lastKeyPressed = dPadDn;
    }
  }
  if((angle < -157.5 && angle >= -180) || (angle <= 180 && angle > 157.5))  
  { // RIGHT
    if (lastKeyPressed != dPadRt)  
    {
      if (lastKeyPressed != dPadNone)  
      {
        Keyboard.releaseAll();
      }
      KEY_PRESS(dPadRt);
      lastKeyPressed = dPadRt;
     
    }
  }
  
}

void readKeys() {
  for (int i = 0; i <= 12; i++)  {
    if (digitalRead(dSwitch[i].pin) == LOW) {
      if (dSwitch[i].wasPressed == false)  {
        KEY_PRESS(dSwitch[i].cmd);
        
        dSwitch[i].wasPressed = true;
      }
    }
    else  {
      if (dSwitch[i].wasPressed == true)  {
        KEY_RELEASE(dSwitch[i].cmd);
        
        dSwitch[i].wasPressed = false;
      }
    }
  }
}
