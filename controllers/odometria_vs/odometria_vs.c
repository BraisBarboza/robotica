/*
 * File:          odometria_vs.c
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
#include <webots/camera.h>
  /*
   * You may want to add macros here.
   */

#define PI 3.142857
#define TIME_STEP 64
#define MAX_SPEED 6.28
#define WHEEL_RADIX 0.021
#define WHEEL_GAP 0.1054
#define CELL_SIZE 0.25
#define CRUISE_SPEED 0.628
#define SENSOR_SAMPLING_PERIOD 1
#define DELTA 2.58
#define MATRIX_LIMIT 30
#define WALL_THRESHOLD 190.000
#define EPSILON 0.01
#define GOAL_BLUE_THRESHOLD 150000
   /*
   * This is the main program.
   * The arguments of the main function can be specified by the
   * "controllerArgs" field of the Robot node
   */

void enable_sensors(WbDeviceTag camera, WbDeviceTag left_sensor, WbDeviceTag right_sensor, WbDeviceTag infrared_sensors[]) {

	wb_camera_enable(camera, SENSOR_SAMPLING_PERIOD);

	wb_position_sensor_enable(left_sensor, SENSOR_SAMPLING_PERIOD);
	wb_position_sensor_enable(right_sensor, SENSOR_SAMPLING_PERIOD);
	wb_distance_sensor_enable(infrared_sensors[0], SENSOR_SAMPLING_PERIOD);
	
	wb_distance_sensor_enable(infrared_sensors[1], SENSOR_SAMPLING_PERIOD);
	wb_distance_sensor_enable(infrared_sensors[2], SENSOR_SAMPLING_PERIOD);
	wb_distance_sensor_enable(infrared_sensors[3], SENSOR_SAMPLING_PERIOD);
}
void init_map(int map[][MATRIX_LIMIT]) {
	for (int i = 0; i < MATRIX_LIMIT; i++)
	{
		for (int j = 0; j < MATRIX_LIMIT; j++)
		{
			map[i][j] = -1;

		}
	}
}
double measure_distance(WbDeviceTag position_sensor) {
	double sum=0;
	int i;
	for (i = 0; i < 1000; i++)
	{
		sum+=wb_distance_sensor_get_value(position_sensor);
	}
	return sum /(double) i;
}
void advance1(WbDeviceTag left_motor, WbDeviceTag left_sensor, WbDeviceTag right_motor, WbDeviceTag right_sensor) {
	wb_motor_set_position(left_motor, wb_position_sensor_get_value(left_sensor) + CELL_SIZE / WHEEL_RADIX);
	wb_motor_set_position(right_motor, wb_position_sensor_get_value(right_sensor) + CELL_SIZE / WHEEL_RADIX);
}
void turn(WbDeviceTag left_motor, WbDeviceTag left_sensor, WbDeviceTag right_motor, WbDeviceTag right_sensor, int *orientation, int direction) {

	if (!direction)
	{
		wb_motor_set_position(left_motor, -PI / 2 * DELTA + wb_position_sensor_get_value(left_sensor));
		wb_motor_set_position(right_motor, PI / 2 * DELTA + wb_position_sensor_get_value(right_sensor));
		*orientation = *orientation+1;
		*orientation = *orientation % 4;
	}
	else
	{
		wb_motor_set_position(left_motor, PI / 2 * DELTA + wb_position_sensor_get_value(left_sensor));
		wb_motor_set_position(right_motor, -PI / 2 * DELTA + wb_position_sensor_get_value(right_sensor));
		*orientation = *orientation - 1;
		if (*orientation==-1)
		{
			*orientation = 3;
		}
		*orientation = *orientation % 4;
	}
	printf("%d\n", *orientation);
	//printf("%f\n", -PI / 2 * DELTA + before_movement_sensor_state[0]);
}

