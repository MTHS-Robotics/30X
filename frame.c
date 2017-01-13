#pragma config(Sensor, dgtl1,  intake,         sensorDigitalOut)
#pragma config(Motor,  port2,           rearLeft,      tmotorVex393, openLoop)
#pragma config(Motor,  port3,           frontLeft,     tmotorVex393, openLoop)
#pragma config(Motor,  port4,           frontRight,    tmotorVex393, openLoop)
#pragma config(Motor,  port5,           rearRight,     tmotorVex393, openLoop)
#pragma config(Motor,  port6,           topRight,      tmotorVex393, openLoop)
#pragma config(Motor,  port7,           bottomRight,   tmotorVex393, openLoop)
#pragma config(Motor,  port8,           topLeft,       tmotorVex393, openLoop)
#pragma config(Motor,  port9,           bottomLeft,    tmotorVex393, openLoop)

#include "includes.h";

/* User-defined "modes" of operation for debugging purposes */
typedef enum {
	trig = 1,
	norm = 2
} type;

/* Define mode */
const type mode = trig;

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

task main()
{
	for(;;){
		drive(mode);
		lift();
		in();
	}
}
