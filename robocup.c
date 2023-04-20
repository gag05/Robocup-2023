// modifica
//void gira_180(){}
void gira_destra(){
	setMotorTarget(motorB,30,-200);
	waitUntilMotorStop(motorB);
}

void gira_sinistra(){
	setMotorTarget(motorC,200,30);
	waitUntilMotorStop(motorC);
}

void movimento(int v1,int g){
	setMotorTarget(motorB,g,v1);
	setMotorTarget(motorC,g,v1);
	waitUntilMotorStop(motorC);
	waitUntilMotorStop(motorB);
}

//funzione di quando non trova più il colore nero
void no_nero(){
	while(getColorReflected(S3)<=11){
		setMotorSpeed(motorB,40);
		setMotorSpeed(motorC,-40);
	}
	while(getColorReflected(S1) <= 12){
		setMotorSpeed(motorB,-40);
		setMotorSpeed(motorC,40);
	}
}

void incrocio(){

	TLegoColors cl,cr;
	
	setMotorSpeed(motorB,0);
	setMotorSpeed(motorC,0);
	setMotorSpeed(motorD,0);
	
	while(getColorName(S1) != colorGreen && getColorName(S3) != colorGreen){
			setMotorSpeed(motorB,-10);
			setMotorSpeed(motorC,-10);
			setMotorSpeed(motorD,10);
	}
	setMotorSpeed(motorB,0);
	setMotorSpeed(motorC,0);
	setMotorSpeed(motorD,0);

	cl = getColorName(S1);
	cr = getColorName(S3);

	if(cr == colorGreen && cl == colorGreen ){}
	else if(cr == colorGreen){
		gira_destra();
		return;
	}else if(cl == colorGreen){
		gira_sinistra();
		return;
	}
	setMotorSpeed(motorB,30);
	setMotorSpeed(motorC,30);
	setMotorSpeed(motorD,-30);


}

void discesa(){
	int crr,crl;
	do{
				crr = getColorReflected(S3);
				crl = getColorReflected(S1);


				setMotorSpeed(motorB,20);
				setMotorSpeed(motorC,20);
				setMotorSpeed(motorD,-20);
		}while(crr>12 && crl >12);
		no_nero();

}


void salita(){
	while(getGyroDegrees(S2) < 3){
		setMotorSpeed(motorB,40);
		setMotorSpeed(motorC,40);
		setMotorSpeed(motorD,-40);
	}
	/*setMotorTarget(motorB,270,80);
	setMotorTarget(motorC,270,80);
	waitUntilMotorStop(motorB);
	waitUntilMotorStop(motorC);*/

}

void ostacolo(){
	//primo movimento 90°
	moveMotorTarget(motorB,400,60);
	waitUntilMotorStop(motorB);
	//mov in avanti
	moveMotorTarget(motorB,270,60);
	moveMotorTarget(motorC,270,60);
	moveMotorTarget(motorD,270,-60);
	waitUntilMotorStop(motorB);
	waitUntilMotorStop(motorC);
	//dritto
	moveMotorTarget(motorC,450,60);
	waitUntilMotorStop(motorC);
	//supera ostacolo
	moveMotorTarget(motorB,500,60);
	moveMotorTarget(motorC,500,60);
	moveMotorTarget(motorD,500,-60);
	waitUntilMotorStop(motorB);
	waitUntilMotorStop(motorC);
	//traiettoria verso la linea nera
	moveMotorTarget(motorC,400,60);
	waitUntilMotorStop(motorC);
	//torna sulla linea nera
	moveMotorTarget(motorB,270,60);
	moveMotorTarget(motorC,270,60);
	moveMotorTarget(motorD,270,-60);
	waitUntilMotorStop(motorB);
	waitUntilMotorStop(motorC);
	//si rimette dritto
	moveMotorTarget(motorB,400,60);
	waitUntilMotorStop(motorB);
}

task main(){
	int crr,crl;

	resetGyro(S2);
	while(true){
			//sensore in nero

		do{
				crr = getColorReflected(S3);
				crl = getColorReflected(S1);


				setMotorSpeed(motorB,20);
				setMotorSpeed(motorC,20);
				setMotorSpeed(motorD,-10);
				//controllare porta del sensore di distanza
				if(getUSDistance(S4) < 5){
					setMotorSpeed(motorB,0);
					setMotorSpeed(motorC,0);
					ostacolo();
				}


				if(getGyroDegrees(S2) < -10)
					salita();
				else if(getGyroDegrees(S2) > 15)
					discesa();

		}while(crr>10 && crl >10);
			if(crr<=20 && crl <=20)
				incrocio();
			no_nero();

	}
}
