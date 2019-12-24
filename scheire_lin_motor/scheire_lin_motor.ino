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

#define in0 A0
#define in1 A1
#define in2 A2
#define in3 A3 // pin3 is used for pwm
#define LED A4

bool moving;
long laserTimeout = 5000;
long lastLaserTimer;



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
    laser_on();
  }
  if (b == LOW) {
    armExtend(false);
    laser_on();
  }

  if (c == LOW) {
    rotate(true);
    laser_on();

  }
  if (d == LOW) {
    rotate(false);
    laser_on();

  }
  //  previous = all;
  //
  all = a && b && c && d;

  if (all != previous) {
    Serial.println("changes");
    lastLaserTimer = millis();
    stopArm();
    stopRot();
    laser_on();
  }

  if (millis() - laserTimeout > lastLaserTimer) {
    laser_off();
  }

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

void laser_on() {
  digitalWrite(LED, HIGH);
}
void laser_off() {
  digitalWrite(LED, LOW);
}

void rotate(int direction) {
  Serial.print("rotate direction=");
  Serial.println(direction);
  if (!moving ) {
    if (direction) {
      motor_claw_0.run(FORWARD);
      motor_claw_1.run(BACKWARD);
    }
    else   {
      motor_claw_0.run(BACKWARD);
      motor_claw_1.run(FORWARD);
    }
    moving = true;

  }
  motor_claw_0.setSpeed(100);
  motor_claw_1.setSpeed(100);
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
