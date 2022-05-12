#pragma config(StandardModel, "EV3_REMBOT")
int currentPriority=0;
TSemaphore semaforo;
int canIWork(int asked_priority){
	int value;
	semaphoreLock(semaforo);
	if(bDoesTaskOwnSemaphore(semaforo)){
		if(asked_priority>=currentPriority){
			currentPriority = asked_priority;
			value=1;
		}else
			value=0;
		semaphoreUnlock(semaforo);
	}

	return value;
}
void doneWorking(int priority){
	semaphoreLock(semaforo);
	if(bDoesTaskOwnSemaphore(semaforo)){
		if(priority==2){
			stopAllTasks();
		}
		if(priority>0){
			currentPriority--;
		}
		semaphoreUnlock(semaforo);
	}
}

int measure(){
	int tiempo=0;
	int i=0;
	int valor=0;
	clearTimer(T1);
	tiempo= getTimerValue(T1);
	while(tiempo<1000){
		valor=valor+SensorValue[sonarSensor];
		i++;
		tiempo= getTimerValue(T1);
	}
	valor=valor/i;
	return valor;
}

task buscarPared(){
	const int LimitDistance = 25;
	int currentDistance = 0;
	int const myPriority=0;
	while(true){
		if(canIWork(myPriority)){
			currentDistance= measure();
			while(currentDistance>LimitDistance){
				setMotorSpeed(leftMotor, 50);
				setMotorSpeed(rightMotor, 50);
				currentDistance=measure();
			}
			doneWorking(myPriority);
		}else abortTimeslice();
	}
}

task seguirPared(){
	const int LimitDistance = 25;
	int currentDistance=1;
	int const myPriority=1;
	int angle_before=0;
	int correction=-70;
	int time;
	while(true){
		if(canIWork(myPriority)){
			currentDistance=SensorValue[sonarSensor];
			if (currentDistance<=LimitDistance){
				resetGyro(gyroSensor);
				angle_before=getGyroDegrees(gyroSensor);
				while(angle_before<60){
					currentDistance=SensorValue[sonarSensor];
					angle_before=getGyroDegrees(gyroSensor);
					setMotorSpeed(leftMotor, 20);
					setMotorSpeed(rightMotor, -20);
				}

				clearTimer(T2);
				currentDistance=SensorValue[sonarSensor];
				time=time1[T2];
				while(currentDistance>LimitDistance && time<2000){
					setMotorSpeed(leftMotor, 20);
					setMotorSpeed(rightMotor, 20);
					currentDistance=SensorValue[sonarSensor];
					time=time1[T2];
				}
				resetGyro(gyroSensor);
				angle_before=getGyroDegrees(gyroSensor);
				while(angle_before>-80&& currentDistance>LimitDistance+15){
					setMotorSpeed(leftMotor, -15  );
					setMotorSpeed(rightMotor, 15  );
					angle_before=getGyroDegrees(gyroSensor);
					currentDistance=SensorValue[sonarSensor];
				}
				correction=getGyroDegrees(gyroSensor);
			}
		}else abortTimeslice();
		doneWorking(myPriority);
	}
}
task irLuz(){
	int const myPriority=2, umbral = 25,umbralStop = 30;
	float currentLight=100;
	while(true){
		currentLight=getColorAmbient(colorSensor);
		writeDebugStreamLine("Valor recogido por el sensor de luz= %f\n ", currentLight);

		if (currentLight > umbral){
			if (canIWork(myPriority)){
					setMotorSpeed(leftMotor, 0 );
					setMotorSpeed(rightMotor, 0  );
				doneWorking(myPriority);
			}
		}else abortTimeslice();
	}
}


task escapar(){
	int const myPriority=3;
	int minDistance=20;
	int currentDistance=0;
	int touching=0;
	while(true){
		touching=getTouchValue(touchSensor);
		currentDistance=SensorValue[sonarSensor];
		if(currentDistance<minDistance || touching){
			if (canIWork(myPriority)){
				while(currentDistance<minDistance || touching){
					setMotorSpeed(leftMotor, -40);
					setMotorSpeed(rightMotor, -40);
					currentDistance=SensorValue[sonarSensor];
				}
				doneWorking(myPriority);
			}else abortTimeslice();
		}
	}
}
task main()
{

	semaphoreInitialize(semaforo);
	clearDebugStream();
	startTask(buscarPared);
	startTask(seguirPared);
	startTask(irLuz);
	startTask(escapar);
	while(true){
	}
}
