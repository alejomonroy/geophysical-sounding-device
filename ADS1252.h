
#ifndef __ADS1252__
#define __ADS1252__

#include <Adafruit_MCP23X17.h>
#include <Vector.h>

// SCLK maximo 16Mhz
std::vector<float> Vmn;
std::vector<float> IAB;
int ApVmn;
int ApIAB;

int		ABMAX_Sev;					// Es la posicion del AB/2 maximo.
double	logRa[30];					// 4*30 = 120

typedef struct		// 4 * 6= 24
{
	double V1;
	double V2;
	double I1;
	double I2;
	double SP1;
	double SP2;
} Adq;
Adq		adq;

// ----------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
class ADS1252
{
private:
	Adafruit_MCP23X17 mcp0;   // device.
  
	pin_size_t	_CLK_PIN;
	
	pin_size_t	_SCLK0_PIN;
	pin_size_t	_MISO0_PIN;
	pin_size_t	_SCLK1_PIN;
	pin_size_t	_MISO1_PIN;
	
	unsigned int	_CLK;		// ADS CLK.
	unsigned int	_SPS;		// Samples per second.
	
	float	_ganancia = 0.5;
	int		tamArray = 5000;
	
public:
	ADS1252(){};
	
	byte available();
	int read_data(byte numADS);
	
	void setCLK(pin_size_t clk);
	void setADS0(pin_size_t sclk, pin_size_t miso);
	void setADS1(pin_size_t sclk, pin_size_t miso);
	
	void begin(int sps);
	void reset_adc();
	void setGanancia(int ganancia);
	
	void	stop();
};

// * ************************************************************************************************
// *                                       IMPLEMENTACION                                       	  *
// **************************************************************************************************
void ADS1252::setCLK(pin_size_t clk)
{
	_CLK_PIN = clk;
}

void ADS1252::setADS0(pin_size_t sclk, pin_size_t miso)
{
	_SCLK0_PIN = sclk;
	_MISO0_PIN = miso;
}

void ADS1252::setADS1(pin_size_t sclk, pin_size_t miso)
{
	_SCLK1_PIN = sclk;
	_MISO1_PIN = miso;
}

void ADS1252::setGanancia(int ganancia)
{
//	if((ganancia<0)||(ganancia>3)) return;
  Serial.print("Ganancia: ");  Serial.println(ganancia);
 
	mcp0.writeGPIO(ganancia);
	switch(ganancia)
	{
		case 0:		_ganancia = 0.5047;	break;  // @
		case 1:		_ganancia =  2.1611;	break; // @
		case 2:		_ganancia =  8.0325;	break;
		case 3:		_ganancia = 23.4855;	break;
		default:	_ganancia = 0.5047;
	}
};

// ----------------------------------------------------------------------------------------------------
void ADS1252::begin(int sps)
{
	mcp0.begin_I2C(0x20, &Wire1);
	for(int i=0; i<16; i++)		mcp0.pinMode(i, OUTPUT);
	mcp0.writeGPIO(0x00);
	mcp0.writeGPIO(0x00, 1);
	
	tamArray = 2.5*config.t_OFF + 2*config.t_ON + 500;		// 2.5x1000 + 2x1000 + 500
	
	Vmn.resize(tamArray);
	IAB.resize(tamArray);
	
	_SPS = sps;
	_CLK = 384*_SPS;
	
	pinMode(_SCLK0_PIN, OUTPUT);	pinMode(_MISO0_PIN, INPUT);
	pinMode(_SCLK1_PIN, OUTPUT);	pinMode(_MISO1_PIN, INPUT);
	pinMode(_CLK_PIN, OUTPUT);
	
	analogWriteFreq(_CLK);
	analogWriteRange(32);
	analogWrite(_CLK_PIN, 16);
	
	stop();
}

byte ADS1252::available()
{
	if(digitalRead(_MISO0_PIN)==1)
	{
		while(digitalRead(_MISO0_PIN)==1);
		
		// reconocer DRDY.
		unsigned int tDRDY = (36+2) * 1000000/_CLK;     // en microsegundos.
		delayMicroseconds(tDRDY);
		
		Vmn[ApVmn] = (2500*((float)read_data(0))/0x800000)/_ganancia;
		Serial.print(Vmn[ApVmn++]);		Serial.print(", ");
		if(ApVmn>=tamArray) ApVmn = 0;
		
		IAB[ApIAB] = 2538*((float)read_data(1))/0x800000;
		Serial.print(IAB[ApIAB++]);   Serial.println();    //Serial.println(millis());
		if(ApIAB>=tamArray) ApIAB = 0;
		
		return 1;
	}
	
	/*if(digitalRead(_MISO1_PIN)==1)
	{
		IAB[ApIAB] = 5000*((float)read_data(1))/0x800000;
		//Serial.print(ApIAB);          Serial.print(": ");
		Serial.print(IAB[ApIAB++]);   Serial.println();    //Serial.println(millis());
		if(ApIAB>=tamArray) ApIAB = 0;
		
		return 2;
	}	//*/
	return 0;
}

void ADS1252::reset_adc()
{
	ApVmn=0;
	ApIAB=0;
	
	unsigned int tCICLO = 1000000/_SPS;
	unsigned int tRESET = 6*tCICLO;		// 5 periodos de conversion expresado en microsegundos.
	
	digitalWrite(_SCLK0_PIN, HIGH);		digitalWrite(_SCLK1_PIN, HIGH);
	delayMicroseconds(tRESET);
	
	digitalWrite(_SCLK0_PIN, LOW);
	//delayMicroseconds(tCICLO/2);
	digitalWrite(_SCLK1_PIN, LOW);
	delayMicroseconds((21*tCICLO)/2);   // se debe verificaro porke se requiere esto.
	
  // para verificarlo se debe correr la funcion continuamente y ver a travez de osciloscopio.
}


//  Serial.print(millis());  Serial.println(": up 2");

int ADS1252::read_data(byte numADS)
{
	int data = 0;
	pin_size_t sclk;
	pin_size_t miso;
	
	if(numADS==0)
	{
		sclk = _SCLK0_PIN;
		miso = _MISO0_PIN;
	}else
	{
		sclk = _SCLK1_PIN;
		miso = _MISO1_PIN;
	}
	
	for(int i=0; i<24; i++)
	{
		delayMicroseconds(1);			// t6 > 30ns.
		digitalWrite(sclk, HIGH);		// La lectura se hace en el flanco de subida.
		
		int bit = digitalRead(miso);
		data |= bit<<(23-i);
		
		delayMicroseconds(1);
		digitalWrite(sclk, LOW);		// El cambio del dato se realiza en el flanco de bajada.
	}
	
	if(data&0x800000) data |= 0xff000000;
	return data;
}

void ADS1252::stop()
{
	digitalWrite(_SCLK0_PIN, HIGH);
	digitalWrite(_SCLK1_PIN, HIGH);
	
	Serial.print("ApVmn: ");	Serial.println(ApVmn);
	Serial.print("ApIAB: ");	Serial.println(ApIAB);
}

#endif /* #ifndef __ADS1252__ */
