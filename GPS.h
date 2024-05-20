/*
*		set_reloj() - toma el dato q se lee desde el reloj DS3231 y actualiza el RELOJ DEL SISTEMA.
*		setDS3231() - toma el  dato desde el sistema y lo configura en el DS3231 RTC.
*		leerGPS()	- leer la informacion desde el GPS, si hay informacion valida, se monta la hora
*						en el reloj del sistema y en el DS3231.
*/
#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <DS3231.h>

#include <time.h>
#include <sys/time.h>

TinyGPS gps;
SoftwareSerial ss(26, 27);

// --------------------------------------------------
static void print_float(float val, float invalid, int len, int prec)
{
	if (val == invalid)
	{
		while (len-- > 1)	Serial.print('*');
		Serial.print(' ');
	}else
	{
		Serial.print(val, prec);
		int vi = abs((int)val);
		int flen = prec + (val < 0.0 ? 2 : 1); // . and -
		flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
		for (int i=flen; i<len; ++i) Serial.print(' ');
	}
}

// --------------------------------------------------
static void print_int(unsigned long val, unsigned long invalid, int len)
{
	char sz[32];
	if (val == invalid)	strcpy(sz, "*******");
	else				sprintf(sz, "%ld", val);
	
	sz[len] = 0;
	for (int i=strlen(sz); i<len; ++i)	sz[i] = ' ';
	
	if (len > 0) sz[len-1] = ' ';
	Serial.print(sz);
}

// --------------------------------------------------
static void print_date(TinyGPS &gps)
{
	int year;
	byte month, day, hour, minute, second, hundredths;
	unsigned long age;
	gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
	if (age == TinyGPS::GPS_INVALID_AGE)	Serial.print("********** ******** ");
	else
	{
		char sz[32];
		sprintf(sz, "%02d/%02d/%02d %02d:%02d:%02d ", month, day, year, hour, minute, second);
		Serial.print(sz);
	}
	print_int(age, TinyGPS::GPS_INVALID_AGE, 5);
}

// --------------------------------------------------
class GPS_NEO
{
private:
	// leer la hora y actualizar el reloj del uC
	// y el reloj DS3231.
	DS3231	Clock;
public:
	long	tOut;
	bool	RTC_actualizado=false;
	
	char	coordenadas[25]="";	// -45.123456, -74.123456
	char	hora[25]="";	    // 12:00 PM
	float	latitud=0;
	float	longitud=0;
	int		hdop=0;

	void	set_reloj();
	void	setDS3231();
	void	begin();
	int		leerGPS();
	void	verHora();
};

// --------------------------------------------------
struct tm	*timezone(byte year, byte month, byte date, byte hour, byte minute, byte second, int Timezone)
{
    struct tm tmstruct;
    tmstruct.tm_year = year + 2000 - 1900;
	tmstruct.tm_mon = month - 1;
	tmstruct.tm_mday = date;
	tmstruct.tm_hour = hour;
	tmstruct.tm_min = minute;
	tmstruct.tm_sec = second;
	
	time_t tt= mktime(&tmstruct) + Timezone*3600;
	
	return localtime(&tt);
}

// --------------------------------------------------
void GPS_NEO::set_reloj()	// DS3231 -> RP2040
{
//	*
//	* Leer la fecha y hora desde el RTC DS3231 y 
//	* montar este dato en el reloj del RP2040 la 
//	* hora de los archivos generados se sincronza
//	* con esta hora.
//	*
	Serial.println("-----------------------------------------------------------");
	byte year, month, date, DoW, hour, minute, second;
	Clock.getTime(year, month, date, DoW, hour, minute, second);
	
  //{
    year=0;
    month=0;
    date=0;
    hour=0;
    minute=0;
    second=0;
  //}

	char buff[60];
	sprintf(buff, "%02d/%02d/%02d %02d:%02d:%02d", month, date, year, hour, minute, second);
	Serial.println(buff);
	Serial.println("-----------------------------------------------------------");
	
	// ----------------------
    struct tm tmstruct;
    tmstruct.tm_year = year + 2000 - 1900;
	tmstruct.tm_mon = month - 1;
	tmstruct.tm_mday = date;
	tmstruct.tm_hour = hour;
	tmstruct.tm_min = minute;
	tmstruct.tm_sec = second;
	
	time_t tt= mktime(&tmstruct);
	struct timeval tv;
	tv.tv_sec = tt;
	tv.tv_usec = 0;
	
	settimeofday(&tv, nullptr);
	// ----------------------
}

