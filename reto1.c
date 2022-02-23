#pragma config(StandardModel, "EV3_REMBOT")
const int Limitdistance = 20;
int currentDistance = 0;

void stop_movement()
{
		setMotorSpeed(leftMotor, 0);
		setMotorSpeed(rightMotor, 0);
}
void default_movement()
{
		setMotorSpeed(leftMotor, 50);
		setMotorSpeed(rightMotor, 50);
}
void giro_grados_izquierda(int grados)
{
	int current_degrees;
	resetGyro(gyroSensor);
	current_degrees=getGyroDegrees(gyroSensor);
	while(current_degrees>=grados)
	{
		current_degrees=getGyroDegrees(gyroSensor);
		setMotorSpeed(rightMotor, 30);
	}
}
void giro_grados_derecha(int grados)
{
	int current_degrees;
	resetGyro(gyroSensor);
	current_degrees=getGyroDegrees(gyroSensor);
	while(current_degrees<=grados)
	{
		current_degrees=getGyroDegrees(gyroSensor);
		setMotorSpeed(leftMotor, 30);
	}
}
task main()
{
	default_movement();
	while (true)
	{
		currentDistance= SensorValue[sonarSensor];
		if(Limitdistance>=currentDistance)
		{
			stop_movement();
			giro_grados_derecha(90);
			default_movement();


		}
		if(currentDistance>=Limitdistance){
				sleep(1000);
				giro_grados_izquierda(-90);
				while(SensorValue[sonarSensor]>=Limitdistance)
				{
					default_movement();
				}
				stop_movement();
				giro_grados_derecha(90);
				default_movement();
			}
	}
}
