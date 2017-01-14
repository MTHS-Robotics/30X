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
	wait1Msec(2000);

	nMotorEncoder[frontLeft] = 0;
	nMotorEncoder[frontRight] = 0;
	
	motor[frontLeft] = 127;
	motor[frontRight] = 127;
	motor[rearLeft] = 127;
	motor[rearRight] = 127;
	
	wait1Msec(10000);
	StopTask(autonomous);
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
typedef enum {
	trig = 1,
	norm = 2
} type;

/* Define mode */
const type mode = trig;
#define MAX_SPEED 127

typedef struct {
	float frontLeft;
	float frontRight;
	float rearRight;
	float rearLeft;
} Outputs;

typedef struct {
	float rad;
	float speed;
} Joystick;

float get_val(Outputs *in, int i){
	switch(i){
		case 0: return in->frontLeft;
		case 1: return in->frontRight;
		case 2: return in->rearRight;
		case 3: return in->rearLeft;
	}
	//an error, resume operation
	return 0;
}

float MaxFloat(Outputs *nums){
	float maxValue = 0;
	float temp;
	int z = 0;
	for(z = 0; z <= 3; z++){
		temp = abs(get_val(nums, z));
		if(temp > maxValue)
			maxValue = temp;
	}
	return maxValue;
}


/* Main code for movement
 *
 * Note that rad and speeds are passed in from drive(),
 * and that the values are taken from the getPolar() function
 */
void move(float rad, float speed = 1, byte rotation = 0)
{
	if(speed > 0)
	{
		Outputs output;
		output.frontLeft = -MAX_SPEED * cos(PI/4 - rad) + rotation;
		output.frontRight =  MAX_SPEED * cos(PI/4 + rad) + rotation;
		output.rearRight =  MAX_SPEED * cos(PI/4 - rad) + rotation;
		output.rearLeft	= -MAX_SPEED * cos(PI/4 + rad) + rotation;

		float maxOutput = MaxFloat(&output);
		speed *= (MAX_SPEED / maxOutput);

		output.frontLeft *= speed;
		output.frontRight *= speed;
		output.rearLeft *= speed;
		output.rearRight *= speed;

		motor[frontLeft] = (byte)output.frontLeft;
		motor[frontRight] = (byte)output.frontRight;
		motor[rearLeft] = (byte)output.rearLeft;
		motor[rearRight] = (byte)output.rearRight;
	}
	//not moving, so rotation is allowed
	//needs to be fixed so movement + rotation is possible
	else if (abs(rotation) > 20)
	{
		motor[frontLeft] = -rotation;
		motor[frontRight] = -rotation;
		motor[rearLeft] = -rotation;
		motor[rearRight] = -rotation;
	}
	else
	{
		motor[frontLeft] = 0;
		motor[frontRight] = 0;
		motor[rearLeft] = 0;
		motor[rearRight] = 0;
	}
}



/* Basic trig...
 * atan of Ch3 and Ch4 gives the angle of the drive joystick
 * sqrt(Ch3^2 + Ch4^2) yields the amount the joystick is being pushed, which
 * is then taken out of 127
 *
 * Notice how we use pointers, so we don't need to return anything
 */
void getPolar(float *rad, float *speed, TVexJoysticks joy_x = Ch3, TVexJoysticks joy_y = Ch4) {
	byte x_val = vexRT[joy_x];
	byte y_val = vexRT[joy_y];
	//tolerances
	if((abs(x_val) < 20) && (abs(y_val) < 20)) {
		*rad = 0;
		*speed = 0;
	}
	else {
		*rad = atan2(y_val,x_val);
		float tmpSpeed = sqrt((y_val * y_val) + (x_val * x_val));
		*speed = tmpSpeed/127;
		if(*speed > 1) *speed = 1;
	}
}
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
	while (true)
	{
		for(;;){
			drive(mode);
			lift();
			in();
			antiRoll();
		}
	}
}
