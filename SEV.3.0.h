
#include <Wire.h>

#define		SDA		18
#define		SCL		19

void init_wire1()
{
	Wire1.setSDA(SDA);
	Wire1.setSCL(SCL);
	Wire1.begin();
	
	// **************************************************
	Serial.println("\nI2C Scanner");
	byte error, address;
	int nDevices;
	
	Serial.println("Scanning...");
	
	nDevices = 0;
	
	for(address = 1; address < 127; address++ )
	{
		Wire1.beginTransmission(address);
		error = Wire1.endTransmission();
		
		if (error == 0)
		{
			Wire1.requestFrom(address, 1);     // Solicitud de 1 Byte.
			int intTemp = Wire1.read();
			
			Serial.print("Dispositivo I2C encontrado en la direccion 0x");
			if (address<16) Serial.print("0");
			Serial.print(address,HEX);
			
			Serial.print("   ");
			switch(address)
			{
				case 0x20: Serial.print("Ctrl Prueba de elec.  ");  break;
				case 0x48: Serial.print("ADS1115 para medida de V. ");  break;
				case 0x49: Serial.print("ADS1115 P. elec.  ");  break;
				case 0x68: Serial.print("Reloj RTC DS3231.     ");  break;
			}
			
			Serial.print("  !");
			Serial.println(0xff&intTemp, BIN);
			
			nDevices++;
		}
		else if (error==4)
		{
			Serial.print("Unknow error at address 0x");
			if (address<16)   Serial.print("0");
			Serial.println(address,HEX);
		}
	}
	if (nDevices == 0)  Serial.println("No se encontraron dispositivos I2C\n");
	else                Serial.println("done\n");
	
}

// --------------------------------------------------
void	proc_PTEMPER()
{
	niveles.medirFuentes();
	
	if(NEXTION_PAGE == PTEMPER)
	{
		niveles.medirTemp();
		Serial.println("Muestra voltajes en grafica.");
		
		int		Ap = niveles.getAp();
		float	V1, V2, V3, V4;
		
		Ap--;	if(Ap < 0) Ap=199;
		niveles.getData(&V3, &V2, &V4, &V1, Ap);
		
		char buff[10];
		if(V4 != 0)
		{
			P1_wGraf0.addValue(0, 5+100*(V1-10.5)/4);
			sprintf(buff, "%.2fV", V1);      P1_tVolt1.setText(buff);
			P1_wGraf1.addValue(0, 5+100*(V2-3)/0.7);
			sprintf(buff, "%.2fV", V2);      P1_tVolt2.setText(buff);
			P1_wGraf2.addValue(0, 5+100*(V3-3.1)/0.4);
			sprintf(buff, "%.2fV", V3);      P1_tVolt3.setText(buff);
			P1_wGraf3.addValue(0, 5+100*(V4-4.8)/0.4);
			sprintf(buff, "%.2fV", V4);      P1_tVolt4.setText(buff);
		}
		
		// --------------------------------------------------
		P1_nTemp1.setValue((int)niveles.TempInterna);
		P1_nTemp2.setValue((int)niveles.TempCPU);
   
		// --------------------------------------------------
		if(gps_neo.hdop>0)
		{
			P1_tCoord.setText(gps_neo.coordenadas);
		}
	}
}

// --------------------------------------------------
void	proc_GPS()
{
//	int n = Serial.availableForWrite();
//	Serial.print("n: ");    Serial.println(n);
	gps_neo.tOut = millis() + 1000;
	gps_neo.verHora();
	
	if(!gps_neo.RTC_actualizado) gps_neo.leerGPS();
	
	// BLINKING...
	if(digitalRead(LED_BUILTIN))    digitalWrite(LED_BUILTIN, LOW);
	else                            digitalWrite(LED_BUILTIN, HIGH);
}



// Arreglo con nombres de proyectos.

// Datos proyecto abierto.

// Arreglo con nombres de SEVs.

// Datos de SEV abierto.
