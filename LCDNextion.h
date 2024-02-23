#include <Nextion.h>
#include "TestElectro.h"
#include "ADS1252.h"
#include "FileSystem.h"
#include "Adquisicion.h"
#include "GPS.h"
#include "nivelFuentes.h"

#define		PINTRO			0	// Paginas
#define		PTEMPER			1
#define		PABRIR_PROY		2
#define		PNUEVO_PROY		3
#define		PABRIR_SEV		4
#define		PNUEVO_SEV		5
#define		PGRAFICA		6
#define		PMENSAJE		7
#define		PCONF			8
#define		PPRUEBA_ELEC	9
#define		PREPETIR		10

int	proxAbertura();
int	proxAbertura(byte Set_ApSEV);
void actualizar_Ra();

int NEX_Test_Electrodes();
void  Pag_Config(void *ptr);

// **********************************************************************************************
// *                                       		PAGINAS                                       	  *
// **********************************************************************************************
int		NEXTION_PAGE = PTEMPER;

NexPage 	pIntro			= NexPage(0, 0, "Intro");
NexPage 	pTemper			= NexPage(1, 0, "Temp");
NexPage 	pAbrirProy		= NexPage(2, 0, "Abrir_Proy");
NexPage 	pNuevoProy		= NexPage(3, 0, "Nuevo_Proy");
NexPage 	pAbrirSEV		= NexPage(4, 0, "Abrir_SEV");
NexPage 	pNuevoSEV		= NexPage(5, 0, "Nuevo_SEV");
NexPage 	pGrafica		= NexPage(6, 0, "Grafica");
NexPage 	pMensaje		= NexPage(7, 0, "Mensaje");
NexPage 	pConf			= NexPage(8, 0, "Config");
NexPage 	pPruebaElec		= NexPage(9, 0, "Prueba_Elec");
NexPage 	pRepetir		= NexPage(10, 0, "Repetir");

bool	Nex_page(int page)
{
	NEXTION_PAGE = page;
	
	switch(page)
	{
		case PINTRO: 		return pIntro.show();
		case PTEMPER: 		return pTemper.show();
		case PABRIR_PROY:	return pAbrirProy.show();
		case PNUEVO_PROY:	return pNuevoProy.show();
		case PABRIR_SEV: 	return pAbrirSEV.show();
		case PNUEVO_SEV: 	return pNuevoSEV.show();
		case PGRAFICA: 		return pGrafica.show();
		case PMENSAJE: 		return pMensaje.show();
		case PCONF: 		return pConf.show();
		case PPRUEBA_ELEC:	return pPruebaElec.show();
		case PREPETIR:		return pRepetir.show();
	}
	
	return false;
}

// ************************************************************************************************
// *                                       	COMPONENTES                                       	  *
// ************************************************************************************************
#define		PINTRO			0	// Paginas
#define		PTEMPER			1
#define		PABRIR_PROY		2
#define		PNUEVO_PROY		3
#define		PABRIR_SEV		4
#define		PNUEVO_SEV		5
#define		PGRAFICA		6
#define		PMENSAJE		7
#define		PCONF			8
#define		PPRUEBA_ELEC	9

//	--- INICIO ---
NexText P0_tInicio 		= NexText(0, 2, "t0");		// Text Inicio.

//	--- TEMPERATURA ---
NexWaveform P1_wGraf0	= NexWaveform(1, 1, "Temp.s0");		// Voltaje 12V
NexWaveform P1_wGraf1	= NexWaveform(1, 2, "Temp.s1");		// Voltaje 3.6V
NexWaveform P1_wGraf2	= NexWaveform(1, 3, "Temp.s2");		// Voltaje CPU
NexWaveform P1_wGraf3	= NexWaveform(1, 4, "Temp.s3");		// Voltaje 5V

NexNumber	P1_nTemp1	= NexNumber(1, 5, "Temp.n0");		// Temp. Interna
NexNumber	P1_nTemp2	= NexNumber(1, 6, "Temp.n1");		// Temp. CPU

NexText P1_tCoord 		= NexText(1, 12, "t0");
NexText P1_tHora 		= NexText(1, 13, "t1");
NexText P1_tArchivos	= NexText(1, 14, "t2");

NexText P1_tVolt1		= NexText(1, 15, "Temp.t3");
NexText P1_tVolt2		= NexText(1, 16, "Temp.t4");
NexText P1_tVolt3		= NexText(1, 17, "Temp.t5");
NexText P1_tVolt4		= NexText(1, 18, "Temp.t6");

//	--- ABRIR PROY ---
NexButton P2_bArriba	= NexButton(2, 1, "Abrir_Proy.b4");
NexButton P2_bAbajo		= NexButton(2, 2, "Abrir_Proy.b5");

NexButton P2_bAbrir		= NexButton(2, 4, "Abrir_Proy.b6");
NexButton P2_bNuevo		= NexButton(2, 6, "Abrir_Proy.b7");
NexButton P2_bBorrar	= NexButton(2, 11, "Abrir_Proy.b8");

NexSlider P2_scrollFile = NexSlider(2, 12, "Abrir_Proy.h0");

NexText P2_tListFile	= NexText(2, 3, "Abrir_Proy.t0");
NexText P2_tTitulo		= NexText(2, 5, "Abrir_Proy.t1");
NexText P2_tListTime	= NexText(2, 13, "Abrir_Proy.t2");

NexTouch P2_mLinea0		= NexText(2, 14, "Abrir_Proy.m0");
NexTouch P2_mLinea1		= NexText(2, 15, "Abrir_Proy.m1");
NexTouch P2_mLinea2		= NexText(2, 16, "Abrir_Proy.m2");
NexTouch P2_mLinea3		= NexText(2, 17, "Abrir_Proy.m3");
NexTouch P2_mLinea4		= NexText(2, 18, "Abrir_Proy.m4");
NexTouch P2_mLinea5		= NexText(2, 19, "Abrir_Proy.m5");
NexTouch P2_mLinea6		= NexText(2, 20, "Abrir_Proy.m6");
NexTouch P2_mLinea7		= NexText(2, 21, "Abrir_Proy.m7");
NexTouch P2_mLinea8		= NexText(2, 22, "Abrir_Proy.m8");
NexTouch P2_mLinea9		= NexText(2, 23, "Abrir_Proy.m9");

NexText P2_tMsg			= NexText(2, 24, "Abrir_Proy.t3");

//	--- NUEVO PROY ---
NexButton P3_bNuevo		= NexButton(3, 1, "Nuevo_Proy.b4");

NexText P3_tFecha 		= NexText(3, 2, "Nuevo_Proy.t0");
NexText P3_tHora 		= NexText(3, 3, "Nuevo_Proy.t1");
NexText P3_tOperador 	= NexText(3, 4, "Nuevo_Proy.t2");
NexText P3_tMunicipio	= NexText(3, 9, "Nuevo_Proy.t3");
NexText P3_tNombre		= NexText(3, 10, "Nuevo_Proy.t4");

//	--- ABRIR SEV ---
NexButton P4_bArriba	= NexButton(4, 1, "Abrir_SEV.b4");
NexButton P4_bAbajo		= NexButton(4, 2, "Abrir_SEV.b5");

NexButton P4_bAbrir		= NexButton(4, 9, "Abrir_SEV.b6");
NexButton P4_bNuevo		= NexButton(4, 10, "Abrir_SEV.b7");
NexButton P4_bBorrar	= NexButton(4, 11, "Abrir_SEV.b8");

NexSlider P4_scrollFile = NexSlider(4, 12, "Abrir_SEV.h0");

NexText P4_tListFile 		= NexText(4, 3, "Abrir_SEV.t0");
NexText P4_tTitulo 		= NexText(4, 4, "Abrir_SEV.t1");
NexText P4_tListTime	= NexText(4, 13, "Abrir_SEV.t2");

