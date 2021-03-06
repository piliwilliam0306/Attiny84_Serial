
#include "avr/interrupt.h"; 
#include<avr/io.h>
#include "SoftwareSerial_Tiny.h";

#define motorIn1 8
#define InA 2
#define InB 3

#define LOOPTIME        50

const int Rx = 10; // this is physical pin 2
const int Tx = 9; // this is physical pin 3

SoftwareSerial_Tiny mySerial(Rx, Tx); 

volatile long Encoderpos = 0;
volatile int lastEncoded = 0;
unsigned long lastMilli = 0;                    // loop timing 
long dT = 0;

double omega_target = 0;
double omega_actual = 0;

int PWM_val = 0;                                // (25% = 64; 50% = 127; 75% = 191; 100% = 255)
int CPR = 52;                                   // encoder count per revolution
int gear_ratio = 65.5; 

float Kp = 1;
float Ki = 0.01;
//float Ki = 0;
float Kd = 1;
//float Kd = 10;

// the setup function runs once when you press reset or power the board
void setup() 
{

  pinMode(motorIn1,OUTPUT);   pinMode(InA,OUTPUT);  pinMode(InB,OUTPUT);  pinMode(7,OUTPUT);
  
  pinMode(Rx, INPUT); pinMode(Tx, OUTPUT);
  mySerial.begin(19200);

  DDRA &= ~(1 << PA0); //physical pin 13 as input
  DDRA &= ~(1 << PA1); //physical pin 12 as input
  PORTA |= (1 << PA0) | (1 << PA1); //enable pullup

  GIMSK  = 0b00110000; //Pin Change Interrupt Enable 0
  PCMSK0 = 0b00000011; //turn on interrupt for PCINT0, PCINT1
  PCMSK1 = 0b00000011; //turn on interrupt for PCINT8, PCINT9
  sei();
  
}

void loop() 
{ 
  /*if((millis()-lastMilli) >= LOOPTIME)   
     {                                    // enter tmed loop
        dT = millis()-lastMilli;
        lastMilli = millis();
        getMotorData();                                                           // calculate speed
        //read target from Mega
        //if (mySerial.available()>0)
          //{ omega_target= (mySerial.read() * 0.00698039215); }
        PWM_val= (updatePid(omega_target, omega_actual));                       // compute PWM value from rad/s 

        if (PWM_val <= 0)   { analogWrite(motorIn1,PWM_val);  analogWrite(motorIn2,0);}
        if (PWM_val > 0)    { analogWrite(motorIn1,0);  analogWrite(motorIn2,PWM_val);}
        
        //send actual to Mega
        mySerial.write(int(omega_actual/0.00698039215));
     }
     */
  mySerial.write(Encoderpos);     
  PWM_val = 64;
    if (PWM_val <= 0)   { analogWrite(motorIn1,abs(PWM_val));  digitalWrite(InA, LOW);  digitalWrite(InB, HIGH); }
    if (PWM_val > 0)    { analogWrite(motorIn1,abs(PWM_val));  digitalWrite(InA, HIGH);   digitalWrite(InB, LOW);} 
}

void getMotorData()  
{                               
  static long EncoderposPre = 0;       
  //converting ticks/s to rad/s
  omega_actual  = ((Encoderpos - EncoderposPre)*(1000/dT))*2*PI/(CPR*gear_ratio);  //ticks/s to rad/s
  EncoderposPre = Encoderpos;                 
}
 
double updatePid(double targetValue,double currentValue)   
{            
  double pidTerm = 0;                                                            // PID correction
  double error, sum_error, d_error=0;  
  static double last_error=0;                            
  error = targetValue - currentValue; 
  sum_error = sum_error + error * dT;
  d_error = (error - last_error) / dT;
  pidTerm = Kp * error + Ki * sum_error + Kd * d_error;   
  //pidTerm = Kp * error + Kd * d_error;                         
  last_error = error;  
  return constrain(int(pidTerm/0.04039215686), -255, 255);
}

ISR(PCINT0_vect)
{
  int sum  = (lastEncoded << 8) | PINA; //adding to the previous encoded value
  //PA1 = 0; PA0 = 0; PINA = 0X34;
  //PA1 = 0; PA0 = 1; PINA = 0X35;
  //PA1 = 1; PA0 = 0; PINA = 0X36;
  //PA1 = 1; PA0 = 1; PINA = 0X37;
  if(sum == 0x3735 || sum == 0x3534 || sum == 0x3436 || sum == 0x3637)
      Encoderpos++;
  if(sum == 0x3736 || sum == 0x3537 || sum == 0x3435 || sum == 0x3634)
      Encoderpos--;

  lastEncoded = PINA;
}

