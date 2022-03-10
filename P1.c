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
	if(priority>0){
		currentPriority--;
	}
}

int measure(){
	int tiempo=0;
	int i=0;
	int valor=0;
	clearTimer(T1);
	tiempo= getTimerValue(T1);
  while(tiempo<100){
  	valor=valor+SensorValue[sonarSensor];
  	i++;
  	tiempo= getTimerValue(T1);
  	 setMotorSpeed(leftMotor, 0);
		setMotorSpeed(rightMotor, 0);
  }
  valor=valor/i;
  return valor;
}

task buscarPared(){
	const int LimitDistance = 35;
	int currentDistance = 0;
	int const myPriority=0;
	int tiempo=0;
	while(true){
		if (canIWork(myPriority)){
			currentDistance= measure();
			while(currentDistance>LimitDistance){
				clearTimer(T2);
				tiempo= getTimerValue(T2);
  			while(tiempo<1000){
  				setMotorSpeed(leftMotor, 30);
					setMotorSpeed(rightMotor, 50);
					tiempo= getTimerValue(T2);
  			}
				currentDistance=measure();
			}
			doneWorking(myPriority);
		}else abortTimeslice();

	}

}

task seguirPared(){
	const int LimitDistance = 35;
	int currentDistance=0;
	int const myPriority=1;
	while(true){
		currentDistance=SensorValue[sonarSensor];
		if(currentDistance<=LimitDistance){
			if (canIWork(myPriority)){
      	resetGyro(gyroSensor);
				while(currentDistance<=LimitDistance || getGyroDegrees(gyroSensor)<60){
					currentDistance=measure();
					setMotorSpeed(leftMotor, 50);
					setMotorSpeed(rightMotor, -50);
				}
				setMotorSpeed(leftMotor, 40);
				setMotorSpeed(rightMotor, 60);
			}
			doneWorking(myPriority);

		}
	}
}
task irLuz(){
	int const myPriority=2;
	int currentLight=0;
	while(true){
		currentLight=getColorAmbient(colorSensor);
		//if(currentLight<60){
		if(getColorName(colorSensor)==colorRed){
			if (canIWork(myPriority)){
				setMotorSpeed(leftMotor, 50);
				setMotorSpeed(rightMotor, 50);
				doneWorking(myPriority);
			}

		}
	}
}
task escapar(){
	int const myPriority=3;
	int minDistance=30;
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
				doneWorking(myPriority);
			}

		}
	}
}
task main()
{
	clearDebugStream();
	startTask(buscarPared);
	startTask(seguirPared);
	//startTask(irLuz);
	//startTask(escapar);
	while(true){
	}
}