NexTouch P4_mLinea0		= NexText(4, 14, "Abrir_SEV.m0");
NexTouch P4_mLinea1		= NexText(4, 15, "Abrir_SEV.m1");
NexTouch P4_mLinea2		= NexText(4, 16, "Abrir_SEV.m2");
NexTouch P4_mLinea3		= NexText(4, 17, "Abrir_SEV.m3");
NexTouch P4_mLinea4		= NexText(4, 18, "Abrir_SEV.m4");
NexTouch P4_mLinea5		= NexText(4, 19, "Abrir_SEV.m5");
NexTouch P4_mLinea6		= NexText(4, 20, "Abrir_SEV.m6");
NexTouch P4_mLinea7		= NexText(4, 21, "Abrir_SEV.m7");
NexTouch P4_mLinea8		= NexText(4, 22, "Abrir_SEV.m8");
NexTouch P4_mLinea9		= NexText(4, 23, "Abrir_SEV.m9");

NexText P4_tMsg			= NexText(4, 24, "Abrir_SEV.t3");

//	--- NUEVO SEV ---
NexButton P5_bNuevo		= NexButton(5, 1, "Nuevo_SEV.b4");

NexText P5_tFecha 		= NexText(5, 6, "Nuevo_SEV.t0");
NexText P5_tHora 		= NexText(5, 7, "Nuevo_SEV.t1");
NexText P5_tOperador 	= NexText(5, 8, "Nuevo_SEV.t2");
NexText P5_tCoord		= NexText(5, 9, "Nuevo_SEV.t3");
NexText P5_tNombre		= NexText(5, 10, "Nuevo_SEV.t4");
NexText P5_tAB2			= NexText(5, 11, "Nuevo_SEV.t5");

//	--- GRAFICA ---
NexButton P6_bSiguiente	= NexButton(6, 11, "Grafica.b0");
NexButton P6_bCerrar	= NexButton(6, 14, "Grafica.b1");

NexText P6_tAB2 		= NexText(6, 2, "Grafica.t0");
NexText P6_tmn2 		= NexText(6, 3, "Grafica.t1");
NexText P6_tI1 			= NexText(6, 4, "Grafica.t2");
NexText P6_tI2 			= NexText(6, 5, "Grafica.t3");
NexText P6_tV1 			= NexText(6, 6, "Grafica.t4");
NexText P6_tV2 			= NexText(6, 7, "Grafica.t5");
NexText P6_tSP 			= NexText(6, 8, "Grafica.t6");
NexText P6_tRa 			= NexText(6, 9, "Grafica.t7");
NexText P6_tM 			= NexText(6, 10, "Grafica.t8");
NexText P6_tAB2next 	= NexText(6, 12, "Grafica.t9");
NexText P6_tMN2next 	= NexText(6, 13, "Grafica.t10");

NexText P6_tNombreSEV 	= NexText(6, 15, "Grafica.t11");

NexWaveform P6_wGrafica	= NexWaveform(6, 1, "Grafica.s0");
NexProgressBar P6_jProgressBar = NexProgressBar(6, 16, "Grafica.j0");

NexTouch P6_m0_Repetir		= NexTouch(6, 19, "Grafica.m0");

//	--- MENSAJE ---
NexText P7_tTitulo		= NexText(7, 6, "Mensaje.t2");		// max 30 bytes
NexText P7_tMensaje		= NexText(7, 4, "Mensaje.t1");		// max 350 bytes

NexButton P7_bOk		= NexButton(7, 5, "Mensaje.b0");
NexButton P7_bCerrar	= NexButton(7, 8, "Mensaje.b1");

//	--- CONFIGURACION ---
NexButton P8_bPBElectr	= NexButton(8, 5, "Config.b4");
NexButton P8_bBorrar	= NexButton(8, 6, "Config.b5");
NexButton P8_bGuardar	= NexButton(8, 7, "Config.b6");

//NexText P8_tFecha 		= NexText(8, 1, "Config.t0");
NexText P8_tHora 		= NexText(8, 1, "Config.t0");
NexText P8_tCoord	 	= NexText(8, 2, "Config.t2");
NexText P8_tNumSondeos	= NexText(8, 3, "Config.t3");

//	--- PRUEBA ELECTRODOS ---
NexButton P9_bCerrar	= NexButton(9, 2, "Prueba_Elec.b0");
NexCrop P9_qA			= NexCrop(9, 3, "Prueba_Elec.q0");
NexCrop P9_qm			= NexCrop(9, 4, "Prueba_Elec.q1");
NexCrop P9_qn			= NexCrop(9, 5, "Prueba_Elec.q2");
NexCrop P9_qB			= NexCrop(9, 6, "Prueba_Elec.q3");
NexText P9_tMensaje		= NexText(9, 8, "Prueba_Elec.t1");

//	--- PRUEBA ELECTRODOS ---
NexButton  P10_bCerrar = NexButton(10, 2, "Repetir.b0");
NexButton	P10_bOk		= NexButton(10, 3, "Repetir.b1");
NexText		P10_tRepeticiones = NexText(10, 5, "Repetir.t2");

//	------------ BARRA IZQUIERDA ------------
//	--- TEMPERATURA ---
NexButton P1_Proyectos	= NexButton(1, 8, "Temp.b0");
NexButton P1_SEVs	= NexButton(1, 9, "Temp.b1");
NexButton P1_Temper	= NexButton(1, 10, "Temp.b2");
NexButton P1_Config	= NexButton(1, 11, "Temp.b3");

//	--- ABRIR PROY ---
NexButton P2_Proyectos	= NexButton(2, 7, "Abrir_Proy.b0");
NexButton P2_SEVs	= NexButton(2, 8, "Abrir_Proy.b1");
NexButton P2_Temper	= NexButton(2, 9, "Abrir_Proy.b2");
NexButton P2_Config	= NexButton(2, 10, "Abrir_Proy.b3");

//	--- NUEVO PROY ---
NexButton P3_Proyectos	= NexButton(3, 5, "Nuevo_Proy.b0");
NexButton P3_SEVs	= NexButton(3, 6, "Nuevo_Proy.b1");
NexButton P3_Temper	= NexButton(3, 7, "Nuevo_Proy.b2");
NexButton P3_Config	= NexButton(3, 8, "Nuevo_Proy.b3");

//	--- ABRIR SEV ---
NexButton P4_Proyectos	= NexButton(4, 5, "Abrir_SEV.b0");
NexButton P4_SEVs	= NexButton(4, 6, "Abrir_SEV.b1");
NexButton P4_Temper	= NexButton(4, 7, "Abrir_SEV.b2");
NexButton P4_Config	= NexButton(4, 8, "Abrir_SEV.b3");

//	--- NUEVO SEV ---
NexButton P5_Proyectos	= NexButton(5, 2, "Nuevo_SEV.b0");
NexButton P5_SEVs	= NexButton(5, 3, "Nuevo_SEV.b1");
NexButton P5_Temper	= NexButton(5, 4, "Nuevo_SEV.b2");
NexButton P5_Config	= NexButton(5, 5, "Nuevo_SEV.b3");

//	--- CONFIGURACION ---
NexButton P8_Proyectos	= NexButton(8, 8, "Config.b0");
NexButton P8_SEVs	= NexButton(8, 9, "Config.b1");
NexButton P8_Temper	= NexButton(8, 10, "Config.b2");
NexButton P8_Config	= NexButton(8, 11, "Config.b3");

// **************************************************************************************************
// *                                       IMPLEMENTACION                                       	  *
// **************************************************************************************************
void cargaNivelesVolt()
{
	if(NEXTION_PAGE != PTEMPER) return;
	
	int		Ap = niveles.getAp();
	float	V1, V2, V3, V4;
	
	for(int i=0; i<200; i++)
	{
		niveles.getData(&V3, &V2, &V4, &V1, Ap++);
		if( Ap >= 200 ) Ap=0;
		
		if(V4 != 0)
		{
			P1_wGraf0.addValue(0, 5+100*(V1-10.5)/4);
			P1_wGraf1.addValue(0, 5+100*(V2-3)/0.7);
			P1_wGraf2.addValue(0, 5+100*(V3-3.1)/0.4);
			P1_wGraf3.addValue(0, 5+100*(V4-4.8)/0.4);
		}
	}
}

// ---------------------------------------- MENSAJES ----------------------------------------
int		Nex_msgAdvertencia(char *strMsg)
{
	Serial.println(strMsg);
	pMensaje.show();
	P7_tMensaje.setText(strMsg);
	return 0;
}

