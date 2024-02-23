#include <ArduinoJson.h>

#define L_MAX_MEN 50

// --------------------------------------------------
/*
	{comando:"equipo"}
	{comando:"proyectos"}
	{comando:"sondeos", proyecto:"nombre_proyecto"}
	{comando:"datos", sondeo:"nombre_sondeo"}
*/
// Cargar proyectos.
int envia_proyectos()
{
	listProy(true);
	return 0;
}

// Abrir proyecto.
int abre_proyecto(char *NombreProyecto)
{
	abrirProy(NombreProyecto);
	listSEVs();
	return 0;
}

// Abrir sondeo.
int abre_sondeo(char *NombreSEV)
{
	abrirSEV(NombreSEV);
	return 0;
}

// --------------------------------------------------
int proc_message(char *message)
{
		Serial.print(F("Conf. RX: "));		Serial.println(message);
		
		char prueba[50];
		strcpy(prueba, "cadena de prueba para strstr");
		Serial.print("prueba de strstr: ");		Serial.println(strstr(prueba, "cadena de prueba para strstr"));
		Serial.print("prueba de strstr: ");		Serial.println(strstr(prueba, "para"));
		Serial.print("prueba de strstr: ");		Serial.println(strstr(prueba, "nadaa"));
		
		if(strstr(message, "comando") == NULL)
		{
			return -1;
		}
		StaticJsonDocument<50> doc;
		
		doc.clear();
		DeserializationError error = deserializeJson(doc, message);
		if(error)
		{
			Serial.print(F("deserializeJson() fallo: "));
			Serial.println(error.c_str());
			return -2;
		}
		
		const char  *comando = doc["comando"];
		if(strcmp(comando, "equipo")==0)
		{
			Serial.println("______________________________________________");
			Serial.println("Equipo entra en modo conexion con Software PC.");
			Serial.println("@Conectado>{\"equipo\":\"Equipo de sondeos 3.0\"}");
//			STOP==false;
		}
		
		if(strcmp(comando, "proyectos")==0)
		{
			listProy(true);
		}
		
		if(strcmp(comando, "sondeos")==0)
		{
			const char *NombreProyecto = doc["proyecto"];
			abrirProy((char *)NombreProyecto);
			listSEVs(true);
		}
		
		if(strcmp(comando, "datos")==0)
		{
			const char *NombreSEV = doc["sondeo"];
			abrirSEV((char *)NombreSEV);
		}
		
		return 1;
//	}while(!STOP);

}
	

// --------------------------------------------------
int recvSerial()
{
	if(Serial.available())
	{
		Serial.println(F("recibiendo desde PC..."));
		char message[L_MAX_MEN+1];
		bzero(&message, sizeof(message));
		int i=0;
		
		long tini = millis() + 1000;
		while(tini>millis())
		{
			if(Serial.available())
			{
				tini = millis() + 1000;
				char c = Serial.read();
				Serial.print(c);    Serial.print(" ");
				message[i++]=c;
				
				if(i==L_MAX_MEN)
				{
					Serial.print(F("Conf. RX: "));    Serial.println(message);
					Serial.println(F("Desbordamiento de variable message !!!"));
					return 0;
				}
				
				if(0xff&c == 0x0a)
				{
					message[i++]=0;
					return proc_message(message);
				}
			}
		}
	}
	return 0;
}
