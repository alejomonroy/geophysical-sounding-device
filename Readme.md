
# Equipo de Sondeos Eléctricos Verticales (SEV) 3.0

Este repositorio contiene el código fuente y las especificaciones técnicas del **Equipo de Sondeos Eléctricos Verticales (SEV) 3.0**, desarrollado para aplicaciones geofísicas. Este equipo está diseñado para realizar mediciones de resistividad eléctrica del suelo con alta precisión y facilidad de uso, integrando hardware y firmware avanzados.

---

## **Características principales**

### **Hardware**

1. **Microcontrolador principal**:
   - Raspberry Pi Pico (RP2040).

2. **Conversores analógico-digital**:
   - 2 x ADS1252 para medidas principales de voltaje y corriente.
   - 2 x ADS1115:
     - Uno conectado a un amplificador OPA4241 para medir voltajes de alimentación y estado de baterías.
     - Otro para verificar la conexión de electrodos mediante baja corriente inducida.

3. **Entradas y sensores**:
   - Amplificador INA114 para entradas diferenciales.
   - Medidor de corriente ACS-5A.

4. **Pantalla táctil**:
   - Pantalla Nextion de 7 pulgadas para interacción con el usuario y visualización de datos.

5. **Sensores de alimentación**:
   - Voltajes monitoreados:
     - Batería principal (12V).
     - Batería del RTC DS3231 (3.6V, 1/2 AA).
     - Lógica de 5V y 3.3V.

### **Software**

1. **Adquisición y procesamiento de datos**:
   - Uso de filtros IIR para reducción de ruido.
   - Control dinámico de ganancia en los conversores ADS1252.
   - Cálculo de resistividad aparente en tiempo real.

2. **Interfaz de usuario**:
   - Implementación de menús interactivos en la pantalla Nextion.
   - Gráficas dinámicas de las mediciones adquiridas.

3. **Verificación de electrodos**:
   - Circuito dedicado para verificar la conexión al suelo mediante baja corriente.
   - Indicadores visuales y mensajes en pantalla para diagnóstico.

4. **Sincronización y almacenamiento**:
   - Sincronización de tiempo con el módulo RTC DS3231.
   - Almacenamiento de proyectos y datos de sondeos en formato JSON y CSV en memoria interna.

---

## **Estructura del Proyecto**

### **Directorios y Archivos principales**

- **`SEV.3.1.ino`**: Archivo principal del firmware.
- **`Adquisicion.h`**: Control de adquisición de datos de voltaje y corriente.
- **`ADS1252.h`**: Implementación del control para los conversores ADS1252.
- **`TestElectro.h`**: Verificación del estado de los electrodos.
- **`LCDNextion.h`**: Gestión de la interfaz gráfica en la pantalla Nextion.
- **`nivelFuentes.h`**: Medición de niveles de voltaje en las fuentes de alimentación.
- **`FileSystem.h`**: Organización de proyectos y datos en memoria interna.
- **`GPS.h`**: Gestión del módulo GPS y sincronización con el RTC.

---

## **Requisitos del Sistema**

### **Hardware**

1. Raspberry Pi Pico (RP2040).
2. Conversores ADS1252 y ADS1115.
3. Amplificador INA114 y OPA4241.
4. Módulo RTC DS3231.
5. Pantalla táctil Nextion de 7 pulgadas.
6. Medidor de corriente ACS-5A.

### **Software**

1. **Arduino IDE**: Versión 1.8.x o superior.
2. Librerías necesarias:
   - `Adafruit_MCP23X17`.
   - `Adafruit_ADS1X15`.
   - `ArduinoJson`.
   - `TinyGPS`.
   - `Nextion`.
   - `LittleFS`.
   - `CSV_Parser`.

---

## **Cómo usar este código**

1. **Configurar el entorno**:
   - Instalar las librerías mencionadas en el Arduino IDE.
   - Conectar los componentes de hardware según el diseño del circuito.

2. **Cargar el firmware**:
   - Abrir el archivo `SEV.3.1.ino` en el Arduino IDE.
   - Seleccionar la placa `Raspberry Pi Pico` y el puerto correspondiente.
   - Compilar y cargar el firmware en el microcontrolador.

3. **Interacción**:
   - Usar la pantalla táctil para gestionar proyectos y visualizar datos.
   - Realizar pruebas de electrodos y sondeos mediante las opciones disponibles en la interfaz.

---

## **Contribuciones**

Este proyecto está en constante desarrollo. Si tienes sugerencias, encuentras errores o quieres contribuir, por favor abre un issue o envía un pull request.

---

## **Licencia**

Este proyecto se distribuye bajo la licencia MIT. Consulta el archivo `LICENSE` para más detalles.

---

## **Contacto**

