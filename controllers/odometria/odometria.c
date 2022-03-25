#include <webots/motor.h>
#include <webots/robot.h>
#define TIME_STEP 64
#define MAX_SPEED 6.28
int main(int argc, char **argv){
  wb_robot_init();
  WbDeviceTag left_motor= wb_robot_get_device("left wheel motor");
  WbDeviceTag right_motor= wb_robot_get_device("right wheel motor");
  wb_motor_set_position(left_motor, INFINITY);
  wb_motor_set_position(right_motor, INFINITY);
  
  wb_motor_set_velocity(left_motor,0.1*MAX_SPEED);
  wb_motor_set_velocity(right_motor,0.1*MAX_SPEED);
  
  while(wb_robot_step(TIME_STEP)!=-1){
  }
  wb_robot_cleanup();
}