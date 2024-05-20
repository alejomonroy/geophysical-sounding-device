#ifndef __FileSystem__
#define __FileSystem__

#include <ListLib.h>

//	Define las funciones de organizacion de proyectos y sondeos
//	en un sistema de archivos sobre la memoria flash.
//	
//	Define funciones y formato JSON para el almacenamiento de configuracion
//	y datos de cada proyecto y sondeos.

#include <LittleFS.h>
#include <ArduinoJson.h>
#include <CSV_Parser.h>

#define FileSystem LittleFS

int NumSondeos = 0;

typedef struct 
{
	char	fecha[20];
	char	hora[10];
	char	operador[21];
	char	municipio[21];
	char	nombre[25];
} PROYECTO;
PROYECTO Proyecto;

// -------------------------
typedef struct 
{
	char	fecha[20];
	char	hora[10];
	char	coord[25];
	int		AB2;
	char	nombre[25];
} SEV;
SEV sev;

float	datosSEV[9][40];	// 8*40*4 bytes = 1280 Bytes.

// * Estructuras en donde se almacena los proyectos
// * y los sondeos.
 
typedef struct
{
	char nombre[25];
	time_t creacion;
	size_t tamano;
} FileItem;

List<FileItem> proy_List;
List<FileItem> SEV_List;

// --------------------------------------------------
int	fileFirstName(char *strOut, char *strIn)
{
	int i;
	for(i=0; i< strlen(strIn); i++) if(strIn[i]=='.') break;
	for(int j=0; j<i; j++) strOut[j] = strIn[j];
	
	strOut[i]=0;
	
	return i;
}

// -------------------------
void	listProy();
int		fileFirstName(char *strOut, char *strIn);
void	OrdenarFiles(List<FileItem> &fileList);
void	init_FileSystem();
void	fileInfo(File file);
void	listProy(boolean debug);
void	listProy();
int		abrirProy(char *NombreProyecto);
int		nuevoProy();
bool	existeProy(char *NombreProyecto);
bool	removeProy(char *folder);
int		borrarProy(char *NombreProyecto);
void	listSEVs(boolean debug);
void	listSEVs();
int		abrirSEV(char *NombreSEV);
int		nuevoSEV();
bool	existeSEV(char *NombreSEV);
int		borrarSEV(char *NombreSEV);
int		guardarDatos(float AB, float mn, float SP, float dV1, float dV2, float Ra, float e);
int		_NumSondeos();
int		borrarMemoria();

// -------------------------
// --------------------------------------------------
void OrdenarFiles(List<FileItem> &fileList)
{
	time_t maximo=0;
	int indexMaximo;
	
	Serial.print("----- ORDENAR FILES -----");  Serial.println(millis());
	Serial.print("Numero de archivos: ");	Serial.println(fileList.Count());
	if(fileList.Count()==0) return;

	int i;
	for(i=0; i<fileList.Count()-1; i++)
	{
		indexMaximo = i;
		maximo=0;
		
		for(int j=i; j<fileList.Count(); j++)
		{
			if(fileList[j].creacion > maximo)
			{
				maximo = fileList[j].creacion;
				indexMaximo = j;
			}
		}
		
		FileItem fileItem;
		fileItem = fileList[indexMaximo];
		fileList.Remove(indexMaximo);
		fileList.Insert(i, fileItem);
		
		Serial.print("@FILE>{\"nombre\": \"");		Serial.print(fileItem.nombre);
		Serial.print("\", \"tamaño\": ");		Serial.print(fileItem.tamano);
		
		char buff[80];
		strftime(buff, sizeof(buff), "%x    %X", localtime(&fileItem.creacion));
		Serial.print(", \"creacion1\": ");		Serial.print((long)fileItem.creacion);
		Serial.print(", \"creacion2\": \"");    Serial.print(buff);
		Serial.println("\"}");
	}
	
	FileItem fileItem;
	fileItem = fileList[i];
	
	Serial.print("@FILE>{\"nombre\": \"");    Serial.print(fileItem.nombre);
	Serial.print("\", \"tamaño\": ");   Serial.print(fileItem.tamano);
	
	char buff[80];
	strftime(buff, sizeof(buff), "%x    %X", localtime(&fileItem.creacion));
	Serial.print(", \"creacion1\": ");    Serial.print((long)fileItem.creacion);
	Serial.print(", \"creacion2\": \"");    Serial.print(buff);
	Serial.println("\"}");
	
	Serial.print("----- FIN Ordenar Files -----");  Serial.println(millis());
}

