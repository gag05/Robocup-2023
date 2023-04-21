void gira_destra(){
	setMotorTarget(motorB,30,-200);
	waitUntilMotorStop(motorB);
}

void gira_sinistra(){
	setMotorTarget(motorC,200,30);
	waitUntilMotorStop(motorC);
}

//funzione di quando non trova più il colore nero
void no_nero(){
	setMotorSpeed(motorD,5);
			
	if(getColorReflected(S4) < 16)
		while(getColorReflected(S4) <16){
			setMotorSpeed(motorB,20);
			setMotorSpeed(motorC,-25);
		}
	else
		while(getColorReflected(S2) < 16){
			setMotorSpeed(motorB,-25);
			setMotorSpeed(motorC,20);
		}
}

void linea_spezzata(){
	while(getColorReflected(S2) > 14 && getColorReflected(S3) > 14 && getColorReflected(S4) > 14){
		setMotorSpeed(motorB,20);
		setMotorSpeed(motorC,20);
		setMotorSpeed(motorD,10);
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
	while(getColorReflected(S2) > 14){
		setMotorSpeed(motorB,20);
		setMotorSpeed(motorC,20);
		setMotorSpeed(motorD,10);
	}
	
	setMotorSpeed(motorB,20);
	setMotorSpeed(motorC,20);
	setMotorSpeed(motorD,10);
	//si rimette dritto
	moveMotorTarget(motorB,400,60);
	waitUntilMotorStop(motorB);
	//bianco 83-70
	//nero 9
	//nero-bianco 16
}

task main(){
	int crr,crl,crc;
  // sensore destro 4
	// sensore centrale 3
	// sensore sisnistro 2
	while(true){
		//sensore in nero

		do{
			crr = getColorReflected(S4);
			crc = getColorReflected(S3);
			crl = getColorReflected(S2);

			//if(crr - crl < -10 || crl - crr > 10)


			setMotorSpeed(motorB,20);
			setMotorSpeed(motorC,20);
			setMotorSpeed(motorD,10);
			//controllare porta del sensore di distanza
			int distanza = getUSDistance(S1);
			if(getUSDistance(S1) < 6){
				setMotorSpeed(motorB,0);
				setMotorSpeed(motorC,0);
				setMotorSpeed(motorD,0);
				ostacolo();
			}
				
			crr = getColorReflected(S4);
			crc = getColorReflected(S3);
			crl = getColorReflected(S2);

		}while(crc <= 14 && crr > 16 && crl > 16);

		if(crc > 16 && crr > 16 && crl > 16)
			linea_spezzata();
		else
			no_nero();

	}
}
