/*
 * File:          refuerzo_vs.c
 * Date:
 * Description:
 * Author:
 * Modifications:
 */

 /*
  * You may need to add include files like <webots/distance_sensor.h> or
  * <webots/motor.h>, etc.
  */
#include <stdio.h>
#include <webots/robot.h>
#include <webots/motor.h>
#include <webots/position_sensor.h>
#include <webots/distance_sensor.h>
  /*
   * You may want to add macros here.
   */
#define TIME_STEP 64
#define SENSOR_SAMPLING_PERIOD 1
#define FAST_SPEED 5.0
#define SLOW_SPEED -1.0
#define GAMMA 0.5

void enable_sensors(WbDeviceTag infrared_sensors[]) {
	wb_distance_sensor_enable(infrared_sensors[0], SENSOR_SAMPLING_PERIOD);
	wb_distance_sensor_enable(infrared_sensors[1], SENSOR_SAMPLING_PERIOD);
	wb_distance_sensor_enable(infrared_sensors[2], SENSOR_SAMPLING_PERIOD);
	wb_distance_sensor_enable(infrared_sensors[3], SENSOR_SAMPLING_PERIOD);
}
/*
 * This is the main program.
 * The arguments of the main function can be specified by the
 * "controllerArgs" field of the Robot node
 */
int get_state(double sensor_values[]) {
	if ((sensor_values[1] > 750) && (sensor_values[3] < 500))
	{
		return 0;
	}
	else if ((sensor_values[2] > 750) && (sensor_values[0] < 500))
	{
		return 1;
	}
	else
		return 2;
};

void do_action(WbDeviceTag left_motor, WbDeviceTag right_motor, int action) {
	switch (action) {
	case 0:
		wb_motor_set_velocity(left_motor, FAST_SPEED);
		wb_motor_set_velocity(right_motor, SLOW_SPEED);
		break;
	case 1:
		wb_motor_set_velocity(left_motor, SLOW_SPEED);
		wb_motor_set_velocity(right_motor, FAST_SPEED);
		break;
	case 2:
		wb_motor_set_velocity(left_motor, FAST_SPEED);
		wb_motor_set_velocity(right_motor, FAST_SPEED);
		break;
	}
}
void print_matrixes(double q[3][3], double a[3][3]) {
	int row, columns;
	for (row = 0; row <3; row++)
	{
		for (columns = 0; columns < 3; columns++)
		{
			printf("%lf	", q[row][columns]);
		}
		printf("\n");
	}
	printf("\n");
	printf("\n");
	for (row = 0; row < 3; row++)
	{
		for (columns = 0; columns < 3; columns++)
		{
			printf("%lf	", a[row][columns]);
		} 
		printf("\n");
	}
	printf("------------------------------------------\n");
}
void update_q(double q[3][3], double a[3][3], int* prev_state, int* prev_action, double sensor_values[], int random) {
	int next_state = get_state(sensor_values);
	int next_action = 0;
	next_action = random ? rand() % 3 : select_action(q, *prev_state);
	double r = 0.0;
	double aux = 1 / (1 + a[*prev_state][*prev_action]);
	for (int i = 0; i < 4; i++)
	{
		r += (sensor_values[i] < 500.0 ? 1.0 : 0.0);
	}
	q[*prev_state][*prev_action] = (1 - aux) * q[*prev_state][*prev_action] + aux * (r + GAMMA * q[next_state][next_action]);
	a[*prev_state][*prev_action] += 1;
	*prev_state = next_state;
	*prev_action = next_action;
	print_matrixes(q, a);
}
int select_action(double q[3][3], int state) {
	int best_action = 0;
	int aux = 0;
	for (int i = 0; i < 3; i++)
	{
		if (q[state][i] > best_action)
		{
			best_action = q[state][i];
			aux = i;
		}
	}
	return aux;
}
int main(int argc, char** argv) {
	srand(time(NULL));
	/* necessary to initialize webots stuff */
	int state = 0;
	int action = 0;
	int random = 0;
	int random_limit = 50;
	int switch_random=1;
	double sensor_values[4] = { 0,0,0,0 };
	double q[3][3] = {
		{0, 0, 0},
		{0, 0, 0},
		{0, 0, 0}
	};

	double a[3][3] = {
	  {0, 0, 0},
	  {0, 0, 0},
	  {0, 0, 0}
	};
	wb_robot_init();
	WbDeviceTag left_motor = wb_robot_get_device("left wheel motor");
	WbDeviceTag right_motor = wb_robot_get_device("right wheel motor");
	wb_motor_set_position(left_motor, INFINITY);
	wb_motor_set_position(right_motor, INFINITY);
	WbDeviceTag infrared_sensors[4] = { wb_robot_get_device("ground left infrared sensor") , wb_robot_get_device("ground front left infrared sensor") ,wb_robot_get_device("ground front right infrared sensor") ,wb_robot_get_device("ground right infrared sensor") };
	enable_sensors(&infrared_sensors);

	/*
	 * You should declare here WbDeviceTag variables for storing
	 * robot devices like this:
	 *  WbDeviceTag my_sensor = wb_robot_get_device("my_sensor");
	 *  WbDeviceTag my_actuator = wb_robot_get_device("my_actuator");
	 */

	 /* main loop
	  * Perform simulation steps of TIME_STEP milliseconds
	  * and leave the loop when the simulation is over
	  */
	while (wb_robot_step(TIME_STEP) != -1) {

			random = rand() % 50;
			random = random < random_limit;
			random_limit--;
			printf(" % d \n", random_limit);


		for (int i = 0; i < 4; i++)
		{
			sensor_values[i] = wb_distance_sensor_get_value(infrared_sensors[i]);
		}
		update_q(q, a, &state, &action, &sensor_values, random);
		/*
	 * Read the sensors :
	 * Enter here functions to read sensor data, like:
	 *  double val = wb_distance_sensor_get_value(my_sensor);
	 */

	 /* Process sensor data here */

	 /*
	  * Enter here functions to send actuator commands, like:
	  * wb_motor_set_position(my_actuator, 10.0);
	  */
		do_action(left_motor, right_motor, action);


		/* Enter your cleanup code here */

		/* This is necessary to cleanup webots resources */
	};
	wb_robot_cleanup();

	return 0;
}
