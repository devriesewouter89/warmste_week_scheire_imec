/*************************************************************
  Motor Shield 1-Channel DC Motor Demo
  by Randy Sarafan

  For more information see:
  https://www.instructables.com/id/Arduino-Motor-Shield-Tutorial/

*************************************************************/
#include <AFMotor.h>

AF_DCMotor motor_lin(1);
AF_DCMotor motor_claw_0(3);
AF_DCMotor motor_claw_1(4);

int cornerDelay;

#define in0 14
#define in1 15
#define in2 16
#define in3 17 // pin3 is used for pwm
#define LED 2

bool moving;

void setup() {


  pinMode(in0, INPUT_PULLUP);
  pinMode(in1, INPUT_PULLUP);
  pinMode(in2, INPUT_PULLUP);
  pinMode(in3, INPUT_PULLUP);
  pinMode(LED, OUTPUT);

  Serial.begin(115200);
  moving = false;
}
int all, previous = 1;
void loop() {


  int a, b, c, d = HIGH;
  a = digitalRead(in0);
  b = digitalRead(in1);
  c = digitalRead(in2);
  d = digitalRead(in3);
 
  if (a == LOW) {
    armExtend(true);
  }
  if (b == LOW) {
    armExtend(false);
  }

  if (c == LOW) {
    rotate(true);
  }
  if (d == LOW) {
    rotate(false);
  }
  //  previous = all;
  //
  all = a && b && c && d;

  if (all != previous) {
    Serial.println("changes");
    stopArm();
    stopRot();
  }

  //  test();
}

void test() {
  armExtend(true);
  delay(3000);
  stopArm();
  armExtend(false);
  delay(3000);
  stopArm();
  rotate(false);
  delay(3000);
  stopRot();
  rotate(true);
  delay(3000);
  stopRot();
}
void armExtend(bool direction) {
  Serial.print("moving direction=");
  Serial.println(direction);
  if (!moving ) {
    if (direction) {
      motor_lin.run(FORWARD);
    }
    else   {
      motor_lin.run(BACKWARD);
    }
    moving = true;
  }
  motor_lin.setSpeed(255);
  delay(500);
}

void rotate(int direction) {
  Serial.print("rotate direction=");
  Serial.println(direction);
  if (!moving ) {
    if (direction) {
      motor_claw_0.run(FORWARD);
      motor_claw_1.run(FORWARD);
    }
    else   {
      motor_claw_0.run(BACKWARD);
      motor_claw_1.run(BACKWARD);
    }
    moving = true;

  }
  motor_claw_0.setSpeed(255);
  motor_claw_1.setSpeed(255);
  delay(500);
}
void stopArm() {
  //  Serial.println("brake on");
  moving = false;
  motor_lin.run(RELEASE);
}
void stopRot() {
  //  Serial.println("brake on");
  moving = false;
  motor_claw_0.run(RELEASE);
  motor_claw_1.run(RELEASE);

}
/*
  1. calculate distance needed to traverse
  2.


*/
int cornerToDelayCalc(int _degrees)
{
  int rpm = 130;
  long minute = 60000; //ms
  float degpms = 0.78;//360*rpm/minute; // degrees per milliseconds

  int _delay = int(_degrees / degpms);
  return _delay;
}