// --------------------------------------------------
void init_FileSystem()
{
	bzero(&Proyecto, sizeof(Proyecto));
	bzero(&sev, sizeof(sev));
	
	FileSystem.begin();
	
	FSInfo fs_info;
	FileSystem.info(fs_info);
	
	_NumSondeos();
	
	Serial.print("Memoria: ");        Serial.println(fs_info.totalBytes);
	Serial.print("Memoria usada: ");  Serial.println(fs_info.usedBytes);
}

// --------------------------------------------------
void fileInfo(File file)
{
	//* Muestra datos basicos del archivo.
	//*  FILE: nombre.txt   54 bytes    1970-01-01  00:00:57
	if(file.isFile())       Serial.print(">FILE:\t");
	if(file.isDirectory())  Serial.print(">DIR :\t");
	
	Serial.print(file.name());    Serial.print("\t");   Serial.print(file.size());
	
	time_t cr = file.getCreationTime();
	struct tm * tmstruct = localtime(&cr);
	Serial.printf(" bytes\t%d-%02d-%02d\t%02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
}

// --------------------------------------------------
void listProy(boolean debug)
{
	Serial.println(">Lista de proyectos:");
	proy_List.Clear();
	
	Dir root = FileSystem.openDir("/");
	
	while(root.next())
	{
		File file = root.openFile("r");
		if(debug==true) fileInfo(file);         // muestra la informacion del archivo.
		
		if(file.isDirectory())
		{
			FileItem proyecto;
			char  strfile[50];
			
			sprintf(strfile, "/%s/datos.json", file.name());  //Serial.println(strfile);	// Abre JSON de cada proyecto.
			File fileJSON = FileSystem.open(strfile,"r");
			
			strcpy(proyecto.nombre, file.name());
			proyecto.creacion = fileJSON.getCreationTime();
			proyecto.tamano   = file.size();
			
			proy_List.Add(proyecto);			// Llenar una lista con datos del tipo PROYECTO.
			fileJSON.close();
		}
		
		file.close();
	}
	
	// Ordenar los proyectos desde el mas nuevo al mas antiguo.
	Serial.println("@LISTAProy>");
	OrdenarFiles(proy_List);
	Serial.println("@LISTAEnd>");
}

// -------------------------
void listProy()
{
	listProy(false);
}

// --------------------------------------------------
int abrirProy(char *NombreProyecto)
{
	bzero(&Proyecto, sizeof(Proyecto));
	
	StaticJsonDocument<500> doc;
	char strbuff[500];
	char strfile[40];
	
	sprintf(strfile, "/%s/datos.json", NombreProyecto);
	File file = FileSystem.open(strfile,"r");
	
	Serial.print("Abrir proyecto: ");	Serial.println(strfile);
	if (!file)
	{
		Serial.println("No es posible abrir el proyecto...");
		return -1;
	}
	
	if(file.available())
	{
		int n = file.read((uint8_t *)strbuff, 500);
		
		Serial.print("leidos: ");	Serial.println(n);
		Serial.print("@ABREProy>");	Serial.write(strbuff, n);	Serial.println();
		
		// Obtener elementos JSON validos.
		doc.clear();
		DeserializationError error = deserializeJson(doc, strbuff);
		
		strlcpy( Proyecto.fecha,	doc["fecha"]|"",	sizeof(Proyecto.fecha));
		strlcpy( Proyecto.hora,		doc["hora"]|"",		sizeof(Proyecto.hora));
		strlcpy( Proyecto.operador,	doc["operador"]|"operador",	sizeof(Proyecto.operador));
		strlcpy( Proyecto.municipio,doc["municipio"]|"municipio", sizeof(Proyecto.municipio));
		strlcpy( Proyecto.nombre,	doc["nombre"]|"nombre proyecto",sizeof(Proyecto.nombre));
		
		return 1;
	}
	
	return 0;
}

// --------------------------------------------------
int nuevoProy()
{
	StaticJsonDocument<500> doc;
	char strbuff[500];
	char strfile[40];
	
	sprintf(strfile, "/%s/datos.json", Proyecto.nombre);
	File file = FileSystem.open(strfile, "w");
	
	Serial.print("Crear proyecto: ");	Serial.println(strfile);
	if(file)
	{
		// crear el archivo json.
		doc.clear();
		doc["fecha"]		= Proyecto.fecha;
		doc["hora"]			= Proyecto.hora;
		doc["operador"]		= Proyecto.operador;
		doc["municipio"]	= Proyecto.municipio;
		doc["nombre"]		= Proyecto.nombre;
		
		serializeJson(doc, strbuff);
		Serial.println(strbuff);
		
		file.write(strbuff, strlen(strbuff));
		file.close();
		
		return 1;
	}
	else
	{
		Serial.println("No se puede crear el archivo, No se puede crear el proyecto");
		return -1;
	}
}

// --------------------------------------------------
bool existeProy(char *NombreProyecto)
{
	char strfile[40];
	sprintf(strfile, "/%s/", NombreProyecto);
	
	return FileSystem.exists(strfile);
}

// --------------------------------------------------
bool removeProy(char *folder)
{
	Serial.printf("Lista de archivos a eliminar: %s\n", folder);
	
	char	strfile[32];
	sprintf(strfile, "/%s/", folder);
	Dir root = FileSystem.openDir(strfile);
	
	while(root.next())
	{
		char strName[20];
		root.fileName().toCharArray(strName, 20);
		
		sprintf(strfile, "/%s/%s", folder, strName);
		FileSystem.remove(strfile);
		Serial.println(strfile);
	}
	return true;
}

// --------------------------------------------------
int borrarProy(char *NombreProyecto)
{
	char strfile[40];
	
	sprintf(strfile, "%s", NombreProyecto);
	if(removeProy(NombreProyecto))
	{
		Serial.print("Proyecto eliminado...");    Serial.println(strfile);
	}else
	{
		Serial.print("No fue posible eliminar el proyecto...");    Serial.println(strfile);
		return -1;
	}
	
	return 1;
}

// --------------------------------------------------
void listSEVs(boolean debug)
{
	Serial.printf("Lista de sondeos: %s - ", Proyecto.nombre);  Serial.println(millis());
	SEV_List.Clear();
	
	char	strfile[50];
	sprintf(strfile, "/%s/", Proyecto.nombre);
	
	Serial.printf("millis: %d\n", millis());
	
	Dir root = FileSystem.openDir(strfile);
	while(root.next())
	{
		File file = root.openFile("r");
		if(debug==true) fileInfo(file);         // muestra la informacion del archivo.
		
		if(file.isFile())
		{
			// adicionar elemento al arreglo. filtrando por extencion.
			if(strstr(file.name(), ".json"))
			{
				continue;	// salta archivos .json
			}
			
			FileItem sondeo;
			
			fileFirstName(sondeo.nombre, (char *)file.name());
			sondeo.creacion = file.getCreationTime();
			sondeo.tamano = file.size();
			
			SEV_List.Add(sondeo);
		}
		
		file.close();
	}
	
	Serial.println("@LISTASondeos>");
	OrdenarFiles(SEV_List);
	Serial.println("@LISTAEnd>");
}

void listSEVs()
{
	listSEVs(false);
}
// --------------------------------------------------
int abrirSEV(char *NombreSEV)
{
	char strfile[40];
	bzero(&sev, sizeof(sev));
	
	// --------------- Abrir archivo JSON. ---------------
	sprintf(strfile, "/%s/%s.json", Proyecto.nombre, NombreSEV);	// archivo JSON del sondeo.
	Serial.print("Abrir HEADER del sondeo: ");  Serial.println(strfile);
	
	File file = FileSystem.open(strfile,"r");
	
	if (!file)
	{
		Serial.println("No es posible abrir HEADER del sondeo...");
		return -1;
	}
	
	if(file.available())
	{
		int n;
		StaticJsonDocument<500> doc;
		char strbuff[500];
		
		n=file.read((uint8_t *)strbuff, 500);
		
		Serial.print("leidos: ");   Serial.println(n);
		Serial.print("@ABRESev>");
		Serial.write(strbuff, n);	Serial.println();
		
		// Obtener elementos JSON validos.
		doc.clear();
		DeserializationError error = deserializeJson(doc, strbuff);
		
		strlcpy( sev.fecha,	doc["fecha"]|"2000:01:01",			sizeof(sev.fecha));
		strlcpy( sev.hora,	doc["hora"]|"13:01",				sizeof(sev.hora));
		strlcpy( sev.coord,	doc["coordenadas"]|"4.000000, -74.000000",sizeof(sev.coord));
		strlcpy( sev.nombre,doc["nombre"]|"nombre SEV",			sizeof(sev.nombre));
		sev.AB2			= doc["AB"];
		
		Serial.print("sev.fecha : ");		Serial.println(sev.fecha);
		Serial.print("sev.hora  : ");		Serial.println(sev.hora);
		Serial.print("sev.coord : ");		Serial.println(sev.coord);
		Serial.print("sev.nombre: ");		Serial.println(sev.nombre);
		Serial.print("sev.AB2   : ");		Serial.println(sev.AB2);
	}
	file.close();
	
	// --------------- Abrir matriz de datos. ---------------
	sprintf(strfile, "/%s/%s.txt", Proyecto.nombre, NombreSEV);	// archivo JSON del sondeo.
	Serial.print("Abrir matriz de datos del sondeo: "); Serial.println(strfile);
	file = FileSystem.open(strfile,"r");
	
	if (!file)
	{
		Serial.println("No es posible abrir matriz de datos del sondeo...");
		return -2;
	}
	
	if(file.available())
	{
		int n;
		char buffTxt[2050];
		bzero(&buffTxt, sizeof(buffTxt));
		
		n=file.read((uint8_t *)buffTxt, 2048);
		file.close();
		
		Serial.print("leidos: ");   Serial.print(n);    Serial.print(" - ");   Serial.println(millis());
		
		/*char *token;
		token = strtok(buffTxt, "\n");
		
		while(token != NULL)
		{
			Serial.print("@DATA>");	Serial.println(token);
			token = strtok(NULL, "\n");
		}*/
		
		//Serial.write(buffTxt, n); Serial.println();
		delay(400);
		Serial.print(" - ");   Serial.println(millis());
		
		// parcear los datos del sondeo en una matriz.
		// AB/2		mn	SP	dV1	dV2		I1	I2		Ra	e"
		CSV_Parser cp(buffTxt, /*format*/ "fffffffff", /*has_header*/ true, /*delimiter*/ '\t');
		Serial.print("Filas: ");    Serial.print(cp.getRowsCount());
		Serial.print(", Columnas: "); Serial.println(cp.getColumnsCount());
		
		float *AB = (float *)cp[0];
		float *mn = (float *)cp[1];
		float *Ra = (float *)cp[7];
		
		float *col0= (float*)cp[0];
		float *col1= (float*)cp[1];
		float *col2= (float*)cp[2];
		float *col3= (float*)cp[3];
		float *col4= (float*)cp[4];
		float *col5= (float*)cp[5];
		float *col6= (float*)cp[6];
		float *col7= (float*)cp[7];
		float *col8= (float*)cp[8];
		
		Serial.println("@DATA>AB/2\tmn\tSP\tdV1\tdV2\tI1\tI2\tRa\te\n");
		
		int i, j=0;
		for(i = 0; i < cp.getRowsCount(); i++)
		{
			Serial.print("@DATA>");
			Serial.print(col0[i]);      Serial.print("\t");
			Serial.print(col1[i]);      Serial.print("\t");
			Serial.print(col2[i]);      Serial.print("\t");
			Serial.print(col3[i]);      Serial.print("\t");
			Serial.print(col4[i]);      Serial.print("\t");
			Serial.print(col5[i]);      Serial.print("\t");
			Serial.print(col6[i]);      Serial.print("\t");
			Serial.print(col7[i]);      Serial.print("\t");
			Serial.print(col8[i]);      Serial.println("\t");
			
			if((AB[i] == AB[i+1])&&(mn[i] == mn[i+1]))
			{
				// no graficar
			} else
			{
				// si graficar
				//double  K  = 3.14159265359 * ( AB*AB - mn*mn )/(2*mn);
				//double  Ra = K*dV/I;
				
				if(Ra[i]>1) logRa[j] = log10(Ra[i]);
				else logRa[j] = 0;
				j++;
			}
		}
		
		Serial.println("@EOF>");
		Serial.print("return: ");	Serial.println(j);
		return j;
	}
	file.close();
	
	return 0;
}

// --------------------------------------------------
int nuevoSEV()
{
	StaticJsonDocument<500> doc;
	char strbuff[500];
	char strfile[40];
	
	// --------------- Crear archivo JSON. ---------------
	sprintf(strfile, "/%s/%s.json", Proyecto.nombre, sev.nombre);	// archivo JSON del sondeo.
	File file = FileSystem.open(strfile, "w");
	
	Serial.print("Crear proyecto: ");	Serial.println(strfile);
	if(file)
	{
		// crear el archivo json.
		doc.clear();
		doc["fecha"]	= sev.fecha;
		doc["hora"]		= sev.hora;
		doc["coordenadas"]	= sev.coord;
		doc["AB"]		= sev.AB2;
		doc["nombre"]	= sev.nombre;
		
		serializeJson(doc, strbuff);
		Serial.println(strbuff);
		
		file.write(strbuff, strlen(strbuff));
		file.close();
		
		// poner en ceros la matriz con los datos de adquisicion.
	}
	else
	{
		Serial.println("No se puede crear el archivo JSON, No se puede crear el proyecto");
		return -1;
	}
	
	// --------------- Crear archivo JSON. ---------------
	sprintf(strfile, "/%s/%s.txt", Proyecto.nombre, sev.nombre);	// archivo JSON del sondeo.

	file = FileSystem.open(strfile, "w");
	Serial.print("Crear proyecto: "); Serial.println(strfile);
	if(file)
	{
		file.printf("AB/2\tmn\tSP\tdV1\tdV2\tI1\tI2\tRa\te\n");
		file.close();
	}
	else
	{
		Serial.println("No se puede crear el archivo TXT, No se puede crear el proyecto");
		return -2;
	}
	
	return 1;
}

// --------------------------------------------------
bool existeSEV(char *NombreSEV)
{
	char strfile[40];
	sprintf(strfile, "/%s/%s.json", Proyecto.nombre, NombreSEV);	// archivo JSON del sondeo.
	
	return FileSystem.exists(strfile);
}

// --------------------------------------------------
int borrarSEV(char *NombreSEV)
{
	char strfile[40];
	Serial.println("Eliminando:");
	
	// --------------- Abrir archivo JSON. ---------------
	sprintf(strfile, "/%s/%s.json", Proyecto.nombre, NombreSEV);	// archivo JSON del sondeo.
	Serial.println(strfile);
	
	if(!FileSystem.remove(strfile))
	{
		Serial.println("No fue posible eliminar el HEADER del sondeo...");
		return -1;
	}
	
	// --------------- Abrir matriz de datos. ---------------
	sprintf(strfile, "/%s/%s.txt", Proyecto.nombre, NombreSEV);	// archivo JSON del sondeo.
	Serial.println(strfile);
	
	if(!FileSystem.remove(strfile))
	{
		Serial.println("No fue posible eliminar los datos del sondeo...");
		return -1;
	}
	
	Serial.println("done");
	return 1;
}

// --------------------------------------------------
int guardarDatos(float AB, float mn, float SP, float dV1, float dV2, float Ra, float e)
{
	char  strfile[150];
  
	sprintf(strfile, "/%s/%s.txt", Proyecto.nombre, sev.nombre);	// archivo txt del sondeo.
	File file = FileSystem.open(strfile, "a");
	
	Serial.print("Abrir sondeo: "); Serial.println(strfile);
	
	if(file)
	{
		sprintf(strfile, "%.1f\t%.1f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n", AB, mn, SP, dV1, dV2, adq.I1, adq.I2, Ra, e);
		Serial.print("ADD linea: |"); Serial.println(strfile);
		
		file.printf(strfile);
		file.close();
	}
	else
	{
		Serial.println("No se puede crear el archivo TXT, No se puede crear el proyecto");
		return -2;
	}
	return 1;
}

// --------------------------------------------------
int _NumSondeos()
{
	NumSondeos = 0;
	listProy();
	
	for(int i=0; i<proy_List.Count(); i++)
	{
		strcpy(Proyecto.nombre , proy_List[i].nombre);
		listSEVs();
		
		NumSondeos += SEV_List.Count();
	}
	Proyecto.nombre[0] =0;
	Serial.print("NumSondeos: ");  Serial.println(NumSondeos);
	return NumSondeos;
}

// --------------------------------------------------
int	borrarMemoria()
{
	if (!FileSystem.format())
	{
		Serial.printf("Unable to format(), aborting\n");
		return 0;
	}
	return 1;
}

#endif /* #ifndef __FileSystem__ */