// --------------------------------------------------------------------------------
int _Selecc[][2] =
{{ 61, 95 },
{ 95, 129 },
{ 129, 163 },
{ 163, 197 },
{ 197, 231 },
{ 231, 265 },
{ 265, 299 },
{ 299, 333 },
{ 333, 367 },
{ 367, 401 }};

byte	inicio_menu;

// selecciona un archivo cuando se selecciona un punto.
void drawSelecFile(int linea, NexText _NEXTest)
{
	int X1 = 75;
	int Y1 = _Selecc[linea][0];
	int X2 = 715;
	int Y2 = _Selecc[linea][1];
	
	_NEXTest.draw(X1, Y1, X2, Y2, "21979");
	_NEXTest.draw(X1++, Y1++, X2--, Y2--, "21979");
	_NEXTest.draw(X1++, Y1++, X2--, Y2--, "21979");
}

//* ******************************************************************
//*						GESTION DE PROYECTOS						*
//*********************************************************************
void	Act_menu_proy()
{
	listProy();			// leer lista de proyectos disponibles o existentes en memoria.
	
	char	strFile[250]="";
	char	strTime[180]="";
	char	buff[20];
	
	//for(int i=0; i<proy_List.Count(); i++)
	for(int i=0; i<10; i++)
	{
		Serial.print("i: ");		Serial.print(i);		Serial.print(", i+ini: ");	Serial.println((i+inicio_menu));
		if((i+inicio_menu) >= proy_List.Count()) break;
		
		FileItem fileItem;
		fileItem = proy_List[i+inicio_menu];
		sprintf(strFile, "%s%s\\r", strFile, fileItem.nombre);
		
		time_t	horaFile = fileItem.creacion;
		strftime(buff, sizeof(buff), "%Y-%b-%d %H:%M", localtime(&horaFile));		Serial.println(buff);
		sprintf(strTime, "%s%s\\r", strTime, buff);
	}
	
	P2_tListFile.setText(strFile);
	P2_tListTime.setText(strTime);
	
	if(proy_List.Count() > 10)
		P2_scrollFile.setMaxval(proy_List.Count()-10);
	else
	{
		P2_scrollFile.setValue(0);		
		P2_scrollFile.setMaxval(0);
	}
}

// ------ PAGINA ABRIR PROY -------
// -------------------------
void proy_selLinea(int	linea)
{
	Serial.print("Linea seleccionada: ");	Serial.println(linea);
	Serial.print("index lista: ");			Serial.println(linea + inicio_menu);
	
	if( (linea + inicio_menu) >= proy_List.Count() ) return;
	
	P2_tMsg.setText("");
	drawSelecFile(linea, P2_tListFile);
	
	char strFileSel[25];
	bzero(strFileSel, sizeof(strFileSel));
	
	strncpy(strFileSel, proy_List[linea + inicio_menu].nombre, 24);
	P2_tTitulo.setText(strFileSel);
	
	Serial.print("@SELProy>{\"proyecto\": \"");
	Serial.print(strFileSel);
	Serial.println("\"}");
	
//	Serial.println("FIN Linea seleccionada: ");
}

void proy_linea0(void *ptr)	{ proy_selLinea(0); }
void proy_linea1(void *ptr)	{ proy_selLinea(1); }
void proy_linea2(void *ptr)	{ proy_selLinea(2); }
void proy_linea3(void *ptr)	{ proy_selLinea(3); }
void proy_linea4(void *ptr)	{ proy_selLinea(4); }
void proy_linea5(void *ptr)	{ proy_selLinea(5); }
void proy_linea6(void *ptr)	{ proy_selLinea(6); }
void proy_linea7(void *ptr)	{ proy_selLinea(7); }
void proy_linea8(void *ptr)	{ proy_selLinea(8); }
void proy_linea9(void *ptr)	{ proy_selLinea(9); }

// -------------------------
void	menu_up(void *ptr)		// 	&P2_bArriba
{
	Serial.print("Menu... boton subir");
	
	// Baja index.
	if(inicio_menu <= 0)
	{
		inicio_menu = 0;
		return;
	}
	
	inicio_menu--;
	
	// actualizar menu.
	Act_menu_proy();
}

// -------------------------
void	menu_down(void *ptr)		// 	&P2_bAbajo
{
	Serial.print("Menu... boton bajar");
	
	// Aumenta index.
	if(inicio_menu >= proy_List.Count()-5)
	{
		inicio_menu = proy_List.Count()-5;
		return;
	}
	
	inicio_menu++;
	
	// actualizar menu.
	Act_menu_proy();
}

// -------------------------
void  Menu_SEVs(void *ptr);

void	abrir_proy(void *ptr)		// 	&P2_bAbrir,
{
	// Abrir carpeta.
	char	strProy[25];

	bzero(&strProy, sizeof(strProy));
	P2_tTitulo.getText(strProy, 21);
	
	if(abrirProy(strProy)<0)
	{
		P2_tMsg.setText("Seleccionar un//rproyecto...");
		return;
	}
	
	Serial.println("Datos Proyecto.");
	Serial.print("fecha: ");  Serial.println( Proyecto.fecha );
	Serial.print("hora : ");  Serial.println( Proyecto.hora );
	Serial.print("operador : ");  Serial.println( Proyecto.operador );
	Serial.print("municipio: ");  Serial.println( Proyecto.municipio );
	Serial.print("nombre   : ");  Serial.println( Proyecto.nombre );
	
	// pasa a pagina de sondeos.
	Menu_SEVs(ptr);
}

// -------------------------
void	nuevo_proy(void *ptr)		// 	&P2_bNuevo,
{
	Nex_page(PNUEVO_PROY);
	delay(200);
	
	// carga la fecha y hora actual.
	time_t now;
	char buff[15];
	time(&now);
	
	strftime(buff, sizeof(buff), "%Y-%b-%d", localtime(&now));   Serial.println(buff);
	P3_tFecha.setText(buff);
	
	strftime(buff, sizeof(buff), "%H:%M", localtime(&now));   Serial.println(buff);
	P3_tHora.setText(buff);
}

// -------------------------
long	time_borrar=0;

void	push_eliminar_proy(void *ptr)		// 	&P2_bBorrar,
{
	// Preciona borrar carpeta.
	Serial.print("PRESIONA ELIMINAR PROYECTO - ");	Serial.println(millis());
	char	strProy[25];
	
	bzero(&strProy, sizeof(strProy));
	P2_tTitulo.getText(strProy, 21);
	Serial.print("Borrar: ");	Serial.println(strProy);
	
	if(existeProy(strProy))
	{
		time_borrar = millis();
	}else
	{
		Serial.print("No existe el proyecto: - ");	Serial.println(strProy);
	}
}

void	eliminar_proy(void *ptr)		// 	&P2_bBorrar,
{
	// Borrar carpeta.
	Serial.print("ELIMINAR PROYECTO - ");	Serial.println(millis());
	long time_press = millis() - time_borrar;
	
	if((time_press>4000)&&(time_press<6000))
	{
		char	strProy[25];
		
		bzero(&strProy, sizeof(strProy));
		P2_tTitulo.getText(strProy, 21);
		Serial.print("Borrar: ");	Serial.println(strProy);
		
		borrarProy(strProy);
		Act_menu_proy();
		_NumSondeos();
	}
}

// -------------------------
void	scrollbar_proy(void *ptr)		// 	&P2_scrollFile,
{
  uint32_t  scrollValor;
  uint32_t  scrollMAX;
  
  P2_scrollFile.getMaxval(&scrollMAX);
  P2_scrollFile.getValue(&scrollValor);
  inicio_menu = scrollMAX - scrollValor;
  
  Act_menu_proy();
}

