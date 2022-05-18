#include <webots/motor.h>
#include <webots/robot.h>
#include <webots/distance_sensor.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_SPEED 6.0
#define TIME_STEP 64
#define WHEEL_RADIX 0.021
#define WHEEL_GAP 0.1054
/*
  S1 := Sale por la derecha
  S2 := Sale por la izquierda
  S3 := Otro
*/

#define S1 0   
#define S2 1
#define S3 2

/*
 * A1 := Gira a la derecha
 * A2 := Gira a la izquierda
 * A3 := Hacia delante
 */

#define A1 0
#define A2 1
#define A3 2



int main(int argc, char** argv) {
  wb_robot_init();

  /*  
  WbDeviceTag left_motor = wb_robot_get_device("left wheel motor");
  WbDeviceTag right_motor = wb_robot_get_device("right wheel motor");
  WbDeviceTag left_sensor = wb_robot_get_device("left wheel sensor");
  WbDeviceTag right_sensor = wb_robot_get_device("right wheel sensor");
  WbDeviceTag infrared_sensors[4] = { wb_robot_get_device("front infrared sensor") , wb_robot_get_device("left infrared sensor") ,wb_robot_get_device("rear infrared sensor") ,wb_robot_get_device("right infrared sensor") };
  */while (wb_robot_step(TIME_STEP) != -1) {
  
  }
  wb_robot_cleanup();

  return 0;
}