int stopped(WbDeviceTag left_motor, WbDeviceTag left_sensor, WbDeviceTag right_motor, WbDeviceTag right_sensor) {
	double left_motor_target_position = wb_motor_get_target_position(left_motor);
	double right_motor_target_position = wb_motor_get_target_position(right_motor);
	//return ((int)(left_motor_target_position * 100 + .5) == (int)(wb_position_sensor_get_value(left_sensor) * 100 + .5)) && ((int)(right_motor_target_position * 100 + .5) == (int)(wb_position_sensor_get_value(right_sensor) * 100 + .5));
	return fabs(left_motor_target_position - wb_position_sensor_get_value(left_sensor)) < EPSILON && fabs(right_motor_target_position - wb_position_sensor_get_value(right_sensor)) < EPSILON;
}
void print_matrix(int map[][MATRIX_LIMIT]) {
	int row, columns;
	for (row = 0; row < MATRIX_LIMIT; row++)
	{
		for (columns = 0; columns <MATRIX_LIMIT; columns++)
		{
			printf("%d	", map[row][columns]);
		}
		printf("\n");
	}
	printf("\n");
	printf("\n");
}
int is_goal(WbDeviceTag camera) {
	int image_width, image_height,r=0,g=0,b=0;
	image_width = wb_camera_get_width(camera);
	image_height = wb_camera_get_height(camera);
	const unsigned char* image = wb_camera_get_image(camera);
	for (int x = (image_width/2)-10; x < (image_width/2)+10; x++)
		for (int y = (image_height/2)-10; y < (image_height/2)+10; y++) {
			r += wb_camera_image_get_red(image, image_width, x, y);
			g += wb_camera_image_get_green(image, image_width, x, y);
			b += wb_camera_image_get_blue(image, image_width, x, y);
			
		}
	return b<GOAL_BLUE_THRESHOLD;
}
void update_map(WbDeviceTag infrared_sensors[], int orientation, int map[][MATRIX_LIMIT], int location[], int blocked[], int goal_location[]) {
	map[location[0]][location[1]] = 2;
	int map_value;
	printf("%d\n", orientation);
	switch (orientation)
		
	{
	case 0:
		for (int i = 0; i < 4; i++)
		{
			map_value = measure_distance(infrared_sensors[i]) > WALL_THRESHOLD;
			blocked[i] = map_value;
				switch (i)
				{
				case 0:
					map[location[0] - 1][location[1]] = map_value;
					break;
				case 1:
					map[location[0]][location[1] - 1] = map_value;

					break;

				case 2:
					map[location[0] + 1][location[1]] = map_value;

					break;
				case 3:
					map[location[0]][location[1] + 1] = map_value;

					break;
				default:
					break;
				}
			
		}
		break;
	case 1:
		for (int i = 0; i < 4; i++)
		{
			map_value = measure_distance(infrared_sensors[i]) > WALL_THRESHOLD;
			blocked[i] = map_value;
				switch (i)
				{
				case 0:
					map[location[0]][location[1] - 1] = map_value;
					break;
				case 1:
					map[location[0] + 1][location[1]] = map_value;

					break;
				case 2:
					map[location[0]][location[1]+1] = map_value;

					break;
				case 3:
					map[location[0]-1][location[1]] = map_value;

					break;
				default:
					break;
				}
			
		}
		break;
	case 2:
		for (int i = 0; i < 4; i++)
		{
			map_value = measure_distance(infrared_sensors[i]) > WALL_THRESHOLD;
			blocked[i] = map_value;
				switch (i)
				{
				case 0:
					map[location[0]+1][location[1] ] = map_value;
					break;
				case 1:
					map[location[0]][location[1]+1] = map_value;

					break;
				case 2:
					map[location[0] - 1][location[1]] = map_value;

					break;
				case 3:
					map[location[0]][location[1] - 1] = map_value;

					break;
				default:
					break;
				
			}
		}
		break;
	case 3:
		for (int i = 0; i < 4; i++)
		{
			map_value = measure_distance(infrared_sensors[i]) > WALL_THRESHOLD;
			blocked[i] = map_value;
				switch (i)
				{
				case 0:
					map[location[0]][location[1]+1] = map_value;
					break;
				case 1:
					map[location[0]-1][location[1]] = map_value;

					break;
				case 2:
					map[location[0]][location[1] - 1] = map_value;

					break;
				case 3:
					map[location[0] + 1][location[1]] = map_value;
					break;
				default:
					break;
				}
			
		}
		break;
	default:
		break;
	}
	if (!goal_location[0]==-1)
	{
		map[goal_location[0]][goal_location[1]] = 3;
	}
}

