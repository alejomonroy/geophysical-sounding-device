
#define ALPHA	0.6

typedef struct    // 8 Bytes.
{
	double  b;
	double  m;
} Datos;

Datos datosV, datosI;

double	SumX1, SumY1, SumXX1, SumXY1;
double	SumX2, SumY2, SumXX2, SumXY2;
long		sensorAp;

double VCC_medios;

// -------------------------
void regresion1(double Vmn, Datos *datos)
{
	double Divisor;
	
	// -------------------------
	SumX1 += sensorAp;					// datos eje X.
	SumY1 += Vmn;				// datos eje Y.
	SumXX1 += sensorAp*sensorAp;		// datos eje X.
	SumXY1 += sensorAp*Vmn;		// datos eje X y Y.
	
	if(sensorAp==1) return;
	
	Divisor = (sensorAp*SumXX1) - (SumX1*SumX1);
	(*datos).b = (SumXX1*SumY1 - SumX1*SumXY1)/Divisor;
	(*datos).m = (sensorAp*SumXY1 - SumX1*SumY1)/Divisor;
}

// -------------------------
void regresion2(double IAB, Datos *datos)
{
	double Divisor;
	
	// -------------------------
	SumX2 +=	sensorAp;				// datos eje X.
	SumY2 +=	IAB;					// datos eje Y.
	SumXX2 +=	sensorAp*sensorAp;		// datos eje X.
	SumXY2 +=	sensorAp*IAB;			// datos eje X y Y.
	
	if(sensorAp==1) return;
  
	Divisor	= (sensorAp*SumXX2) - (SumX2*SumX2);
	(*datos).b = (SumXX2*SumY2 - SumX2*SumXY2)/Divisor;
	(*datos).m = (sensorAp*SumXY2 - SumX2*SumY2)/Divisor;
}

// -------------------------
void valorRMS()
{
	//media = media + (((x*x)-media)/n);
	//sqrt(media);
}

// -------------------------
void filtroEMA()
{
//	double EMA=0;
	
//	 = ALPHA*value + (1 - ALPHA)*EMA;

}

// -------------------------
void filtroMEDIA()
{
	
}
