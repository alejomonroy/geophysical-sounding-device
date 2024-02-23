#include <Adafruit_ADS1X15.h>

#define    ADS1115_ADD1 (0x48)

class	Niveles
{
private:
	float	voltajes[200][4];
	int		ap;
	Adafruit_ADS1115 ADS_1115;
	
	bool  inicializado=false;
public:
	long	tOut;
	
	float TempCPU;
	float TempFuente;
	float TempInterna;
	
	Niveles(){};
	
	void	begin();
	void	medirFuentes();
	void	medirTemp();
	
	void	getData(float *V1, float *V2, float *V3, float *V4, int n);
	int		getAp() { return ap; }
};

void	Niveles::begin()
{
	ap=0;
	
	bzero(&voltajes, sizeof(voltajes));
	inicializado = ADS_1115.begin(ADS1115_ADD1, &Wire1);
	if (inicializado)   Serial.println("ADS_Niveles inicializado.");
	else                Serial.println("Failed to initialize ADS_Niveles.");
}

void	Niveles::medirFuentes()
{
	/*Serial.print("niveles.ap  : "); Serial.println(ap);
	Serial.print("inicializado: "); Serial.println(inicializado); //*/
	if( inicializado == false ) return;
	
	int16_t adc0, adc1, adc2, adc3;
	
	adc0 = ADS_1115.readADC_SingleEnded(0);
	adc1 = ADS_1115.readADC_SingleEnded(1);
	adc2 = ADS_1115.readADC_SingleEnded(2);
	adc3 = ADS_1115.readADC_SingleEnded(3);
	
	voltajes[ap][0]	= 0.03 + (2.5 + ADS_1115.computeVolts(adc0)/10)/0.833333333333;
	voltajes[ap][1]	= 0.03 + ADS_1115.computeVolts(adc1);
	voltajes[ap][2] = 0.03 + (2.5 + ADS_1115.computeVolts(adc2)/10)/0.54022989;
	voltajes[ap][3]	= 0.01 + (2.524 + ADS_1115.computeVolts(adc3)/5.1)*3.938;
	
	//Serial.print("Volt CPU: ");	Serial.println(voltajes[ap][0]);
	//Serial.print("Volt RTC: ");	Serial.println(voltajes[ap][1]);
	//Serial.print("Volt 5V :");	Serial.println(voltajes[ap][2]);
	//Serial.print("Volt BAT:");	Serial.println(voltajes[ap][3]);  //*/
	
	ap++;
	if(ap>=200) ap=0;
}

void	Niveles::medirTemp()
{
	TempCPU = analogReadTemp() + 2;
//	TempFuente = ;
	TempInterna = analogReadTemp();
	
	/*Serial.printf("Core temperature: %2.1fC\n", TempCPU);
	Serial.printf("Source temperature: %2.1fC\n", TempCPU);
	Serial.printf("Internal temperature: %2.1fC\n", TempCPU); //*/
}

void	Niveles::getData(float *V1, float *V2, float *V3, float *V4, int n)
{
	if(n >= 200) return;
	
	*V1 = voltajes[n][0];
	*V2 = voltajes[n][1];
	*V3 = voltajes[n][2];
	*V4 = voltajes[n][3];
}

Niveles niveles;
//*/
