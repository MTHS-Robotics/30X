#define MAX_SPEED 127

/* Function to find maximum value in an array of 10 floats */

float FindMaxFloat(float a, float b,  float c = (0), float d = (0), float e = (0),
float f = (0), float g = (0), float h = (0), float i = (0), float j = (0))
{
	float values[] = {a,b,c,d,e,f,g,h,i,j};
	float maxValue = 0;
	for(int z = 0; z < 9; z++) {
		if(abs(values[z]) > maxValue) maxValue = abs(values[z]);
	}
	return maxValue;
}

/* Main code for movement
 *
 * Note that radians and speeds are passed in from drive(),
 * and that the values are taken from the getPolar() function
 */
void move(float radians, float speed = 1, byte rotation = 0)
{
	if(speed > 0)
	{
		float frontLeftOutput 	= -MAX_SPEED * cos(PI/4 - radians);
		float frontRightOutput 	=  MAX_SPEED * cos(PI/4 + radians);
		float rearRightOutput 	=  MAX_SPEED * cos(PI/4 - radians);
		float rearLeftOutput	= -MAX_SPEED * cos(PI/4 + radians);

		frontLeftOutput += rotation;
		frontRightOutput += rotation;
		rearRightOutput += rotation;
		rearLeftOutput += rotation;


		float maxOutput = FindMaxFloat(frontLeftOutput, frontRightOutput, rearRightOutput, rearLeftOutput);
		speed *= (MAX_SPEED / maxOutput);


		frontLeftOutput *= speed;
		frontRightOutput *= speed;
		rearLeftOutput *= speed;
		rearRightOutput *= speed;

		motor[frontLeft] = (byte)frontLeftOutput;
		motor[frontRight] = (byte)frontRightOutput;
		motor[rearLeft] = (byte)rearLeftOutput;
		motor[rearRight] = (byte)rearRightOutput;
	}
	else if (rotation > 20 || rotation < -20)
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

typedef struct
{
	float radians;
	float speed;
} PolarJoystick;


/* Basic trig...
 * atan of Ch3 and Ch4 gives the angle of the joystick
 * sqrt(Ch3^2 + Ch4^2) yields the amount the joystick is being pushed, which
 * is then taken out of 127
 */
void getPolar(float *radians, float *speed, TVexJoysticks joy_x = Ch3, TVexJoysticks joy_y = Ch4) {
	byte x_val = vexRT[joy_x];
	byte y_val = vexRT[joy_y];
	if((abs(x_val) < 20) && (abs(y_val) < 20)) {
		*radians = 0;
		*speed = 0;
	}
	else {
		*radians = atan2(y_val,x_val);
		float tmpSpeed = sqrt((y_val * y_val) + (x_val * x_val));
		*speed = tmpSpeed/127;
		if(*speed > 1) *speed = 1;
	}
}
