#pragma config(StandardModel, "EV3_REMBOT")
const int Limitdistance = 15;
int currentDistance = 0;
TEV3LEDPatterns color=ledOrange;
int direccion;


task giroTiempo()
{
	direccion= rand() % 2;
	if(direccion)
	{
		setMotorSpeed(leftMotor, 50);
		setMotorSpeed(rightMotor, -50);
	}else
	{
		setMotorSpeed(leftMotor, -50);
		setMotorSpeed(rightMotor, 50);
	}
	sleep(1000);
}
task giroGrados(int direccion)
{
	direccion= rand() % 2;
	if(direccion)
	{
		setMotorTarget(leftMotor, 30, 50);
	}else
	{
		setMotorTarget(rightMotor, 30, 50);
	}

}

task main()
{
	setMotorSpeed(leftMotor, 50);
	setMotorSpeed(rightMotor, 50);
	currentDistance= SensorValue[cont];
	while (true)
	{
		currentDistance= SensorValue[sonarSensor];
		if (currentDistance<=2*Limitdistance)
		{
			setLEDColor(color);
		}
		if (getTouchValue(touchSensor))
		{
			setMotorSpeed(leftMotor, -50);
			setMotorSpeed(rightMotor, -50);
			sleep(2000);
			giroTiempo();
			setMotorSpeed(leftMotor, 50);
			setMotorSpeed(rightMotor, 50);
		}
	}
}