// ------ NUEVO PROY -------
// -------------------------
void	crear_proy(void *ptr)		// 	&P3_bNuevo
{
	// Crear carpeta y archivo de proyecto.
	bzero(&Proyecto, sizeof(Proyecto));
	
	P3_tOperador.getText(Proyecto.operador, 20);
	P3_tMunicipio.getText(Proyecto.municipio, 20);
	P3_tNombre.getText(Proyecto.nombre, 20);
	P3_tFecha.getText(Proyecto.fecha, 19);
	P3_tHora.getText(Proyecto.hora, 9);
	
	// pasa pagina de sondeos.
	Serial.println("Datos Proyecto.");
	Serial.print("fecha: ");	Serial.println(	Proyecto.fecha );
	Serial.print("hora : ");	Serial.println(	Proyecto.hora );
	Serial.print("operador : ");	Serial.println(	Proyecto.operador );
	Serial.print("municipio: ");	Serial.println(	Proyecto.municipio );
	Serial.print("nombre   : ");	Serial.println(	Proyecto.nombre );
	
	nuevoProy();
	Nex_page(PABRIR_SEV);
}

// * ******************************************************************
// *							GESTION DE SONDEOS						*
// *********************************************************************
void	Act_menu_SEV()
{
	listSEVs();
	
	char	strFile[270]="";
	char	strTime[200]="";
	char	buff[20];
	
	for(int i=0; i<10; i++)
	{
		if((i+inicio_menu) >= SEV_List.Count()) break;
		
		FileItem fileItem;
		fileItem = SEV_List[i+inicio_menu];
		sprintf(strFile, "%s%s\\r", strFile, fileItem.nombre);
		
		time_t	horaFile = fileItem.creacion;
		strftime(buff, sizeof(buff), "%Y-%b-%d %H:%M", localtime(&horaFile));		Serial.println(buff);
		sprintf(strTime, "%s%s\\r", strTime, buff);
	}
	
	Serial.print("millis: ");  Serial.println(millis());
	P4_tListFile.setText(strFile);
	P4_tListTime.setText(strTime);
	
	if(SEV_List.Count() > 10)
		P4_scrollFile.setMaxval(SEV_List.Count()-10);
	else
	{
		P4_scrollFile.setValue(0);		
		P4_scrollFile.setMaxval(0);		
	}
}

// ------ ABRIR SEV --------
// -------------------------
void SEV_selLinea(int	linea)
{
	Serial.print("Linea seleccionada: "); Serial.println(linea);
	Serial.print("index lista: ");      Serial.println(linea + inicio_menu);
	
	if( (linea + inicio_menu) >= SEV_List.Count() ) return;
	
	P4_tMsg.setText("");
	drawSelecFile(linea, P4_tListFile);
	
	char strFileSel[50];
	bzero(&strFileSel, sizeof(strFileSel));
	
	strncpy(strFileSel, SEV_List[linea + inicio_menu].nombre, 25);		strFileSel[24]=0;
	P4_tTitulo.setText(strFileSel);
	
	Serial.print("@SELSondeo>{\"sondeo\": \"");
	Serial.print(strFileSel);
	Serial.println("\"}");
}

void SEV_linea0(void *ptr)	{ SEV_selLinea(0); }
void SEV_linea1(void *ptr)	{ SEV_selLinea(1); }
void SEV_linea2(void *ptr)	{ SEV_selLinea(2); }
void SEV_linea3(void *ptr)	{ SEV_selLinea(3); }
void SEV_linea4(void *ptr)	{ SEV_selLinea(4); }
void SEV_linea5(void *ptr)	{ SEV_selLinea(5); }
void SEV_linea6(void *ptr)	{ SEV_selLinea(6); }
void SEV_linea7(void *ptr)	{ SEV_selLinea(7); }
void SEV_linea8(void *ptr)	{ SEV_selLinea(8); }
void SEV_linea9(void *ptr)	{ SEV_selLinea(9); }

// -------------------------
void	menu2_up(void *ptr)		//	&P4_bAbrir
{
	Serial.print("Menu... boton subir - ");  Serial.println(millis());
	
	// Baja index.
	if(inicio_menu <= 0)
	{
		inicio_menu = 0;
		return;
	}
	
	inicio_menu--;
	
	// actualizar menu.
	Act_menu_SEV();
}

// -------------------------
void	menu2_down(void *ptr)		//	&P4_bAbajo
{
	Serial.print("Menu... boton bajar - ");  Serial.println(millis());
	
	// Aumenta index.
	if(inicio_menu >= SEV_List.Count()-5)
	{
		Serial.print("inicio_menu      : ");    Serial.println(inicio_menu);
		Serial.print("proy_List.Count  : ");    Serial.println(SEV_List.Count());
		Serial.print("proy_List.Count-5: ");    Serial.println(SEV_List.Count() - 5);
		inicio_menu = SEV_List.Count()-5;
		return;
	}
	
	inicio_menu++;
	
	// actualizar menu.
	Act_menu_SEV();
}

// -------------------------
void	abrir_SEV(void *ptr)		//	&P4_bArriba,
{
	char	strSEV[30];
	
	bzero(&strSEV, sizeof(strSEV));
	P4_tTitulo.getText(strSEV, 40);
	Serial.print("Archivo seleccionado: ");  Serial.println(strSEV);
	
	byte Set_ApSEV = abrirSEV(strSEV);
	if(Set_ApSEV<0)
	{
		P4_tMsg.setText("Seleccionar un//rsondeo...");
		return;
	}
	
	// pasa a pagina de sondeos.
	Nex_page(PGRAFICA);

	delay(200);
	ABMAX_Sev = sev.AB2;
	proxAbertura(Set_ApSEV);
	actualizar_Ra();
}

// -------------------------
void	nuevo_SEV(void *ptr)		//	&P4_bNuevo,
{
	// pasa pagina nueva.
	Nex_page(PNUEVO_SEV);
	delay(100);
	
	// carga la fecha y hora actual.
	time_t now;
	char buff[15];
	time(&now);
	
	gps_neo.leerGPS();
	
	strftime(buff, sizeof(buff), "%Y-%b-%d", localtime(&now));   Serial.println(buff);
	P5_tFecha.setText(buff);
	
	strftime(buff, sizeof(buff), "%H:%M", localtime(&now));   Serial.println(buff);
	P5_tHora.setText(buff);
	
	// -------------------------
	P5_tOperador.setText(Proyecto.operador);
	P5_tCoord.setText(gps_neo.coordenadas);		// debe tener la coordenada N y E en el mismo string.
}

// -------------------------
char  strSEV_delete[25];
int   cont_deleteSEV=0;

void	push_eliminar_SEV(void *ptr)		// 	&P4_bBorrar,
{
	// Presiona borrar SEV.
	Serial.print("PRESIONA ELIMINAR SEV - ");	Serial.println(millis());
	char	strSEV[25];
	
	bzero(&strSEV, sizeof(strSEV));
	P4_tTitulo.getText(strSEV, 21);
	Serial.print("Borrar: ");	Serial.println(strSEV);
	
	if(existeSEV(strSEV))
	{
		time_borrar = millis();
    
    if((strcmp(strSEV_delete, strSEV) == 0))
    {
      cont_deleteSEV++;
      Serial.print("Contador eliminar SEV - ");  Serial.println(cont_deleteSEV);
      
      char  msg[50];
      sprintf(msg, "Borrar %s: %d", strSEV, cont_deleteSEV);
      P4_tMsg.setText(msg);
      
      if(cont_deleteSEV >= 3)
      {
        Serial.print("Borrar: "); Serial.println(strSEV);
        
        NumSondeos--;
        borrarSEV(strSEV);
        Act_menu_SEV();
        cont_deleteSEV=0;
      }
    }else
    {
      cont_deleteSEV=1;
      strcpy(strSEV_delete, strSEV);
    }
	}else
	{
		Serial.print("No existe el proyecto: - ");	Serial.println(strSEV);
	}
}

// -------------------------
void	eliminar_SEV(void *ptr)		// 	&P4_bBorrar,
{
	// Borrar SEV.
	Serial.print("ELIMINAR SEV - ");	Serial.println(millis());
	long time_press = millis() - time_borrar;
	
	if((time_press>4000)&&(time_press<6000))
	{
		char	strSEV[25];
		
		bzero(&strSEV, sizeof(strSEV));
		P4_tTitulo.getText(strSEV, 21);
		Serial.print("Borrar: ");	Serial.println(strSEV);
		
		NumSondeos--;
		borrarSEV(strSEV);
		Act_menu_SEV();
	}
}