void GPS_NEO::setDS3231()
{
	time_t now;     // 
	char buff[80];
	
	time(&now);
	struct tm *tm_now;
	tm_now = localtime(&now);
	
	// --------------------------------------------------
	// SET DS3231.
	Clock.setClockMode(false);  // set to 24h
	
	Clock.setYear((tm_now->tm_year) + 1900 - 2000);
	Clock.setMonth((tm_now->tm_mon) + 1);
	Clock.setDate(tm_now->tm_mday);
	Clock.setHour(tm_now->tm_hour);
	Clock.setMinute(tm_now->tm_min);
	Clock.setSecond(tm_now->tm_sec);
}

//* ************************************************************************************************
// *                                       	GPS                                       	 		  *
// **************************************************************************************************
void GPS_NEO::begin()
{
	ss.begin(9650);
	set_reloj();
}

int GPS_NEO::leerGPS()
{
	bool newData = false;
	int ret=0;
	
	while(ss.available())
	{
		char c = ss.read();
		
		if (gps.encode(c))
		{
			newData = true;
			break;
		}
	}
	
	if(newData)
	{
		// -------------------- SAT --------------------
		print_int(gps.satellites(), TinyGPS::GPS_INVALID_SATELLITES, 5);
		
		// -------------------- hdop --------------------
		hdop = gps.hdop();
		print_int(gps.hdop(), TinyGPS::GPS_INVALID_HDOP, 5);
		
		if(hdop == TinyGPS::GPS_INVALID_HDOP)
		{
			hdop = 0;
			ret=-3;
		}
		
		// -------------------- geoposicion. --------------------
		unsigned long age;
		float latitud, longitud;
		gps.f_get_position(&latitud, &longitud, &age);
		
		print_float(latitud, TinyGPS::GPS_INVALID_F_ANGLE, 10, 6);
		print_float(longitud, TinyGPS::GPS_INVALID_F_ANGLE, 11, 6);
		print_int(age, TinyGPS::GPS_INVALID_AGE, 5);
		ret=1;
		
		if((latitud == TinyGPS::GPS_INVALID_F_ANGLE)||(longitud == TinyGPS::GPS_INVALID_F_ANGLE))
		{
			latitud = 0;
			longitud = 0;
			ret=-1;
		}
		
		sprintf(coordenadas, "%.6f, %.6f", latitud, longitud);
		
		// --------------- fecha y hora satelital. ----------
		int year;
		byte month, day, hour, minute, second, hundredths;
		
		print_date(gps);
		gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);	year-=2000;
		Serial.println();
		
		if(age == TinyGPS::GPS_INVALID_AGE)
		{
			year=0;	month=1;	day=1;
			hour=0;		minute=0;	second=0;
			ret=-2;
		} else
		{
			if(RTC_actualizado) return ret;
			
			// Actualiza la hora en el DS3231.
			struct tm *tm_now;
			tm_now = timezone(year, month, day, hour, minute, second, -5);
			
			Clock.setClockMode(false);  // set to 24h
			Clock.setYear((tm_now->tm_year) + 1900 - 2000);
			Clock.setMonth((tm_now->tm_mon) + 1);
			Clock.setDate(tm_now->tm_mday);
			Clock.setHour(tm_now->tm_hour);
			Clock.setMinute(tm_now->tm_min);
			Clock.setSecond(tm_now->tm_sec);
			
			// Actualiza la hora en el reloj del SISTEMA.
			set_reloj();
			RTC_actualizado=true;
		}
		
		return ret;
	}
	
	return 0;
}

void GPS_NEO::verHora()
{
	time_t now;
	
	time(&now);
	strftime(hora, sizeof(hora), "%H:%M:%S %b-%d", localtime(&now));
	//strftime(hora, sizeof(hora), " - %H:%M:%S", localtime(&now));
	Serial.println(hora);
}

GPS_NEO	gps_neo;
