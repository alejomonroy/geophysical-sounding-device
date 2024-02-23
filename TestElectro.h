
#include <Adafruit_ADS1X15.h>
#include <Adafruit_MCP23X17.h>

#define		E_PE           28		// Enable prueba de electrodos.
#define		PCF8574        0x38 
#define		ADS1115_ADD2   0x49

Adafruit_ADS1115 ADS_PE;    // CONFLICTO DE DIRECCIONES DE ADS1115

class TestElectrodos
{
private:
	Adafruit_MCP23X17 mcp0;		// device.
	
	void aplicarCorriente(int Salida);
	void ajusteOffset();
	
	float ch1, ch2, ch3, ch4;		// Lectura de cada canal.
	float _V1, _V2, _V3, _V4;		// Nivel medio, offset.
	float RA, RB, Rm, Rn;		// Ra.
	
public:
	int  init_PE();
	int  probarElectrodos();
};

// --------------------------------------------------
int	TestElectrodos::init_PE()
{
	Serial.println("Inicializa ADS1115 prueba de electrodos.");
	
	digitalWrite(E_PE, LOW);
	pinMode(E_PE, OUTPUT);
	
	//ADS_PE.setGain(GAIN_ONE);
	//ADS_PE.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  0.0625mV
	//ADS_PE.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  0.03125mV
	//ADS_PE.setDataRate(7);    // slow
	
	if (!ADS_PE.begin(ADS1115_ADD2, &Wire1))
	{
		Serial.println("Failed to initialize ADS_PE.");
		return -1;
	}
	
	Serial.println("Inicializa mcp prueba de electrodos.");
	mcp0.begin_I2C(0x20, &Wire1);
	
	for(int i=0; i<16; i++)
		mcp0.pinMode(i, OUTPUT);
	
	mcp0.writeGPIO(0x00);
	mcp0.writeGPIO(0x00, 1);
	
	return 1;
}

// --------------------------------------------------
int		TestElectrodos::probarElectrodos()
{
	Serial.println("-------------------- Probar electrodos --------------------");
	
	_V1=0; _V2=0; _V3=0; _V4=0;
	
	// --------------------------------------------------
	aplicarCorriente(0x04);
	_V3 = ch3;
	_V4 = ch4;
	
	aplicarCorriente(0x80);
	_V1 = ch1;
	_V2 = ch2;
	
	// --------------------------------------------------
	// Prueba los electrodos.
	Serial.println("----------------------------------");
	int _testElectrodos = 0;
	
	aplicarCorriente(0x48); ajusteOffset();		// A+		M+	N-		B-
	if(RA<18000) _testElectrodos |= 0x80;
	if(RB<18000) _testElectrodos |= 0x40;
	if(Rm<18000) _testElectrodos |= 0x20;
	if(Rn<18000) _testElectrodos |= 0x10;
	
	aplicarCorriente(0x44); ajusteOffset();		// A+		M-	N+		B-
	if(RA<18000) _testElectrodos |= 0x08;
	if(RB<18000) _testElectrodos |= 0x04;
	if(Rm<18000) _testElectrodos |= 0x02;
	if(Rn<18000) _testElectrodos |= 0x01;
	
	Serial.print("test Electrodos: ");	Serial.println(_testElectrodos, BIN);
	
	mcp0.writeGPIO(0x00);
	mcp0.writeGPIO(0x00, 1);
	
	return _testElectrodos;
}

// --------------------------------------------------
void	TestElectrodos::aplicarCorriente(int Salida)
{
	float V1, V2, V3, V4;
	
	Serial.print("PIN: ");	Serial.println(Salida, HEX);
	Serial.print("Salida: "); Serial.println(Salida, HEX);
	mcp0.writeGPIO(Salida);
	
	//Serial.println("V1\tV2\tV3\tV4");
	
	int i=0;
	ch1=0; ch2=0; ch3=0; ch4=0;
	for (unsigned long start = millis(); millis() - start < 100;)
	{
		V1= ADS_PE.readADC_SingleEnded(0);	V1= (1000*ADS_PE.computeVolts(V1)) - _V1;
		V2= ADS_PE.readADC_SingleEnded(1);	V2= (1000*ADS_PE.computeVolts(V2)) - _V2;
		V3= ADS_PE.readADC_SingleEnded(2);	V3= (1000*ADS_PE.computeVolts(V3)) - _V3;
		V4= ADS_PE.readADC_SingleEnded(3);	V4= (1000*ADS_PE.computeVolts(V4)) - _V4;
		
		Serial.print("");	  Serial.print(V1, 4);
		Serial.print("\t");	Serial.print(V2, 4);
		Serial.print("\t");	Serial.print(V3, 4);
		Serial.print("\t");	Serial.println(V4, 4);	//*/
		
		ch1+=V1;	ch2+=V2;	ch3+=V3;	ch4+=V4;	i++;
	}	ch1=ch1/i;	ch2=ch2/i;	ch3=ch3/i;	ch4=ch4/i;
	
	//Serial.print("i: ");  Serial.println(i);
	/*Serial.print("");	Serial.print(ch1, 4);
	Serial.print("\t");	Serial.print(ch2, 4);
	Serial.print("\t");	Serial.print(ch3, 4);
	Serial.print("\t");	Serial.println(ch4, 4);	//*/
}

// --------------------------------------------------
void	TestElectrodos::ajusteOffset()
{
	// AB
	if(ch2>0) ch2= ch2-21;
	else      ch2= ch2+15;
	if(ch1>0) ch1= ch1-9;
	else      ch1= ch1+6;
	
	// mn
	if(ch4>0) ch4= ch4-21;
	else      ch4= ch4+15;
	if(ch3>0) ch3= ch3-23.5;
	else      ch3= ch3+17;
	
	RA = (968000/abs(ch1)) -440;
	RB = (968000/abs(ch2)) -440;
	Rm = (968000/abs(ch4)) -440;
	Rn = (968000/abs(ch3)) -440;
	
	Serial.print("***** RA: ");  Serial.print(RA, 4);  Serial.print(" | RB: ");  Serial.print(RB, 4);
	Serial.print("***** Rm: ");  Serial.print(Rm, 4);  Serial.print(" | Rn: ");  Serial.println(Rn, 4);
}

TestElectrodos testElectrodos;
