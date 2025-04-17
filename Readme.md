
# Geophysical Sounding Device

This repository contains the firmware and technical documentation for a complete embedded system designed to perform **Vertical Electrical Soundings (VES)** and **Electrical Resistivity Tomography (ERT)**. The system integrates a data acquisition core, power control electronics, electrode verification, GPS, and a touch-enabled Human-Machine Interface (HMI).

---

## 🧭 Project Overview

This device was developed with the goal of providing a reliable, high-precision solution for field geophysical studies. It enables geoscientists, hydrogeologists, and environmental engineers to acquire subsurface resistivity data in a streamlined and automated manner.

It includes features that ensure:
- Operator safety during high-voltage operation.
- Automatic validation of electrode-ground connectivity.
- Real-time visualization and structured data storage.
- Embedded control of current injection and signal acquisition.

---

## ⚙️ Main Features

### 🧠 Embedded System

- **Dual-core microcontroller with ARM Cortex-M0+ architecture**, ideal for parallel tasks such as data acquisition and HMI interaction.
- **Sampling rate: 1000 samples/second**, processed by:
  - **Notch filter (60 Hz)** to eliminate power-line interference.
  - **Low-pass digital filter** for signal conditioning.
  - **Linear regression** to calculate spontaneous potential (SP) and its slope for compensation.

### 📈 Signal Acquisition and Processing

- **Analog front-end components**:
  - **ADS1252 (24-bit ADC)** for voltage and current measurements from the subsurface.
  - **ADS1115 (16-bit ADC)** for system voltage monitoring and electrode check.
  - **INA114** for precise differential signal amplification.
  - **ACS-5A** current sensor for injected current measurement.

- **Automatic gain selection** based on signal levels to optimize resolution.

### ⚡ Power Injection Control

- High-voltage power supply controlled via **UART at 9600 bps**.
- The system sends commands to configure maximum output voltage and current.
- **H-bridge with electromechanical relays** to alternate polarity and control ON/OFF state.
- Working cycle: `OFF – HV – OFF – HV – OFF` (1 second per stage).

### 🧪 Electrode Test

- Safe, low-current test is executed **before** each sounding to ensure electrodes are properly connected to the ground.
- Faulty connections are detected automatically and visual feedback is provided on the HMI.
- The test system uses a secondary **ADS1115** and relay-controlled paths to validate all four electrodes (A, B, M, N).

### 🖥️ Human-Machine Interface

- **Nextion 7” touchscreen display** is used for all user interactions.
- Menus for:
  - Creating/selecting projects and soundings.
  - Viewing voltage trends and temperatures.
  - Monitoring internal voltages: 12V battery, 5V, 3.3V, and 3.6V RTC.
  - Graphing apparent resistivity in real time.

### 🛰️ GPS and Time Sync

- Integrated **GPS + DS3231 RTC** for geolocation and timestamping of soundings.
- System syncs both microcontroller and RTC clock on startup using GPS time.

---

## 📁 Repository Structure

```bash
geophysical-sounding-device/
├── firmware/
│   ├── Adquisicion.h         # Acquisition logic and measurement routines
│   ├── ADS1252.h             # ADC interface and signal filtering
│   ├── Consola.h             # Serial communication and command handling
│   ├── FileSystem.h          # Project and sounding file management (LittleFS)
│   ├── filtro.h              # Regression and digital filters
│   ├── FuenteAB.h            # High voltage source control via UART
│   ├── GPS.h                 # GPS reading and RTC sync
│   ├── LCDNextion.h          # HMI Nextion screen manager
│   ├── nivelFuentes.h        # Power supply voltage monitoring
│   ├── SEV.3.0.h             # Main initialization routines
│   ├── SEV.3.1.ino           # Main firmware file
│   ├── TestElectro.h         # Electrode test logic
├── docs/
│   └── blog.md               # Technical blog post (in Spanish)
└── README.md                 # This file
```

---

## 🚀 System Workflow

1. **Startup**
   - Initializes ADCs, touchscreen, filesystem, RTC, and GPS.
   - Scans I2C bus and verifies sensor presence.

2. **Electrode Check**
   - Injects a low current using controlled relay paths.
   - Measures resistance of electrodes A, B, M, N.
   - Displays connection status and alerts if any contact is poor.

3. **Sounding Execution**
   - Alternates polarities and injection stages (OFF–HV–OFF–HV–OFF).
   - Samples data at 1000 Hz during each measurement stage.
   - Applies regression and filtering to determine SP and voltage deltas.

4. **Calculation of Apparent Resistivity**
   - Uses collected voltage and current data to compute Ra.
   - Displays result on graph and saves it to internal memory.

5. **Repeat or End**
   - Operator can repeat the measurement or proceed to the next aperture.

---

## ✅ Project Status

- ✅ Hardware complete and field-tested.
- ✅ Firmware stable and well structured.
- 🟡 Documentation (user guide and PC software) under development.
- 🟡 Automated data export functionality in progress.

---

## 📜 License

This project is licensed under the **MIT License**. See `LICENSE` for details.

---

## 👨‍💻 Author

Developed by **Luis Alejandro Monroy**

- 📫 [alejo_un@hotmail.com](mailto:alejo_un@hotmail.com)
- 🔗 [LinkedIn](https://www.linkedin.com/in/alejandro-monroy-dev)
- 🌐 [Portfolio](https://www.alejandro-monroy.com)
- 🧪 [GitHub](https://github.com/alejomonroy)

---

If you find this project useful or would like to contribute, feel free to fork, open issues or contact me. Field testing, robustness, and signal fidelity have been the main drivers of this design.
