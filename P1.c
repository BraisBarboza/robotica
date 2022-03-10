#pragma config(StandardModel, "EV3_REMBOT")
int currentPriority=0;
int canIWork(int asked_priority){
	if(asked_priority>=currentPriority){
		currentPriority = asked_priority;
		return 1;
	}else
		return 0;
}
void doneWorking(int priority){
	currentPriority--;
}
/*
task escapar(){
	const int LimitDistance = 30;
	while(true){
		currentDistance= SensorValue[sonarSensor];
		if(currentDistance<10){
			if(canIWok(3)){
				setMotorSpeed(leftMotor, -50);
				setMotorSpeed(rightMotor, -50);
			}
		}
	}
}*/

task buscarPared(){
	const int LimitDistance = 20;
	int currentDistance = 0;
	int const myPriority=0;
	while(true){
		if (canIWork(myPriority)){
			currentDistance= SensorValue[sonarSensor];
			while(currentDistance>LimitDistance){
				setMotorSpeed(leftMotor, 70);
				setMotorSpeed(rightMotor, 50);
				currentDistance= SensorValue[sonarSensor];
			}
		}else abortTimeslice();
		doneWorking(myPriority);
	}

}

task seguirPared(){
	const int LimitDistance = 20;
	int currentDistance=0;
	int const myPriority=1;
	while(true){
		currentDistance=SensorValue[sonarSensor];
		if(currentDistance<=LimitDistance){
			if (canIWork(myPriority)){
				resetGyro(gyroSensor);
				while(currentDistance<=LimitDistance || getGyroDegrees(gyroSensor)>-60){
					currentDistance=SensorValue[sonarSensor];
					setMotorSpeed(leftMotor, -50);
					setMotorSpeed(rightMotor, 50);
				}
				setMotorSpeed(leftMotor, 70);
				setMotorSpeed(rightMotor, 40);
			}
			doneWorking(myPriority);
		}
	}
}
task irLuz(){
	int const myPriority=2;
	int defaultLight=0;
	int currentLight=0;
	defaultLight=getColorAmbient(colorSensor);
	while(true){
		currentLight=getColorAmbient(colorSensor);
		//if(defaultLight!=currentLight){
		if(getColorName(colorSensor)==colorRed){
			if (canIWork(myPriority)){
				setMotorSpeed(leftMotor, 50);
				setMotorSpeed(rightMotor, 50);
				/*
				while(currentLight<defaultLight){
					setMotorSpeed(leftMotor, -20);
					setMotorSpeed(rightMotor, 20);
				}
				setMotorSpeed(leftMotor, 50);
				setMotorSpeed(rightMotor, 50);
			*/}
			doneWorking(myPriority);
		}
	}
}
task escapar(){
	int const myPriority=3;
	int minDistance=15;
	int currentDistance=0;
	int touching=0;
	while(true){
		touching=getTouchValue(touchSensor);
		currentDistance=SensorValue[sonarSensor];
		if(currentDistance<minDistance || touching){
			if (canIWork(myPriority)){
				while(currentDistance<minDistance || touching){
					setMotorSpeed(leftMotor, -20);
					setMotorSpeed(rightMotor, -20);
					currentDistance=SensorValue[sonarSensor];
				}

			}
			doneWorking(myPriority);
		}
	}
}
task main()
{
	clearDebugStream();
	startTask(buscarPared);
	startTask(seguirPared);
	startTask(irLuz);
	startTask(escapar);
	while(true){
	}
}
