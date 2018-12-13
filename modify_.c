#pragma config(Motor, motorB, lm, PIDControl, encoder)
#pragma config(Motor, motorC, rm, PIDControl, encoder)
#pragma config(Sensor, S1, cs1, sensorEV3_Color)
#pragma config(Sensor, S2, cs2, sensorEV3_Color)
#pragma config(Sensor, S3, gs, sensorEV3_Gyro)
#pragma config(Sensor, S4, us, sensorEV3_Ultrasonic)

int stack[30];
int dark[2], white[2];
int bound;

void rightturn()
{
	resetGyro(gs);
	while(getGyroDegrees(gs) < 90)
	{
		setMotorSpeed(lm, 30);
		setMotorSpeed(rm, -30);
	}
}

void leftturn()
{
	resetGyro(gs);
	while(getGyroDegrees(gs) > -90)
	{
		setMotorSpeed(lm, -30);
		setMotorSpeed(rm, 30);
	}
}

void scanLine(){
	displayBigTextLine(1, "scan dark & push");
	while(getButtonPress(1) == 0) sleep(10);
	playTone(440, 20);
	dark[0] = getColorReflected(cs1);
	dark[1] = getColorReflected(cs2);
	sleep(1000);

	displayBigTextLine(1, "scan white & push");
	resetMotorEncoder(lm);
	setMotorSpeed(lm, 30);
	setMotorSpeed(rm, 30);
	sleep(1000);
	playTone(440, 20);
	white[0] = getColorReflected(cs1);
	white[1] = getColorReflected(cs2);
	sleep(1000);

//	for(int i = 0 ; i<2 ; i++) gray[i] = (dark[i]+white[i])/2;
	bound = (dark[0]+dark[1]+white[0]+white[1])/4;
}

task main()
{
	scanLine();
	int i = -1;
	/*displayBigTextLine(1, "%d", dark[1]);
	displayBigTextLine(3, "%d", white[1]);
	displayBigTextLine(5, "%d", bound);*/
	eraseDisplay();
	resetGyro(gs);
	int curval;
	while(1){
		sleep(1);
		int left = getColorReflected(cs1);
		int right = getColorReflected(cs2);

		if(right < bound || left < bound)
		{
			setMotorSpeed(lm, 30);
			setMotorSpeed(rm, 30);
			sleep(100);

			left = getColorReflected(cs1);
			right = getColorReflected(cs2);

			if(right < bound && left >= bound){
				i++;
	 	 		sleep(600); // go front little
	 	 		stack[i] = getMotorEncoder(lm); // modify point (change order)
	 	 		i++;
	 		  	stack[i] = 2; // right
	  			rightturn();
	  			resetGyro(gs);
	  			resetMotorEncoder(lm);
			  }

			  else if(left < bound && right >= bound){
		  		i++;
		  		sleep(600); // go front little
		  		stack[i] = getMotorEncoder(lm); // modify point (change order)
		  		i++;
		  		stack[i] = 1; // left
		  		leftturn();
		  		resetGyro(gs);
		  		resetMotorEncoder(lm);
			  }

			  else if((left < bound) && (right < bound)){
			  	i++;
		  		stack[i] = getMotorEncoder(lm);
		  		setMotorSpeed(lm, 0);
				setMotorSpeed(rm, 0);
				sleep(3000);
				break;
			  	}
			  }
	  	else
	  	{
	  		curval = getGyroDegrees(gs);
	 			setMotorSpeed(lm, 30 - curval);
				setMotorSpeed(rm, 30 + curval);
			}
 	}

 	for(int j = 0; j <= i; j++)
 	{
 		//displayBigTextLine(1, "%d", i);
 	 	displayBigTextLine((2*j)+1, "%d=%d",j, stack[j]);
 	}
 	//displayBigTextLine(1, "%d", i);
 	sleep(10000);
}
