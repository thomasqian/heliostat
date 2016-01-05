#include <Servo.h>

Servo top, bot;

float p0, p1, p2;
const float tol1 = .02;
const float tol2 = .06;

int angle = 180;

void setup() {
  Serial.begin(9600);
  
  top.attach(6);
  bot.attach(5);
  
  top.write(angle);
  
  //93 to 180
  bot.write(95);
  //94 forward; 96 backward
}

void loop() {
    
  /*** OPTIMIZE BOTTOM SERVO ONLY IF NEEDED ***/
  if (!isBotOptimal(tol1)) {
    do {
      readPhotos();
        
      //reset
      if ((p0 + p1 + p2)/3 < 75) {
        //75 IS A PRE-SET VALUE DEPENDING ON THE LIGHT SOURCE
        angle = 180;
          top.write(angle);
      }
          
      if (p1 > p2) {
        bot.write(93);
      } else {
        bot.write(97);
      }
      
    } while (p2/p1 >= 1 + tol1 || p2/p1 <= 1 - tol1);
         
    bot.write(95);
  }
  /*** AT THIS POINT BOTTOM SERVO SHOULD BE OPTIMAL ***/
    
  /*** OPTIMIZE TOP SERVO ONLY IF NEEDED ***/
  while (!isTopOptimal()) {
    top.write(angle);
      
    if (!isBotOptimal(0.12) || angle < 93 || angle > 180) {
      if (angle < 93) angle = 93;
      else if (angle > 180) angle = 180;
      break;
    }  
      
    delay(50);
    readPhotos();
    if (p0 > (p1 + p2)/2) angle++;
    else angle--;
  }
  /*** AT THIS POINT TOP SERVO SHOULD BE OPTIMAL ***/
  
  //time between checking OPTIMAL STATUS
  //delay(100);
}

boolean isBotOptimal(float tol) {
  readPhotos();
  return (p2/p1 < 1 + tol && p2/p1 > 1 - tol);
}

boolean isTopOptimal() {
  readPhotos();
  float val = 2 * p0 / (p1 + p2);
  return (val < 1 + tol2 && val > 1 - tol2);
}

void readPhotos() {
  p0 = analogRead(A0);
  p1 = analogRead(A1);
  p2 = analogRead(A2);
}
