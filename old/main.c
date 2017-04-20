#pragma config(Sensor, dgtl1,  intake,         sensorDigitalOut)
#pragma config(Motor,  port2,           rearLeft,      tmotorVex393, openLoop)
#pragma config(Motor,  port3,           frontLeft,     tmotorVex393, openLoop)
#pragma config(Motor,  port4,           frontRight,    tmotorVex393, openLoop)
#pragma config(Motor,  port5,           rearRight,     tmotorVex393, openLoop)
#pragma config(Motor,  port6,           topRight,      tmotorVex393, openLoop)
#pragma config(Motor,  port7,           bottomRight,   tmotorVex393, openLoop)
#pragma config(Motor,  port8,           topLeft,       tmotorVex393, openLoop)
#pragma config(Motor,  port9,           bottomLeft,    tmotorVex393, openLoop)
#pragma config(Motor,  port10,          roll,      tmotorVex393, openLoop)

#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

void pre_auton()
{
  bStopTasksBetweenModes = true;
	// All activities that occur before the competition starts
	// Example: clearing encoders, setting servo positions, ...
}
task autonomous()
{
	motor[frontLeft] = -127;
	motor[frontRight] = 127;
	motor[rearLeft] = -127;
	motor[rearRight] = 127;
	motor[roll] = 127;

	wait1Msec(3000);

	motor[frontLeft] = 0;
	motor[frontRight] = 0;
	motor[rearLeft] = 0;
	motor[rearRight] = 0;
	motor[roll] = 127;

	wait1Msec(10000);

	StopTask(autonomous);
}

typedef enum {
	trig = 1,
	norm = 2
} type;

/* Code for driving
 *
 * For trig mode, use the x and y positions of each joystick
 * For normal mode, use standard methods (adding and subtracting joystick channels)
 */
void drive(type mode){

	if(mode == trig){
		Joystick joystick;
		getPolar(&joystick.rad, &joystick.speed);
		move(joystick.rad, joystick.speed, vexRT[Ch1]);
		wait1Msec(10); //needs to be tested
	}
	else if(mode == norm){
		motor[rearRight] = vexRT[Ch2] - vexRT[Ch4] - vexRT[Ch1];
		motor[frontRight] = vexRT[Ch2] - vexRT[Ch4] + vexRT[Ch1];
		motor[frontLeft] = -vexRT[Ch2] + vexRT[Ch4] - vexRT[Ch1];
		motor[rearLeft] = -vexRT[Ch2] + vexRT[Ch4] + vexRT[Ch1];
	}
}

/* Pneumatics code */
void in(){
	if(vexRT[Btn6UXmtr2]){
		SensorValue[intake] = 1;
	}
	else if(vexRT[Btn6DXmtr2]){
		SensorValue[intake] = 0;
	}
}


/* Code for lift
 * When top trigger buttons are pressed,
 * Set the motors to move the lift up.
 *
 * When bottom trigger buttons are pressed,
 * Set the motors to move the lift down.
 */
void lift(){
	if(vexRT[Btn5UXmtr2]){
		motor[topRight] = -127;
		motor[bottomRight] = -127;

		motor[topLeft] = 127;
		motor[bottomLeft] = 127;
	}
	else if(vexRT[Btn5DXmtr2]){
		motor[topRight] = 127;
		motor[bottomRight] = 127;

		motor[topLeft] = -127;
		motor[bottomLeft] = -127;
	}
	else{
		motor[topRight] = 0;
		motor[bottomRight] = 0;
		motor[topLeft] = 0;
		motor[bottomLeft] = 0;
	}
}
void antiRoll(){
	if(vexRT[Btn7L]){
		motor[roll] = -127;
	}
	else if(vexRT[Btn8R]){
		motor[roll] = 127;
	}
	else{
		motor[roll] = 0;
	}
}
task usercontrol()
{
	for(;;){
		drive(mode);
		lift();
		in();
		antiRoll();
	}
}
	/*while( 1=1)
 forward();
if nMotorEncoder[frontLeft] < 0.9 nMotorEncoder[frontRight])
	{
		//Move forward at partial power
	motor[frontLeft] = 0.9 motor[frontRight];

	}
		if else nMotorEncoder[frontRight] < 0.9 nMotorEncoder[frontLeft])
	{
		//Move forward at partial power
	motor[frontLeft] = 90;
	motor[frontRight] = 90;
	break;
}
*/
