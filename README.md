# Solar Weeding Robot (Weeder)

This repository contains the software and documentation for a solar-powered autonomous weeding robot. The project supports two architectures: the original **Raspberry Pi Zero 2 W** implementation and the new, high-efficiency **ESP32-S3** firmware.

Learn more about the project at the [YouTube link here](https://www.youtube.com/watch?v=your-video-id-here).

---

##  Architectures

### 1. ESP32-S3 Implementation (Current)
The ESP32-S3 version is optimized for power efficiency, instant-on performance, and reliability. It utilizes the S3's AI acceleration for weed classification.

#### Hardware Requirements
* **Controller:** ESP32-S3-WROOM-1-N16R8 (16MB Flash, 8MB PSRAM)
* **Camera:** OV2640 or OV5640
* **Motor Drivers:** H-Bridge (e.g., DRV8833)
* **Servos:** 4x Standard PWM Servos (Lid, Tilt, Roll, Swing)

#### Setup & Installation
The ESP32 firmware is located in the `esp32_firmware/` directory and is managed via **PlatformIO**.

1. **Convert Model:**
   ```bash
   python convert_model.py
   ```
   This converts the `model_int8.tflite` into a C++ header file.

2. **Build and Flash:**
   Open the `esp32_firmware` folder in VS Code with PlatformIO and click **Upload**.

---

### 2. Raspberry Pi Implementation (Legacy)
The original implementation uses a Raspberry Pi Zero 2 W running Python.

#### Setup & Installation
1. **Improve SSH Response:**
   If SSH hangs, run the following and reboot:
   ```bash
   echo "IPQoS cs0 cs0" | sudo tee -a /etc/ssh/sshd_config
   ```
2. **Install Git:**
   ```bash
   sudo apt-get -y install git
   ```
3. **Clone Repository:**
   ```bash
   git clone https://github.com/NathanBuildsDIY/weeder
   ```
4. **Run Initial Setup:**
   ```bash
   nohup sh weeder/initial-setup.sh &
   ```
5. **Access Control Panel:**
   Connect to the **weeder** Wi-Fi hotspot and visit `http://weeder.local/run`.

---

##  Hardware & Design
* **Schematics:** See [Schematic_PartsList.pdf](./Schematic_PartsList.pdf) for full electronic details.
* **3D Files:** STL files for the body, lens arms, and covers are in the [3dPrinterFiles/](./3dPrinterFiles/) directory.
* **AI Model:** The robot uses a custom MobileNetV2-based model (`model_int8.tflite`) for real-time weed identification.

##  License
This project is licensed under the terms found in the [LICENSE](./LICENSE) file.
