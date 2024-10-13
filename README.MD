# MechaLeague Motor Controller
The firmware for the MechaLeague Motor Controller.

The hardware needed is an Arduino UNO with an L293D motor controller shield, 
the code responds to several commands sent over the serial pins A0 (RX) and A1 (TX)
by an external microcontroller, the microcontroller of choice is either an ESP32 or OrangePi/Raspberry Pi

## Commands
`m <motorIndex (1 to 4)> <speed (-255 to 255)>` Single motor control

`mm <speed1 (-255 to 255)> <speed2 (-255 to 255)> <speed3 (-255 to 255)> <speed4 (-255 to 255)>` Multiple motor control

`ms` Stop all motors (SERVOS DO NOT STOP UNTIL REACHING ANGLE)

`s <servoIndex> <angleDegrees>` Single servo control