# ESP32C3-Rainmaker-Based-IoT-System-for-Monitoring-Temperature-and-Humidity
An IoT system powered by ESP32, DHT11, and ESP RainMaker for real-time temperature and humidity tracking. It offers cloud integration, temperature notifications, OTA updates, and efficient task handling with FreeRTOS.
![image](https://github.com/user-attachments/assets/296f4d96-8481-4253-8529-ab91f72155dc)

### Technologies Used:
- **ESP32-C3**: Microcontroller used to operate the IoT device.
- **ESP-IDF**: Development framework utilized for building the project.
- **C++**: Programming language used for the implementation.
- **FreeRTOS**: RTOS employed for multitasking and task scheduling.
- **DHT11**: Sensor utilized to measure temperature and humidity levels.
- **ESP RainMaker**: Cloud platform used for managing the device and enabling remote monitoring.
- **Google Assistant**: Voice assistant interface used for interacting with the device.

### Project Features:
- **Continuous monitoring of temperature and humidity in real-time.**
- **ESP RainMaker-based cloud integration for seamless device connectivity and management.**
- **Control via voice commands using Google Assistant.**
- **Task management based on FreeRTOS for efficient multi-tasking.**
- **OTA Firmware Updates.**

  ## 🔧 Hardware Setup

### Required Components:
| Component           | Quantity |
|---------------------|----------|
| ESP32-C3 Dev Board  | 1        |
| DHT11 Sensor        | 1        |
| Red LED             | 1        |
| Jumper Wires        | Several  |
| Acrylic Sheets      | 6        |

### Wiring Diagram:
```plaintext
ESP32-C3 GPIO2  ---> DHT11 Sensor (Data Pin)
ESP32-C3 GPIO10 ---> Red LED (Anode)
ESP32-C3 GND     ---> DHT11 GND, LED Cathode
ESP32-C3 3V3     ---> DHT11 VCC
```
![image](https://github.com/user-attachments/assets/e6ce27c7-7d10-49f8-af57-e81cc700eb49)

## 📥 Installation

### Prerequisites:
- **ESP32 IDF v4.4** or later
- **Visual Studio Code** with ESP-IDF extension installed
- **ESP RainMaker CLI** for managing cloud configurations

### Setup Steps:
1. **Clone the Repository:**
   ```bash
   git clone https://github.com/<YOUR_USERNAME>/IoT-Temperature-Humidity-Monitoring-System.git
   cd IoT-Temperature-Humidity-Monitoring-System
   ```
2. **Install Dependencies:**
    ```bash
    pip install -r requirements.txt
    ```
3. **Configure ESP RainMaker CLI:**
    Install the ESP RainMaker CLI:
    ```bash
    pip install esp-rainmaker-cli
    ```
    
    Log in to RainMaker:
    ```bash
   esp-rainmaker login
    ```

 4.**Build & Flash the Firmware: Set the target to ESP32 and build:**
    ```bash
    idf.py set-target esp32c3
    idf.py build
    idf.py -p PORT flash monitor  # Replace PORT with your device's COM port
    ```

 5.**Mobile App Setup:**
   *Download and install the ESP RainMaker mobile app (available for Android/iOS).*
   *Follow the on-screen instructions within the app to connect your device to the RainMaker platform.*

## ⚙️ How to Use the Project

### Sensor Monitoring
- The system collects temperature and humidity data from the DHT11 sensor.
- The data is transmitted to the ESP RainMaker cloud for remote monitoring.

### Voice Control via Google Assistant
By linking your RainMaker account, you can control the system and retrieve data using Google Assistant.

**Example voice commands:**
- "Hey Google, can you turn on the LED Switch?"
- "Hey Google, can you now turn off the LED."

### LED Alerts
- The Red LED will activate when the temperature or humidity exceeds a set threshold.
### 📡 Over-The-Air (OTA) Updates
To update the firmware remotely:

- Use the RainMaker CLI to upload new firmware:
  ```bash
  python scripts/ota_update.py --url <FIRMWARE_URL>
  ```
