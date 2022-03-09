#pragma config(StandardModel, "EV3_REMBOT")
int currentPriority=0;
int canIWork(int asked_priority){
	if(asked_priority>currentPriority){
		return 1;
	}else
		return 0;
}
void doneWorking(int priority){

}

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
}

task buscarPared(){
	const int LimitDistance = 30;
	int currentDistance = 0;
	while(true){
		if (canIWork(0)){
			currentDistance= SensorValue[sonarSensor];
			while(currentDistance>LimitDistance){
				setMotorSpeed(leftMotor, 50);
				setMotorSpeed(rightMotor, 50);
				currentDistance= SensorValue[sonarSensor];
			}
		setMotorSpeed(leftMotor, 0);
		setMotorSpeed(rightMotor, 0);
		}else abortTimeslice();
	}
}

task seguirPared(){
	while(wallFound){

	}
}

task main()
{
	clearDebugStream();
	startTask(buscarPared);
	startTask(seguirPared);
	while(true){
	}
}
