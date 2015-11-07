# Attiny84_Serial

* To get Softserial working
<<<<<<< HEAD
 * Modify Softwareserial.cpp in ~/arduino-1.6.5-r5/hardware/arduino/avr/libraries/SoftwareSerial
 * Change "PCINT0_vect" to "PCINT1_vect" and comment out the rest. 
=======
 * Use SoftwareSerial_Tiny.
>>>>>>> e2fd60f3562f0c3d90a037b3028f2f3532edc5fd
 * Max Baud Rate is 19200.
 
* Enable Interrupt for encoder reading
 * PCMSK0 = 0b00000011; //turn on interrupt for PCINT0, PCINT1
 * PCMSK1 = 0b00000011; //turn on interrupt for PCINT8, PCINT9

