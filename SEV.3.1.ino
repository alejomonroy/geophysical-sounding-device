// -------------------------
typedef struct 
{
	int   t_ON = 1000;
	int   t_OFF = 1000;
} CONFIGURACION;
CONFIGURACION config;

// -------------------------
#include "LCDNextion.h"
#include "SEV.3.0.h"
#include "Consola.h"

/****************************************************************************
*									nucleo 1								*
*****************************************************************************/
bool finSetup1 = false;

// control NEXTION y BT (APP android o ios) + FileSystem.
void setup()
{
	Serial.begin(115200);
	while (!Serial);

	long tSetup = millis();
	
	// --------------------------------------------------
	nextion_begin();
	init_FileSystem();
	
	// inicializa BT.
	// controla comunicacion serial. recibir comandos por puerto serial.
	
	void *ptr;
	Pag_Temperturas(ptr);
	Serial.print("Fin Setup CORE 1 - ");	Serial.println(millis()-tSetup);
	finSetup1 = true;
}

void loop()
{
	nexLoop(nex_listen_list);
	recvSerial();
}

/****************************************************************************
*									nucleo 2								*
*****************************************************************************/
// Adquisicion de data de ADS1252.
void setup1()
{
	delay(2000);
	while(finSetup1 == false)
	{
		delay(10);
	}
	
	long tSetup = millis();
	
	init_wire1();
	gps_neo.begin();		// inicializa GPS NEO.
	
	ads1252.setCLK(ADS_CLK);
	ads1252.setADS0(ADS0_SCK, ADS0_MISO);
	ads1252.setADS1(ADS1_SCK, ADS1_MISO);
	ads1252.begin(1000);
//	testElectrodos.init_PE();
	
	niveles.begin();		// inicializa ADS1115
	__fuenteAB.begin(AB_POWER, AB_POL, AB_ENABLE);
	
	niveles.tOut = millis() + 2000;
	gps_neo.tOut = millis() + 1000;
	pinMode(LED_BUILTIN, OUTPUT);
}

// ----------------------------------------------------------------------------------------------------
void loop1()
{
	// get page in Nextion.
	// --------------------------------------------------
	if(NEXTION_PAGE == PGRAFICA)
	{
		if( ProgressBar==0 )	// este bloque de codigo trabaja hasta terminar la adquisicion.
		{
			// Adquirir datos desde el CORE2.
			Serial.println("INICIA ADQUISICION DE DATOS...");
			digitalWrite(LED_BUILTIN, HIGH);
			
			Nex_page(PGRAFICA);
			wait_for(600);
			actualizar_Ra();

			Adquirir(&ProgressBar);
		}
	}
	
	// --------------------------------------------------
	if(millis() > gps_neo.tOut)		// cada segundo.
	{
		int n = Serial.availableForWrite();
		Serial.print("n: ");    Serial.println(n);
		gps_neo.tOut = millis() + 1000;
		
		if(NEXTION_PAGE != PABRIR_SEV) gps_neo.verHora();
		if(NEXTION_PAGE == PTEMPER)    P1_tHora.setText(gps_neo.hora);
		if(NEXTION_PAGE == PCONF)      P8_tHora.setText(gps_neo.hora);
		
		// BLINKING...
		if(digitalRead(LED_BUILTIN))    digitalWrite(LED_BUILTIN, LOW);
		else                            digitalWrite(LED_BUILTIN, HIGH);
	}
	
	gps_neo.leerGPS();
	
	if(millis() > niveles.tOut)
	{
		niveles.tOut = millis() + 2000;
		proc_PTEMPER();
	}
}
