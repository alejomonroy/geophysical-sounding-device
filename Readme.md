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