// -------------------------
void	scrollbar_SEV(void *ptr)		//	&P4_scrollFile,
{
	uint32_t  scrollValor;
	uint32_t  scrollMAX;
	
	P4_scrollFile.getMaxval(&scrollMAX);
	P4_scrollFile.getValue(&scrollValor);
	inicio_menu = scrollMAX - scrollValor;
	
	Act_menu_SEV();
}

// -------------------------
//	--- NUEVO SEV ---
void	crear_SEV(void *ptr)		//	&P5_bNuevo,
{
	char	strSEV[6];
	bzero(&sev, sizeof(sev));
	bzero(&strSEV, sizeof(strSEV));
	
	P5_tFecha.getText(sev.fecha, 15);
	P5_tHora.getText(sev.hora, 10);
	P5_tCoord.getText(sev.coord, 25);
	P5_tAB2.getText(strSEV, 5);
	P5_tNombre.getText(sev.nombre, 20);
	
	sev.AB2 = atoi(strSEV);
	
	if(nuevoSEV()==1)
	{
		NumSondeos++;
		
		P5_tNombre.setText("");
		P5_tAB2.setText("");
		
		P5_tFecha.setText("");
		P5_tHora.setText("");
		P5_tCoord.setText("");
		P5_tOperador.setText("");
		
		Nex_page(PGRAFICA);
		delay(100);
		
		ABMAX_Sev = sev.AB2;
		proxAbertura(0);
		
		char  Titulo_graf[40];
		sprintf(Titulo_graf, "%s (%d m)", sev.nombre, ABMAX_Sev);
		P6_tNombreSEV.setText(Titulo_graf);
	}
}

// * ******************************************************************
// *							HMI_ADQUISICION							*
// *********************************************************************
// minimo 0 pixeles - maximo 540 pixeles.
unsigned int pixel_H[] = { 0, 59, 93, 117, 151, 176, 194, 194, 229, 253, 272, 287, 287, 311, 330, 346, 364, 364, 389, 408, 423, 423, 447, 466, 481, 481, 495, 506, 525, 540 };

// --------------------------------------------------
// Tablas con las aberturas de AB y mn.
unsigned int dimAB[] = { 1, 2, 3, 4, 6, 8, 10, 10, 15, 20, 25, 30,  30,  40,  50,  60,  75,  75, 100, 125, 150, 150, 200, 250, 300,  300,  350,  400,  500,  600 };
unsigned int dimMN[] = { 2, 2, 2, 2, 2, 2,  2, 20, 20, 20, 20, 20, 100, 100, 100, 100, 100, 250, 250, 250, 250, 500, 500, 500, 500, 1000, 1000, 1000, 1000, 1000 };

byte	ApSEV = 0;
byte	ProgressBar=5;			// Progreso de la adquiscion dependiendo del dato que llega.
byte	repetir=0;				// si esta en 0 continua adquisicion, 1 repite el ultimo dato.

//***************************************************
int	proxAbertura()
{
	float floatTemp;
	char str[50];
	char mnstr[50];
	
	Serial.print("ApSEV: ");  Serial.println(ApSEV);
	Serial.print("AB   : ");  Serial.println(dimAB[ApSEV]);
	Serial.print("mn   : ");  Serial.println(dimMN[ApSEV]);
	sprintf( str, "AB/2= %d.0 m", dimAB[ApSEV] );		P6_tAB2next.setText(str);
	
	floatTemp = ((float)dimMN[ApSEV])/10;
	sprintf( mnstr, "mn/2= %.1f m", floatTemp );			P6_tMN2next.setText(mnstr);

	return 0;
}

int	proxAbertura(byte Set_ApSEV)
{
	if(Set_ApSEV>=30)
	{
		Serial.println("ApSEV mayor a 30...");
		return -1;
	}
	
	ApSEV = Set_ApSEV;
	return proxAbertura();
}

// ---------------------------------------------------------------------------
byte minY_ant=4, maxY_ant=0;

void add_punto();

void actualizar_Ra()
{
	minY_ant=4, maxY_ant=0;		// obliga a actualizar toda la grafica basandose en el cambio de escalas en ejeY.
	add_punto();
}

// -------------------------
void draw_ejes(int nEjes)
{
	int alto = 280/nEjes;
	
	Serial.print("Ejes: ");  Serial.print(nEjes);
	Serial.print(", h: ");   Serial.println(alto);
	Serial.println("*** DIBUJAR EJES ***");
	
	for(int i=1; i<nEjes; i++)
	{
		int y = 26 + i*alto;
		
		P6_wGrafica.line(26,   y, 566,   y, "BLACK");
		P6_wGrafica.line(26, y+1, 566, y+1, "BLACK");
	}
}

// * **************************************************
// *		monta la informacion adquirida
// *		en la pantalla NEXTION
// **************************************************
void add_punto()
{
	Serial.println(F("___add_punto___"));
	if(ApSEV<1) return;
	
	// **************************************************
	// Dibujar lineas de decadas en eje Y
	// Calculo de limites para obtener la AUTOESCALA de la grafica.
	Serial.print("ApSEV: ");	Serial.println(ApSEV);
	
	int	minY= 4;		// 10^0, 1
	int maxY= 0;		// 10^5, 100.000
	for(int i=0; i<ApSEV; i++)
	{
		if(logRa[i] < minY) minY = (int)logRa[i];
		if(maxY < logRa[i]) maxY = ((int)logRa[i])+1;
		
		Serial.print("i  : ");	Serial.print(i);
		Serial.print(",   min: ");	Serial.print(minY);
		Serial.print(",   max: ");	Serial.println(maxY);
	}
	
	double escala = 280/(maxY-minY);
	Serial.print(F("max: ")); Serial.print(maxY);
	Serial.print(F(",   min: ")); Serial.println(minY);
	
	int inicio_graf=0;
	if((minY_ant==minY)&&(maxY_ant==maxY))
	{
		inicio_graf = ApSEV-1;
		if(inicio_graf<0) inicio_graf=0;
	}
	minY_ant = minY;
	maxY_ant = maxY;
	Serial.print("ApSEV: ");  Serial.println(ApSEV);
  
	// -------------------------
	for(int i=inicio_graf; i<ApSEV; i++)
	{
		Serial.print("i= ");   Serial.println(i);
		if(i==0)
		{
			Serial.println("Actualizar toda la grafica Ra");
			
			P6_wGrafica.cle(0);				// borrar pantalla.
			
			char	Titulo_graf[40];
			sprintf(Titulo_graf, "%s (%d m)", sev.nombre, ABMAX_Sev);
			P6_tNombreSEV.setText(Titulo_graf);
			
			draw_ejes(maxY-minY);			// dibujar lineas horizontales.
			
			// solo circulo.
			double y0= 280 - (escala*(logRa[0] - minY)) + 26;
			int h0 = y0;
			Serial.println("--------------------- DIBUJAR LINEAS ---------------------");
			
			continue;
		}
		
		int		x0= pixel_H[i-1] + 26;
		int		x1= pixel_H[i] + 26;
		
		// circulo y linea.
		double y0= 280 - (escala*(logRa[i-1] - minY)) + 26;
		double y1= 280 - (escala*(logRa[i] - minY)) + 26;
		int h0 = y0;
		int h1 = y1;
		
		if((x0<26)||(566<x0)) continue;
		if((h0<26)||(306<h0)) continue;
		if((x1<26)||(566<x1)) continue;
		if((h1<26)||(306<h1)) continue;
		
		P6_wGrafica.line(x0, h0-1, x1, h1-1, "RED");
		P6_wGrafica.line(x0,   h0, x1,   h1, "RED");
		P6_wGrafica.line(x0, h0+1, x1, h1+1, "RED");
		P6_wGrafica.cirs(x0, h0, 6, "WHITE");
		P6_wGrafica.cirs(x0, h0, 3, "BLACK");
		
		if(i==ApSEV-1)
		{
			P6_wGrafica.cirs(x1, h1, 6, "WHITE");
			P6_wGrafica.cirs(x1, h1, 3, "BLACK");
		}
	}
}