- **Desarrollador**: Luis Alejandro Monroy  
- **LinkedIn**: [Alejandro Monroy](https://www.linkedin.com/in/alejandro-monroy-dev)  
- **Correo**: alejo_un@hotmail.com


# Proyecto SEV 3.1

Este proyecto es un sistema para la adquisición, análisis y visualización de datos obtenidos mediante sondeos eléctricos verticales (SEV). Utiliza hardware embebido, sensores y pantallas para implementar un flujo de trabajo eficiente que incluye la configuración de proyectos, la adquisición de datos y el análisis visual.

## Tabla de Contenidos
- [Componentes del Proyecto](#componentes-del-proyecto)
- [Características Principales](#características-principales)
- [Configuración de Hardware](#configuración-de-hardware)
- [Estructura del Código](#estructura-del-código)
- [Instrucciones de Uso](#instrucciones-de-uso)
- [Créditos](#créditos)

## Componentes del Proyecto
El sistema está compuesto por:

- **Sensores:** ADS1252, ADS1115.
- **Hardware de Control:**
  - Controladores I2C (Adafruit_MCP23X17).
  - RTC DS3231 para sincronización de tiempo.
- **Plataforma de visualización:** Pantalla Nextion para la interfaz gráfica.
- **Microcontrolador:** Microcontrolador RP2040 configurado para la adquisición y procesamiento de datos.
- **Entradas y salidas analógicas:** Configuradas para manejar diferentes rangos de señal.

## Características Principales

1. **Adquisición de Datos**: Utiliza un ADC ADS1252 para obtener datos precisos en tiempo real.
2. **Gestión de Proyectos y SEVs**:
   - Creación, apertura y eliminación de proyectos.
   - Administración de datos específicos de SEVs (Sondeos Eléctricos Verticales).
3. **Visualización Interactiva**:
   - Gráficos en tiempo real para los datos adquiridos.
   - Interacción mediante pantalla táctil Nextion.
4. **Pruebas Automáticas**:
   - Pruebas automáticas de electrodos para garantizar la precisión de las mediciones.
5. **Sincronización Temporal**: Sincronización con el GPS y RTC para estampar fecha y hora.

## Configuración de Hardware

### Pines Importantes
- **I2C**: SDA en pin 18, SCL en pin 19.
- **ADC**: Configuración de ADS1252 y ADS1115 según la documentación del código.
- **Controlador de Fuente**: Pines de control para polaridad, habilitación y corriente.
- **Pantalla Nextion**: Conectada a los pines de comunicación serial.

### Requisitos de Hardware
- Alimentación adecuada para el microcontrolador y periféricos.
- Una configuración estable de cableado para minimizar el ruido.

## Estructura del Código

### Archivos Principales

- **Adquisicion.h:** Gestión de la adquisición de datos y la lógica de ganancia.
- **ADS1252.h:** Configuración y manejo del ADC principal.
- **FileSystem.h:** Gestión de proyectos y SEVs en memoria flash.
- **LCDNextion.h:** Manejo de la pantalla Nextion.
- **GPS.h:** Sincronización y lectura de datos desde el GPS.
- **TestElectro.h:** Pruebas automáticas de electrodos.
- **SEV.3.1.ino:** Archivo principal del proyecto, integra todos los módulos.

### Principales Funciones
- `Adquirir`: Realiza la adquisición de datos desde los sensores.
- `proc_message`: Procesa comandos recibidos.
- `abrirProy`: Abre un proyecto desde el sistema de archivos.
- `abrirSEV`: Abre un SEV asociado a un proyecto.
- `Nex_setAdquisicion`: Actualiza la pantalla con datos adquiridos.

## Instrucciones de Uso

### Paso 1: Configuración Inicial
1. Asegúrate de que todo el hardware esté correctamente conectado.
2. Configura los pines según las definiciones en los archivos de cabecera.
3. Carga el archivo `SEV.3.1.ino` en tu microcontrolador usando el entorno de desarrollo Arduino.

### Paso 2: Gestión de Proyectos
1. Usa la pantalla Nextion para crear o abrir un proyecto.
2. Agrega nuevos SEVs o selecciona uno existente para trabajar.

### Paso 3: Adquisición de Datos
1. Ejecuta las pruebas de electrodos para verificar la conexión adecuada.
2. Inicia la adquisición de datos desde la pantalla táctil.
3. Visualiza los datos en gráficos en tiempo real.

### Paso 4: Almacenamiento y Análisis
1. Guarda los datos automáticamente en la memoria flash.
2. Revisa los datos adquiridos en gráficos detallados.

## Créditos
Este proyecto fue desarrollado por **Luis Alejandro Monroy**, ingeniero electrónico especializado en sistemas embebidos y desarrollo de hardware.

