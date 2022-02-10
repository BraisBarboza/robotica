#pragma config(StandardModel, "EV3_REMBOT")
const int Limitdistance = 15;
int currentDistance = 0;
TEV3LEDPatterns color=ledOrange;
task main()
{
	setMotorSpeed(leftMotor, 50);
	setMotorSpeed(rightMotor, 50);
	currentDistance= SensorValue[sonarSensor];
  while (currentDistance>Limitdistance)
  {
  	currentDistance= SensorValue[sonarSensor];
  	if (currentDistance<=2*Limitdistance)
  	{
  		setLEDColor(color);

  	}
	}
}
