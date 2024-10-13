#include <Arduino.h>
#include <AFMotor.h>
#include <Servo.h>
#include <lib/Commands.h>
#include <SoftwareSerial.h>

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

// Control serial and commands
SoftwareSerial controlSerial(A0, A1); // RX, TX
Commands commands(&controlSerial);

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

// Register commands
void registerCommands() {
  // * m <motor> <speed>
  commands.addCommand("m", [](Stream* serial, LinkedList<String> args) {
    if (args.size() != 2) {
      serial->println("err: Invalid number of arguments");
      return;
    }

    int motor = args.get(0).toInt();

    if (motor < 1 || motor > 4) {
      serial->println("err: Invalid motor number");
      return;
    }

    int speed = args.get(1).toInt();

    if (speed < -255 || speed > 255) {
      serial->println("err: Invalid speed");
      return;
    }

    switch (motor) {
      case 1:
        controlDCMotorSigned(&m1, speed);
        break;
      case 2:
        controlDCMotorSigned(&m2, speed);
        break;
      case 3:
        controlDCMotorSigned(&m3, speed);
        break;
      case 4:
        controlDCMotorSigned(&m4, speed);
        break;
    }
  });

  // * mm <speed> <speed> <speed> <speed>
  commands.addCommand("mm", [](Stream* serial, LinkedList<String> args) {
    if (args.size() != 4) {
      serial->println("err: Invalid number of arguments");
      return;
    }

    int speed1 = args.get(0).toInt();
    int speed2 = args.get(1).toInt();
    int speed3 = args.get(2).toInt();
    int speed4 = args.get(3).toInt();

    if (
      speed1 < -255 || speed1 > 255 ||
      speed2 < -255 || speed2 > 255 ||
      speed3 < -255 || speed3 > 255 ||
      speed4 < -255 || speed4 > 255
    ) {
      serial->println("err: Invalid speed");
      return;
    }

    controlDCMotorSigned(&m1, speed1);
    controlDCMotorSigned(&m2, speed2);
    controlDCMotorSigned(&m3, speed3);
    controlDCMotorSigned(&m4, speed4);
  });

  // * s <servo> <angle>
  commands.addCommand("s", [](Stream* serial, LinkedList<String> args) {
    if (args.size() != 2) {
      serial->println("err: Invalid number of arguments");
      return;
    }

    int servo = args.get(0).toInt();

    if (servo < 1 || servo > 2) {
      serial->println("err: Invalid servo number");
      return;
    }

    int angle = args.get(1).toInt();

    if (angle < 0 || angle > 180) {
      serial->println("err: Invalid angle");
      return;
    }

    switch (servo) {
      case 1:
        servo1.write(angle);
        break;
      case 2:
        servo2.write(angle);
        break;
    }
  });
}

void setup() {
  // Begin serial communication
  Serial.begin(115200);
  
  // Begin control serial communication
  controlSerial.begin(9600);

  // Register commands
  registerCommands();

  // Configure servos
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  
  // Configure motors
  m1.run(RELEASE);
  m2.run(RELEASE);
  m3.run(RELEASE);
  m4.run(RELEASE);
}

extern unsigned int __bss_end;
extern unsigned int __heap_start;
extern void *__brkval;

int freeMemory() {
  int free_memory;
  if ((int)__brkval == 0) {
    free_memory = ((int)&free_memory) - ((int)&__bss_end);
  } else {
    free_memory = ((int)&free_memory) - ((int)__brkval);
  }
  return free_memory;
}

void loop() {
  // if (controlSerial.available()) commands.readSerial();
  // if (controlSerial.available()) commands.readString(controlSerial.readStringUntil('\n'));
  if (controlSerial.available()) {
    String input = controlSerial.readStringUntil('\n');
    Serial.println(input);
    commands.readString(input);
  }

  delay(5);
}