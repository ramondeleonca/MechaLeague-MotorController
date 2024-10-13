#include <Arduino.h>
#include <AFMotor.h>
#include <Servo.h>

// Initialize servos
const int SERVO1_PIN = 9;
const int SERVO2_PIN = 10;

// Initialize motors
Servo servo1;
Servo servo2;

AF_DCMotor m1(1);
AF_DCMotor m2(2);
AF_DCMotor m3(3);
AF_DCMotor m4(4);

// DC motor control middleware
void controlDCMotorSigned(AF_DCMotor *motor, int speed) {
  if (speed < 0) {
    motor->run(BACKWARD);
  } else if (speed > 0) {
    motor->run(FORWARD);
  } else {
    motor->run(RELEASE);
  }

  motor->setSpeed(abs(speed));
}

void controlDCMotorWithAnalog(int pin, AF_DCMotor *motor) {
    // Read input from analog pin
    int input = analogRead(pin);

    // Input is pulled low, stop motor
    if (input < 10) {
        motor->run(RELEASE);
        return;
    }

    // Map input to motor speed
    int speed = map(input, 10, 255, -255, 255);

    // Set motor speed
    controlDCMotorSigned(motor, speed);
}

void controlServoMotorWithAnalog(int pin, Servo *servo) {
    // Read input from analog pin
    int input = analogRead(pin);

    // Map input to servo angle
    int angle = map(input, 0, 255, 0, 180);

    // Set servo angle
    servo->write(angle);
}

void setup() {
  // Begin serial communication
  Serial.begin(115200);

  // Configure servos
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  
  // Configure motors
  m1.run(RELEASE);
  m2.run(RELEASE);
  m3.run(RELEASE);
  m4.run(RELEASE);
}

void loop() {

  controlDCMotorWithAnalog(A0, &m1);
  controlDCMotorWithAnalog(A1, &m2);
  controlDCMotorWithAnalog(A2, &m3);
  controlDCMotorWithAnalog(A3, &m4);

  controlServoMotorWithAnalog(A4, &servo1);
  controlServoMotorWithAnalog(A5, &servo2);

  delay(10);
}