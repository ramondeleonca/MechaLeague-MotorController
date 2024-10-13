#include <Arduino.h>
#include <AFMotor.h>
#include <Servo.h>
#include <LinkedList.h>
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
#define COMMANDS_DEBUG
LinkedList<String> args;
char separator = ' ';
char delimeter = '\n';
SoftwareSerial controlSerial(A0, A1); // RX, TX

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

void setup() {
  // Begin serial communication
  Serial.begin(9600);
  
  // Begin control serial communication
  controlSerial.begin(9600);

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

void processInput(String input) {
    if (input.length() == 0) {
      return;
    }

    args.clear();

    int index = 0;
    int nextIndex = 0;
    while (nextIndex != -1) {
      nextIndex = input.indexOf(separator, index);
      args.add(input.substring(index, nextIndex));
      index = nextIndex + 1;
    }

    String command = args.get(0);
    args.shift();
    
    #ifdef COMMANDS_DEBUG
    Serial.println("Command: " + command);
    Serial.print("Arguments: ");
    for (int i = 0; i < args.size(); i++) {
      Serial.print(args.get(i) + " ");
    }
    Serial.println();
    #endif

    if (command == "m") {
        int motor = args.get(0).toInt();
        int speed = args.get(1).toInt();
        
        switch(motor) {
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
    } else if (command == "mm") {
        int speed1 = args.get(0).toInt();
        int speed2 = args.get(1).toInt();
        int speed3 = args.get(2).toInt();
        int speed4 = args.get(3).toInt();
        
        controlDCMotorSigned(&m1, speed1);
        controlDCMotorSigned(&m2, speed2);
        controlDCMotorSigned(&m3, speed3);
        controlDCMotorSigned(&m4, speed4);
    } else if (command == "ms") {
        controlDCMotorSigned(&m1, 0);
        controlDCMotorSigned(&m2, 0);
        controlDCMotorSigned(&m3, 0);
        controlDCMotorSigned(&m4, 0);
    } else if (command == "s") {
        int servo = args.get(0).toInt();
        int angle = args.get(1).toInt();
        
        switch(servo) {
            case 1:
                servo1.write(angle);
                break;
            case 2:
                servo2.write(angle);
                break;
        }
    }
}

void loop() {
  if (controlSerial.available()) {
    String input = controlSerial.readStringUntil('\n');
    input.trim();
    processInput(input);
  }

  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    processInput(input);
  }

  delay(5);
}