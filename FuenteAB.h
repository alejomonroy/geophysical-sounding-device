#ifndef __fuenteAB__
#define __fuenteAB__

#define fuenteSerial Serial1

class _fuenteAB
{
private:
  pin_size_t pin_corriente;
	pin_size_t pin_polaridad;
public:
  pin_size_t pin_enable_source;

	_fuenteAB(){};
	
  void  _println(const char *str);
  void  begin(pin_size_t corriente, pin_size_t polaridad, pin_size_t enable_source);
	
	void	ON();
	void	OFF();
	
	void	NEG();		// polaridad positiva.
	void	POS();		// polaridad negativa.
};

// -------------------------
void	_fuenteAB::begin(pin_size_t corriente, pin_size_t polaridad, pin_size_t enable_source)
{
	pin_corriente = corriente;
	pin_polaridad = polaridad;
  pin_enable_source = enable_source;
	
	pinMode(corriente, OUTPUT);
  pinMode(polaridad, OUTPUT);
  pinMode(enable_source, OUTPUT);
	
	OFF();
	POS();
	
	fuenteSerial.setTX(0);
	fuenteSerial.setRX(1);
	fuenteSerial.begin(19200);
	pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  digitalWrite(pin_enable_source, LOW);
	
  _println("{entity:\"Set\", Vout:150, Iout:1510}");
}

// -------------------------
void   _fuenteAB::_println(const char *str)
{
	digitalWrite(2, HIGH);
	delay(1);
	Serial.print("> ");	Serial.println(str);
	fuenteSerial.println(str);
	fuenteSerial.flush();
	delay(1);
	digitalWrite(2, LOW);
	
	long tout = millis() + 100;
	
	while(millis() < tout)
	{
		if(fuenteSerial.available())
		{
			tout = millis() + 100;
			
			char c = (char)fuenteSerial.read();
			Serial.print(c);	Serial.print("");
		}
	}
}

// -------------------------
void	_fuenteAB::ON()
{
	digitalWrite(pin_corriente, LOW);				// APICACION DE CORRIENTE.
}

// -------------------------
void	_fuenteAB::OFF()
{
	digitalWrite(pin_corriente, HIGH);				// RETIRA DE CORRIENTE.
}

// -------------------------
void	_fuenteAB::NEG()
{
	digitalWrite(pin_polaridad, LOW);				// CAMBIO DE POLARIDAD.
}

// -------------------------
void	_fuenteAB::POS()
{
	digitalWrite(pin_polaridad, HIGH);				// CAMBIO DE POLARIDAD.
}

_fuenteAB __fuenteAB;

#endif /* #ifndef __fuenteAB__ */