// --------------------------------------------------
bool	Nex_setAdquisicion(float AB, float  mn)			// Enviar venta a LCD
{
	Serial.println("Guardar datos adquiridos...");
	
	char 	str[20];
	
	double	I  = (adq.I1 + adq.I2)/2;
	double	SP = (adq.SP1 + adq.SP2)/2;
	double	dV1= (adq.V1 - adq.SP1);
	double	dV2= -(adq.V2 - adq.SP2);
	double	dV = (dV1 + dV2)/2;
	
	double	K  = 3.14159265359 * ( AB*AB - mn*mn )/(2*mn);
	double  Ra = K*dV/I;
	double  e  = 0.01;
	
	Serial.print(F("I: "));		Serial.print(I);	Serial.print(F(", dV: "));	Serial.print(dV);
	Serial.print(F(", K: "));		Serial.print(K);	Serial.print(F(", Ra: "));	Serial.println(Ra);
	
	// MOSTRAR EN PANTALLA NEXTION.
	if(Ra <= 1)
	{
		Nex_msgAdvertencia("Valor de resisitividad aparente\\r        es igual o menor a 1 Ohm.");
		delay(2000);
		Nex_page(PGRAFICA);
		delay(700);
		actualizar_Ra();	// Esperar mientras muestra el mensaje emergente.*/
		
		logRa[ApSEV] = 0;
	} else
	{
		logRa[ApSEV] = log10(Ra);
	}
  sprintf(str, "Ra:%.2f", Ra);  P6_tRa.setText(str);  P6_tM.setText( "" );
	
	// GUARDAR DATOS ADQUIRIDOS EN ARCHIVO.
	guardarDatos( AB, mn, SP, dV1, dV2, Ra, e);
	
	Serial.println("Calcula Log10(Ra)");
	Serial.print("Ra       : ");	Serial.println(Ra);
	Serial.print("Log10(Ra): ");	Serial.println(logRa[ApSEV]);
	
	if(logRa[ApSEV]<0) logRa[ApSEV]=0;
	ApSEV++;
	
	add_punto();
	
	return 0;
}

// ----------------------------------------------------------------------------------------------------
void Sig_Click(void *ptr)
{
	Serial.println(F("__________________________________________________"));
	Serial.println(F("CLICK BOTON: Adquirir nuevo dato..."));

	if(repetir==1)
	{
		ApSEV--;
		P6_bSiguiente.setText("Siguiente dato");
	}
	
	Serial.print("ApSEV       : ");  Serial.println(ApSEV);
	Serial.print("dimAB[ApSEV]: ");  Serial.println(dimAB[ApSEV]);
	Serial.print("ABMAX_Sev   : ");  Serial.println(ABMAX_Sev);
	
	if(dimAB[ApSEV]>ABMAX_Sev) return;				// YA TERMINO LA ADQUISICION DE ESTE SONDEO.
	if(ApSEV>=30) return;							// YA TERMINO LA ADQUISICION DE ESTE SONDEO.
	
	bzero(&adq, sizeof(adq));
	
	//  ADQUIRIR DATOS.
	P6_jProgressBar.setValue(4);
	
	char 	str[13];
	float	AB= (float)dimAB[ApSEV];
	float	mn= ((float)dimMN[ApSEV])/10;
	sprintf(str, "AB/2: %.1f", AB);			P6_tAB2.setText(str);		Serial.println(str);		// AB/2
	sprintf(str, "mn/2: %.1f", mn);			P6_tmn2.setText(str);		Serial.println(str);		// mn/2
	
	ProgressBar=0;
	long	tfin = millis() + 20000;		// esperar que termine la adquisicion.
	while(tfin>millis())
	{
		if(ProgressBar>0)
		{
			// verificar q no entre al switch constantemente.
			switch(ProgressBar)
			{
				case 0:
					P6_jProgressBar.setValue(4);
					break;
				case 1:									// SP1.
					P6_jProgressBar.setValue(28);
					sprintf(str, "SP: %.2f mV", adq.SP1);		P6_tSP.setText(str);	// SP
					break;
				case 2:									// V1.
					P6_jProgressBar.setValue(52);
					sprintf(str, "V1: %.2f mV", adq.V1);		P6_tV1.setText(str);				// V1
					sprintf(str, "I1: %.2f mA", adq.I1);		P6_tI1.setText(str);				// I1
					break;
				case 3:									// SP2.
					P6_jProgressBar.setValue(76);
					sprintf(str, "SP: %.2f mV", (adq.SP1 + adq.SP2)/2);	P6_tSP.setText(str);	// SP
					break;
				case 4:									// V2.
					P6_jProgressBar.setValue(100);
					sprintf(str, "V2: %.2f mV", adq.V2);		P6_tV2.setText(str);				// V2
					sprintf(str, "I2: %.2f mA", adq.I2);		P6_tI2.setText(str);				// I2
					break;
				case 5:									// SP3.
					P6_jProgressBar.setValue(100);
					break;
			}
			
			if(ProgressBar>=5) break;		// Fin de la adquisicion.
			
			ProgressBar=0;
		}
		delay(10);
	}
	P6_bSiguiente.Set_background_color_bco(63488);

	// Sale si la prueba de los electrodos fue NEGATIVA.
	if(ProgressBar > 5) return;
	
	// VALIDAR LA INFORMACION ADQUIRIDA.
	repetir=0;
	Serial.print("dV:      ");		Serial.print(adq.V1-adq.SP1);	Serial.print(" | ");		Serial.println(adq.V2-adq.SP2);
	Serial.print("I :      ");		Serial.print(adq.I1);			Serial.print(" | ");		Serial.println(adq.I2);
	
	if((adq.I1<5)||(adq.I2<5))		// verificacion del nivel de corriente medido.
	{
		Serial.println("Core1: Corriente MUY BAJA!!!");
		char	msg[330];
		
		sprintf(msg, "Corriente AB, muy baja,\\rI1:  %.2f,    I2:  %.2f", adq.I1, adq.I2);
		Nex_msgAdvertencia(msg);
		delay(2000);
		Nex_page(PGRAFICA);
		delay(700);
		actualizar_Ra();  // Esperar mientras muestra el mensaje emergente.*/
		return;
	}
	else
	{
		if((abs(adq.V1-adq.SP1)<2)||(abs(adq.V2-adq.SP2)<2))	// delta de voltaje es muy bajito.
		{
			Serial.println("Core1: dV MUY BAJO!!!");
			char	msg[330];
			sprintf(msg, "delta de voltaje (dV) muy bajo,\\rdV1:  %.2f,   dV2:  %.2f", adq.V1-adq.SP1, adq.V2-adq.SP2);
			Nex_msgAdvertencia(msg);
			
			delay(2000);
			Nex_page(PGRAFICA);
			delay(700);
			actualizar_Ra();	// Esperar mientras muestra el mensaje emergente.*/
		}
	}	// solo se a adquirido datos.
	
	Serial.print(F("I1: "));	Serial.print(adq.I1);	Serial.print(F(", V1: "));	Serial.print(adq.V1);
	Serial.print(F(", I2: "));	Serial.print(adq.I2);	Serial.print(F(", V2: "));	Serial.println(adq.V2);
	Nex_setAdquisicion(AB, mn);
	proxAbertura();
 
	// MOSTRAR LA INFORMACION ADQUIRIDA.
	
	repetir=0;
}

// -------------------------
void	repetir_punto(void *ptr)		//	&P6_m0_Repetir,
{
  Serial.println(F("CLICK BOTON: Repetir dato..."));
  
  if(ApSEV==0) return;  // No se a adquirido un solo dato.
  
/*  if(repetir==0)
  {
    P6_bSiguiente.Set_background_color_bco(1696);
    P6_bSiguiente.setText("Repetir dato");
    repetir=1;
  }else
  {
    P6_bSiguiente.Set_background_color_bco(63488);
    P6_bSiguiente.setText("Siguiente dato");
    repetir=0;
  }*/
  
  Nex_page(PREPETIR);
}

void	cerrar_Grafica(void *ptr)
{
	Serial.println(F("CLICK BOTON: Cerrar grafica..."));
	Nex_page(PABRIR_SEV);
	
	inicio_menu =0;
	Act_menu_SEV();
}

// -------------------------
void	pb_electrodos(void *ptr)		//	&P8_bPBElectr,	//	--- CONFIGURACION ---
{
	Serial.println(F("CLICK BOTON: Probar electrodos..."));
	__fuenteAB.ON();
	NEX_Test_Electrodes();
	__fuenteAB.OFF();
	
	delay(50);
	Pag_Config(ptr);
}

