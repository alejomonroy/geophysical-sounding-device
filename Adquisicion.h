#include "ADS1252.h"
#include "FuenteAB.h"

#define		ADS_CLK		22
#define		ADS0_MISO	17
#define		ADS0_SCK	16
#define		ADS1_MISO	21
#define		ADS1_SCK	20

#define		AB_POWER	15
#define		AB_POL		14
#define   AB_ENABLE 13

ADS1252 ads1252;

// ***************************************************************************************************
// *																									*
// ****************************************************************************************************
void gananciaVmn(float nivelmn)
{
	// debe iniciar la adquisicion con una ganancia minima, si el valor esta por debajo de un nivel minimo.
	// se aumenta la ganancia proporcionalmente al valor optenido.
	int ganancia=0;
	
	if((-900 < nivelmn)&&(nivelmn < 900))	ganancia=1;
	if((-240 < nivelmn)&&(nivelmn < 240))	ganancia=2;
	if((-80  < nivelmn)&&(nivelmn <  80))	ganancia=3;
	
	ads1252.setGanancia(ganancia);
	delay(1);
}

//* ******************************************************************
//*								ADQUISICION							*
//********************************************************************
void LeerMuestras(char *strComponente, long num_muestras, float &volts, float &current)    // la data se va filtrando.
{
	delay(10);
	
	int	Toff_set = 5;
	
	//Serial.print("\n\t\t\t** ");	Serial.print(strComponente);	Serial.print(" ** - ");	Serial.println(millis());
	ads1252.setGanancia(0);
	
	int		n_fin = ApIAB + Toff_set;                                   // 3 muestras.
	while(ApIAB < n_fin) ads1252.available();                           // esperar que termine la adquisicion de "num_muestras"
	
	float nivelmn = (Vmn[ApIAB-1] + Vmn[ApIAB-2] + Vmn[ApIAB-3])/3;
	ads1252.setGanancia(0);
	
	n_fin = ApIAB + num_muestras - Toff_set;
	while(ApIAB < n_fin) ads1252.available();                           // esperar que termine la adquisicion de "num_muestras"
	ads1252.setGanancia(0);
                                                                        // se debe poner un condicional que no permita el bloqueo.
	int from = ApIAB-num_muestras;
	
	int i;
	volts =0;
	current =0;
	for(i= 15; i<num_muestras; i++)
	{
		volts += Vmn[from + i];
		current += IAB[from + i];
	}
	
	volts = volts/(i-15);
	current = current/(i-15);
}

// ----------------------------------------------------------------------------------------------------
void Adquirir(byte *progress_Bar)
{
	float	I0, SP1, SP2, SP3;
	float	dV1, dV2;
	float	V1, V2;
	float	I1, I2;
	
	// Iniciar la adquisicion de datos.
	__fuenteAB._println("{entity:\"power_ON\"}");			// *****
	digitalWrite(__fuenteAB.pin_enable_source, HIGH);

	delay(1500);
	ads1252.reset_adc();
	
	int i;
	// -------------------------
	LeerMuestras("SP1", config.t_OFF, SP1, I0);				// Leer 1000 muestras en tiempo de SP1
	adq.SP1 = SP1;
	*progress_Bar=1;
	
	// verificar nivel de SP.
	// -------------------------
	__fuenteAB.ON();
	LeerMuestras("V1", config.t_ON, V1, I1);
	I1 = I1 - I0;
	adq.V1 = V1;
	adq.I1 = I1;
	*progress_Bar=2;
	__fuenteAB.OFF();
	
	// -------------------------
	LeerMuestras("SP2", config.t_OFF/2, SP2, I0);			// SP
	
	__fuenteAB.NEG();
	LeerMuestras("SP2", config.t_OFF/2, SP2, I0);			// SP
	adq.SP2 = SP2;
	*progress_Bar=3;
	
	// -------------------------
	__fuenteAB.ON();
	LeerMuestras("V2", config.t_ON, V2, I2);
	I2 = I2 - I0;
	adq.V2 = V2;
	adq.I2 = I2;
	dV1 = V1-SP1;
	dV2 = V2-SP2;
	
	*progress_Bar=4;
	__fuenteAB.OFF();
	
	// -------------------------
	LeerMuestras("SP3", config.t_OFF/2, SP3, I0);			// SP
	ads1252.stop();
	
	__fuenteAB._println("{entity:\"power_OFF\"}");			// *****
	digitalWrite(__fuenteAB.pin_enable_source, LOW);
	__fuenteAB.POS();
	
	// --------------------------------------------------
	Serial.println(F("\n\nCORE 2 **************************************************\n	*"));
	Serial.print(F("	*	SP:		"));	Serial.print(SP1);	Serial.print(F("			"));	Serial.println(SP2);
	Serial.print(F("	*	dV:		"));	Serial.print(dV1);	Serial.print(F("			"));	Serial.println(dV2);
	Serial.print(F("	*	 I:		"));	Serial.print(I1);	Serial.print(F("			"));	Serial.println(I2);
	Serial.println(F("	*\nCORE 2 **************************************************\n"));
	Serial.flush();
	*progress_Bar=5;
}
