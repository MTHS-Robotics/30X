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