void	push_format_mem(void *ptr)		//	&P8_bBorrar,
{
	// Borrar toda la memoria.
	time_borrar = millis();
}

void	format_mem(void *ptr)		//	&P8_bBorrar,
{
	// Borrar toda la memoria.
	Serial.print("FORMATEAR MEMORIA - ");	Serial.println(millis());
	long time_press = millis() - time_borrar;
	
	if((time_press>4000)&&(time_press<6000))
	{
		borrarMemoria();
	}
}

void	save_conf(void *ptr)		//	&P8_bGuardar,
{
	// Nada aun.
	// incluir configuracion de tiempo ON-OFF.
}

// --------------------------------------------------
// abre pagina de proyectos.
void	Menu_proyectos(void *ptr)
{
	Serial.println("*** Entra a PROYECTOS ***");
	Nex_page(PABRIR_PROY);
	
	inicio_menu =0;
	Act_menu_proy();
}

// abre pagina de sondeos.
void	Menu_SEVs(void *ptr)
{
	Serial.println("*** Entra a SEVs ***");
	Serial.print("strlen nombre_proyecto: ");	Serial.println(strlen(Proyecto.nombre));
	
	if(strlen(Proyecto.nombre)==0)
	{
		Serial.println("No hay proyecto abierto...");
		return;
	}
	
	Nex_page(PABRIR_SEV);
	
	inicio_menu =0;
	Act_menu_SEV();
}

// abre pagina de temp.
void	Pag_Temperturas(void *ptr)
{
	Nex_page(PTEMPER);
	Serial.println("*** Entra a MEDIDA DE TEMPERATURAS ***");
	
	char buff[10];
	sprintf(buff, "%d", NumSondeos);
	delay(100);
	
	P1_tArchivos.setText(buff);
	
	cargaNivelesVolt();
}

// abre pagina de configuracion.
void	Pag_Config(void *ptr)
{
	Serial.println("*** Entra a MEDIDA DE CONFIGURACION ***");
	Nex_page(PCONF);
	
	// carga la fecha y hora actual.
	gps_neo.leerGPS();
	
	// -------------------------
	P8_tCoord.setText(gps_neo.coordenadas);		// debe tener la coordenada N y E en el mismo string.
	
	char buff[10];
	sprintf(buff, "%d", NumSondeos);
	P8_tNumSondeos.setText(buff);
}

// * ***********************************************************************************************
// *                                           TEST ELECTRODES                                     *
// *************************************************************************************************
int NEX_Test_Electrodes()
{
	Nex_page(PPRUEBA_ELEC);
	
	digitalWrite(E_PE, HIGH);
	bool	STOP=false;
	long tout= millis()+3000;
	int ret=1;
	
	while(!STOP)
	{
		STOP = true;
		int		electrodo = 0xff;
    //electrodo = testElectrodos.probarElectrodos();
    
		char	txtMesg[120] = "";
		Serial.print("**************************************************   ");      Serial.println(electrodo, BIN);
		
		// --------------------------------------------------
		if((electrodo&0x88)==0)
		{
			Serial.println("Error RA");		P9_qA.setPic(9);
			sprintf(txtMesg, "%sVerificar electrodo A...\\r", txtMesg);
			STOP = false;
		}else
		{
			P9_qA.setPic(8);
		}
		
		// --------------------------------------------------
		if((electrodo&0x44)==0)
		{
			Serial.println("Error RB");		P9_qB.setPic(9);
			sprintf(txtMesg, "%sVerificar electrodo B...\\r", txtMesg);
			STOP = false;
		}else
		{
			P9_qB.setPic(8);
		}
		
		// --------------------------------------------------
		if((electrodo&0x22)==0)
		{
			Serial.println("Error Rm");		P9_qm.setPic(9);
			sprintf(txtMesg, "%sVerificar electrodo m...\\r", txtMesg);
			STOP = false;
		}else
		{
			P9_qm.setPic(8);
		}
		
		// --------------------------------------------------
		if((electrodo&0x11)==0)
		{
			Serial.println("Error Rn");		P9_qn.setPic(9);
			sprintf(txtMesg, "%sVerificar electrodo n...", txtMesg);
			STOP = false;
		}else
		{
			P9_qn.setPic(8);
		}
		
		// --------------------------------------------------
		if(STOP) P9_tMensaje.setText("ELECTRODOS OK!!!");
		else     P9_tMensaje.setText(txtMesg);
		
		if(tout<millis())
		{
			ret = -1;
			STOP = true;
		}
	}
	
	digitalWrite(E_PE, LOW);
	return ret;
}

// * ***********************************************************************************************
// *                                              REPETIR                                          *
// *************************************************************************************************
void	In_Repetir(void *ptr)		// 	&P10_bOk,
{
  Serial.println(F("CLICK BOTON: Repetir dato Ok..."));
	char	strdata[10];
	
	bzero(&strdata, sizeof(strdata));
	
	P10_tRepeticiones.getText(strdata, sizeof(strdata));
	int repetir = atoi(strdata);

  Serial.print("ApSEV: ");  Serial.println(ApSEV);
	if((0<repetir)&&(repetir<ApSEV))
	{
		ApSEV = ApSEV - repetir;
	}

  Serial.print("repetir: ");  Serial.println(repetir);
  Serial.print("ApSEV: ");  Serial.println(ApSEV);
	delay(50);
	Nex_page(PGRAFICA);
	delay(600);
	actualizar_Ra();
  proxAbertura();
}

void	P10_Cerrar(void *ptr)		// 	&P10_bCerrar,
{
  Serial.println(F("CLICK BOTON: Repetir dato CANCEL..."));
	delay(50);
	Nex_page(PGRAFICA);
	delay(600);
	actualizar_Ra();
}

// ---------------------------------------- INICIO ----------------------------------------
int		Nex_msgInicio(char *strMsg) {	P0_tInicio.setText(strMsg); 	return 0;	}		// mensajes mientras carga.


// * ************************************************************************************************
// *                                       NEXLOOP(nex_listen_list)                                 *
// *************************************************************************************************
NexTouch *nex_listen_list[] =
{
	&P2_bArriba,	//	--- ABRIR PROY ---
	&P2_bAbajo,
	&P2_bAbrir,
	&P2_bNuevo,
	&P2_bBorrar,
	&P2_scrollFile,
	
	&P2_mLinea0,
	&P2_mLinea1,
	&P2_mLinea2,
	&P2_mLinea3,
	&P2_mLinea4,
	&P2_mLinea5,
	&P2_mLinea6,
	&P2_mLinea7,
	&P2_mLinea8,
	&P2_mLinea9,
	
	&P3_bNuevo,		//	--- NUEVO PROY ---
	
	&P4_bArriba,	//	--- ABRIR SEV ---
	&P4_bAbajo,
	&P4_bAbrir,
	&P4_bNuevo,
	&P4_bBorrar,
	&P4_scrollFile,

	&P4_mLinea0,
	&P4_mLinea1,
	&P4_mLinea2,
	&P4_mLinea3,
	&P4_mLinea4,
	&P4_mLinea5,
	&P4_mLinea6,
	&P4_mLinea7,
	&P4_mLinea8,
	&P4_mLinea9,
	
	&P5_bNuevo,		//	--- NUEVO SEV ---
	
	&P6_bSiguiente,	//	--- GRAFICA ---
	&P6_bCerrar,
	&P6_m0_Repetir,
	
//	&P7_bOk,		//	--- MENSAJE ---
//	&P7_bCerrar,
	
	&P8_bPBElectr,	//	--- CONFIGURACION ---
	&P8_bBorrar,
	&P8_bGuardar,
	
//	&P9_bCerrar,	//	--- PRUEBA ELECTRODOS ---
	
	&P1_Proyectos,
	&P1_SEVs,
//	&P1_Temper,
	&P1_Config,

//	&P2_Proyectos,
	&P2_SEVs,
	&P2_Temper,
	&P2_Config,

	&P3_Proyectos,
	&P3_SEVs,
	&P3_Temper,
	&P3_Config,

	&P4_Proyectos,
//	&P4_SEVs,
	&P4_Temper,
	&P4_Config,

	&P5_Proyectos,
	&P5_SEVs,
	&P5_Temper,
	&P5_Config,

	&P8_Proyectos,
	&P8_SEVs,
	&P8_Temper,
//	&P8_Config,

	&P10_bOk,
	&P10_bCerrar,
	NULL
};