void locate_forward(int orientation, int map[][MATRIX_LIMIT], int location[]) {
	map[location[0]][location[1]] = 1;
	printf("%d,%d\n", location[0], location[1]);
	switch (orientation)
	{
	case 0:
		location[0] = location[0] - 1;
		map[location[0]][location[1]] = 2;
		break;
	case 1:
		location[1] = location[1] - 1;
		map[location[0]][location[1]] = 2;
		

		break;
	case 2:
		location[0] = location[0] + 1;
		map[location[0]][location[1]] = 2;
		

		break;
	case 3:
		location[1] = location[1] + 1;
		map[location[0]][location[1]] = 2;
		
		break;
	default:
		break;
	}
	printf("%d,%d\n", location[0], location[1]);
}
void locate_goal(int orientation, int goal_location[], int location[]) {
	switch (orientation)
	{
	case 0:
		goal_location[0] = location[0]-1;
		goal_location[1] = location[1];
		break;
	case 1:
		goal_location[0] = location[0];
		goal_location[1] = location[1]-1;
		break;
	case 2:
		goal_location[0] = location[0]+1;
		goal_location[1] = location[1];

		break;
	case 3:
		goal_location[0] = location[0];
		goal_location[1] = location[1]+1;

		break;
	default:
		break;
	}
}
int main(int argc, char** argv) {
	/* necessary to initialize webots stuff */
	wb_robot_init();
	WbDeviceTag camera = wb_robot_get_device("camera");
	WbDeviceTag left_motor = wb_robot_get_device("left wheel motor");
	WbDeviceTag right_motor = wb_robot_get_device("right wheel motor");
	WbDeviceTag left_sensor = wb_robot_get_device("left wheel sensor");
	WbDeviceTag right_sensor = wb_robot_get_device("right wheel sensor");
	WbDeviceTag infrared_sensors[4] = { wb_robot_get_device("front infrared sensor") , wb_robot_get_device("left infrared sensor") ,wb_robot_get_device("rear infrared sensor") ,wb_robot_get_device("right infrared sensor") };
	int blocked[4] = { 0,0,0,0 };
	int map[MATRIX_LIMIT][MATRIX_LIMIT];
	int orientation = 0;
	int wall_found = 0;
	int possible_outer_corner = 0;
	init_map(&map);
	int location[2] = { MATRIX_LIMIT / 2,MATRIX_LIMIT / 2 };
	int initial_location[2] = {location[0],location[1]};
	int goal_location[2] = { -1,-1 };
	map[location[0]][location[1]] = 1;
	enable_sensors(camera,left_sensor, right_sensor, &infrared_sensors);
	double sensors_previous_position[2] = {0,0 };
	double before_movement_sensor_state[2] = { 0,0 };

	wb_motor_set_velocity(left_motor, 0.1 * MAX_SPEED);
	wb_motor_set_velocity(right_motor, 0.1 * MAX_SPEED);


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
		if (stopped(left_motor, left_sensor, right_motor, right_sensor))
		{
			is_goal(camera);
			update_map(infrared_sensors, orientation, &map, location, &blocked, goal_location);
			
			printf("%d,%d,%d,%d\n", blocked[0], blocked[1], blocked[2], blocked[3]);
			print_matrix(&map);
			if (!wall_found)
			{
				if (blocked[3])
				{
					wall_found = 1;
				}
				else
				{
					for (int i = 0; i < 3; i++)
					{
						if (blocked[i])
						{
							if (stopped(left_motor, left_sensor, right_motor, right_sensor))
							{
								turn(left_motor, left_sensor, right_motor, right_sensor, &orientation, 1);
							}
						}
					}

					if (stopped(left_motor, left_sensor, right_motor, right_sensor))
					{
						advance1(left_motor, left_sensor, right_motor, right_sensor);
						locate_forward(orientation, &map, &location);
					}
				}
			}
			else
			{

				if (!blocked[3])
				{
					if (possible_outer_corner == 2)
					{
						advance1(left_motor, left_sensor, right_motor, right_sensor);
						locate_forward(orientation, &map, &location);
						possible_outer_corner = 0;
					}
					else
					{
						turn(left_motor, left_sensor, right_motor, right_sensor, &orientation, 1);
						if (possible_outer_corner==1)
						{
							printf("kappa");
							possible_outer_corner=2;
						}
					}
					
				}

				else
				{
					if (!blocked[0])
					{
						advance1(left_motor, left_sensor, right_motor, right_sensor);
						locate_forward(orientation, &map, &location);
						possible_outer_corner = 1;


					}
					else
					{
						turn(left_motor, left_sensor, right_motor, right_sensor, &orientation, 0);
					}
				}
			}

			//turn(left_motor, right_motor, 0, &before_movement_sensor_state);
		}

	sensors_previous_position[0] = wb_position_sensor_get_value(left_sensor);
	sensors_previous_position[1] = wb_position_sensor_get_value(right_sensor);
	};

	/* Enter your cleanup code here */

	/* This is necessary to cleanup webots resources */
	wb_robot_cleanup();

	return 0;
}

/*
dest_x = 4
dest_y = 1
visited = []

def next(path, x, y) :
	visited.append([x, y])

	# obstacle
	if matrix[x][y] == 0:
return None

# found destination
if x == dest_x and y == dest_y:
return path

for i in[{'x': x - 1, 'y' : y, 'direction' : 'left'},
{ 'x': x + 1, 'y' : y, 'direction' : 'right' },
{ 'x': x, 'y' : y - 1, 'direction' : 'up' },
{ 'x': x, 'y' : y + 1, 'direction' : 'down' }]:
if[i['x'], i['y']] not in visitedand i['x'] >= 0 and i['x'] < len(matrix) and i['y'] >= 0 and i['y'] < len(matrix[x]) :
	n = next(path + [i['direction']], i['x'], i['y'])
	if n != None :
		return n




		matrix = [[1, 1, 1, 1],
		[1, 1, 1, 1],
		[1, 0, 0, 1],
		[1, 1, 1, 1],
		[1, 1, 1, 1]]

		print(next([], 0, 1))
		*/