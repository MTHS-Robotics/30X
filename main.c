#pragma config(Motor,  port2,           rearLeft,      tmotorVex393, openLoop)
#pragma config(Motor,  port3,           frontLeft,     tmotorVex393, openLoop)
#pragma config(Motor,  port4,           frontRight,    tmotorVex393, openLoop)
#pragma config(Motor,  port5,           rearRight,     tmotorVex393, openLoop)
#pragma config(Motor,  port6,           topRight,      tmotorVex393, openLoop)
#pragma config(Motor,  port7,           bottomRight,   tmotorVex393, openLoop)
#pragma config(Motor,  port8,           topLeft,       tmotorVex393, openLoop)
#pragma config(Motor,  port9,           bottomLeft,    tmotorVex393, openLoop)

#include "includes.h";

typedef enum {
	trig = 1,
	norm = 2
} type;

const type mode = trig;

void drive(type mode){
	if(mode == trig){
		PolarJoystick joystick;
		getPolar(&joystick.radians, &joystick.speed);
		drive(joystick.radians, joystick.speed, vexRT[Ch4]);
		wait1Msec(10); //needs to be tested
	}
	else if(mode == norm){
		motor[rearRight] = vexRT[Ch2] - vexRT[Ch4] - vexRT[Ch1];
		motor[frontRight] = vexRT[Ch2] - vexRT[Ch4] + vexRT[Ch1];
		motor[frontLeft] = -vexRT[Ch2] + vexRT[Ch4] - vexRT[Ch1];
		motor[rearLeft] = -vexRT[Ch2] + vexRT[Ch4] + vexRT[Ch1];
	}
}

void lift(){
	if(vexRT[Btn5U] || vexRT[Btn6U]){
		motor[topRight] = 127;
		motor[bottomRight] = 127;

		motor[topLeft] = -127;
		motor[bottomLeft] = -127;
	}
	else if(vexRT[Btn5D] || vexRT[Btn6D]){
		motor[topRight] = -127;
		motor[bottomRight] = -127;

		motor[topLeft] = 127;
		motor[bottomLeft] = 127;
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
	}
}