void nextion_begin()
{
	inicio_menu =0;
	
	nexSerial.setTX(4);
	nexSerial.setRX(5);
	nexInit();
	
	P2_bArriba.attachPop(menu_up, 			&P2_bArriba);		//	--- ABRIR PROY ---
	P2_bAbajo.attachPop(menu_down, 			&P2_bAbajo);
	P2_bAbrir.attachPop(abrir_proy, 		&P2_bAbrir);
	P2_bNuevo.attachPop(nuevo_proy, 		&P2_bNuevo);
	P2_bBorrar.attachPush(push_eliminar_proy, &P2_bBorrar);	// *****
	P2_bBorrar.attachPop(eliminar_proy, 	&P2_bBorrar);
	P2_scrollFile.attachPop(scrollbar_proy, &P2_scrollFile);
	
	P2_mLinea0.attachPop(proy_linea0,		&P2_mLinea0);
	P2_mLinea1.attachPop(proy_linea1,		&P2_mLinea1);
	P2_mLinea2.attachPop(proy_linea2,		&P2_mLinea2);
	P2_mLinea3.attachPop(proy_linea3,		&P2_mLinea3);
	P2_mLinea4.attachPop(proy_linea4,		&P2_mLinea4);
	P2_mLinea5.attachPop(proy_linea5,		&P2_mLinea5);
	P2_mLinea6.attachPop(proy_linea6,		&P2_mLinea6);
	P2_mLinea7.attachPop(proy_linea7,		&P2_mLinea7);
	P2_mLinea8.attachPop(proy_linea8,		&P2_mLinea8);
	P2_mLinea9.attachPop(proy_linea9,		&P2_mLinea9);
	
	P3_bNuevo.attachPop(crear_proy, 		&P3_bNuevo);		//	--- NUEVO PROY ---
	
	P4_bArriba.attachPop(menu2_up,			&P4_bArriba);		//	--- ABRIR SEV ---
	P4_bAbajo.attachPop(menu2_down,			&P4_bAbajo);
	P4_bAbrir.attachPop(abrir_SEV,			&P4_bAbrir);
	P4_bNuevo.attachPop(nuevo_SEV,			&P4_bNuevo);
	P4_bBorrar.attachPush(push_eliminar_SEV, &P4_bBorrar);
	P4_bBorrar.attachPop(eliminar_SEV,		&P4_bBorrar);
	P4_scrollFile.attachPop(scrollbar_SEV, 	&P4_scrollFile);
	
	P4_mLinea0.attachPop(SEV_linea0,		&P4_mLinea0);
	P4_mLinea1.attachPop(SEV_linea1,		&P4_mLinea1);
	P4_mLinea2.attachPop(SEV_linea2,		&P4_mLinea2);
	P4_mLinea3.attachPop(SEV_linea3,		&P4_mLinea3);
	P4_mLinea4.attachPop(SEV_linea4,		&P4_mLinea4);
	P4_mLinea5.attachPop(SEV_linea5,		&P4_mLinea5);
	P4_mLinea6.attachPop(SEV_linea6,		&P4_mLinea6);
	P4_mLinea7.attachPop(SEV_linea7,		&P4_mLinea7);
	P4_mLinea8.attachPop(SEV_linea8,		&P4_mLinea8);
	P4_mLinea9.attachPop(SEV_linea9,		&P4_mLinea9);
	
	P5_bNuevo.attachPop(crear_SEV, 			&P5_bNuevo);		//	--- NUEVO SEV ---
	
	P6_bCerrar.attachPop(cerrar_Grafica,	&P6_bCerrar);	//	--- GRAFICA ---
	P6_bSiguiente.attachPop(Sig_Click,		&P6_bSiguiente);	//	--- GRAFICA ---
	P6_m0_Repetir.attachPop(repetir_punto, 	&P6_m0_Repetir);
	
//	P7_bOk.attachPop(msg_ok, 				&P7_bOk);				//	--- MENSAJE ---
//	P7_bCerrar.attachPop(msg_cerrar, 		&P7_bCerrar);
	
	P8_bPBElectr.attachPop(pb_electrodos, 	&P8_bPBElectr);	//	--- CONFIGURACION ---
	P8_bBorrar.attachPush(push_format_mem, 	&P8_bBorrar);
	P8_bBorrar.attachPop(format_mem, 		&P8_bBorrar);
	P8_bGuardar.attachPop(save_conf, 		&P8_bGuardar);
	
//	P9_bCerrar.attachPop(pe_cerrar, 		&P9_bCerrar);		//	--- PRUEBA ELECTRODOS ---
	
	// --------------------------------------------------
	P1_Proyectos.attachPop(Menu_proyectos, 	&P1_Proyectos);
	P1_SEVs.attachPop(Menu_SEVs, 			&P1_SEVs);
	//P1_Temper.attachPop(Pag_Temperturas, 	&P1_Temper);
	P1_Config.attachPop(Pag_Config, 		&P1_Config);
	
	//P2_Proyectos.attachPop(Menu_proyectos, 	&P2_Proyectos);
	P2_SEVs.attachPop(Menu_SEVs, 			&P2_SEVs);
	P2_Temper.attachPop(Pag_Temperturas, 	&P2_Temper);
	P2_Config.attachPop(Pag_Config, 		&P2_Config);
	
	P3_Proyectos.attachPop(Menu_proyectos, 	&P3_Proyectos);
	P3_SEVs.attachPop(Menu_SEVs, 			&P3_SEVs);
	P3_Temper.attachPop(Pag_Temperturas, 	&P3_Temper);
	P3_Config.attachPop(Pag_Config, 		&P3_Config);
	
	P4_Proyectos.attachPop(Menu_proyectos, 	&P4_Proyectos);
	//P4_SEVs.attachPop(Menu_SEVs, 			&P4_SEVs);
	P4_Temper.attachPop(Pag_Temperturas, 	&P4_Temper);
	P4_Config.attachPop(Pag_Config, 		&P4_Config);
	
	P5_Proyectos.attachPop(Menu_proyectos, 	&P5_Proyectos);
	P5_SEVs.attachPop(Menu_SEVs, 			&P5_SEVs);
	P5_Temper.attachPop(Pag_Temperturas, 	&P5_Temper);
	P5_Config.attachPop(Pag_Config, 		&P5_Config);
	
	P8_Proyectos.attachPop(Menu_proyectos, 	&P8_Proyectos);
	P8_SEVs.attachPop(Menu_SEVs, 			&P8_SEVs);
	P8_Temper.attachPop(Pag_Temperturas, 	&P8_Temper);
	//P8_Config.attachPop(Pag_Config, 		&P8_Config);
	
	P10_bOk.attachPop(In_Repetir, 	&P10_bOk);
	P10_bCerrar.attachPop(P10_Cerrar, 	&P10_bCerrar);
	
	sendCommand("dims=100");
}


/*
// ---------------------------------------------------------------------------
void Flecha_Arriba_Click(void *ptr)
{
	Serial.print(F("CLICK BOTON: Flecha Arriba..."));
	if(0 < offset)
	{
		offset--;
		NombresArchivos();
		b2_Abajo.Set_background_crop_picc(3);	// Boton prendo.
	}
	
	if(0 == offset)
	{
		b1_Arriba.Set_background_crop_picc(4);	// Boton apagado.
	}
}

// -------------------------
void Flecha_Abajo_Click(void *ptr)
{
	Serial.print(F("CLICK BOTON: Flecha Abajo..."));
	if((offset+8) < Num_Archivos)
	{
		offset++;
		NombresArchivos();
		b1_Arriba.Set_background_crop_picc(3);	// Boton prendo.
	}
	
	if((offset+8) == Num_Archivos)
	{
		b2_Abajo.Set_background_crop_picc(4);	// Boton apagado.
	}
}

// -------------------------

// ---------------------------------------- NUEVO ----------------------------------------
*/
