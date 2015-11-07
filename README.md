# Attiny84_Serial

* To get Softserial working

 * Use SoftwareSerial_Tiny.
 * Max Baud Rate is 19200.
 
* Enable Interrupt for encoder reading
 * PCMSK0 = 0b00000011; //turn on interrupt for PCINT0, PCINT1
 * PCMSK1 = 0b00000011; //turn on interrupt for PCINT8, PCINT9

