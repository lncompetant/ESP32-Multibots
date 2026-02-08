A compact, high-performance code for the **Seeed Studio XIAO ESP32**, optimized for BLE (Bluetooth Low Energy) control.  
Compatible with regular ESP32, but has connectivity issues due to a lack of an external antenna.  If a regular ESP32 is
used, it will have both Bluetooth classis and Bluetooth Low energy.

---

## ⚠️ Hardware Compatibility

The XIAO ESP32 utilizes **Bluetooth Low Energy (BLE)**. This creates specific requirements for controller pairing:

* ✅ **Supported:** Xbox Wireless Controllers (Model 1708+).
* ❌ **Incompatible:** PlayStation and Nintendo Switch Controllers (these require Bluetooth Classic). (Compatible with regular ESP32)
* 📱 **Mobile:** Possible, but results may vary and you will have to know bluepad32 really in depth.

---

## ⚙️ Required Setup & Installation

To avoid compilation errors or timing jitters, you must use the specific library versions listed below.

### 1. Board Manager URLs
Add these to your Arduino IDE Preferences (`File > Preferences`):
* `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
* `https://raw.githubusercontent.com/ricardoquesada/esp32-arduino-lib-builder/master/bluepad32_files/package_esp32_bluepad32_index.json`

### 2. Dependencies
| Library | Author | Required Version |
| :--- | :--- | :--- |
| **ESP32Servo** | Kevin Harrington | **3.0.7 or EARLIER** |
| **Bluepad32** | Ricardo Quesada | Latest |

> [!IMPORTANT]
> **DO NOT USE ESP32Servo version 3.0.8 or later.** Using a later version will not work for some reason.  Just use 3.0.7.

---

## 🚀 Quick Start

1.  **Library Check:** Ensure you have downgraded the `ESP32Servo` library in your Library Manager.
2.  **Board Selection:** Select your specific Xiao ESP32S3 or Xiao ESP32C3 board in the IDE.
3.  **Flash:** Upload the code to your robot.
4.  **Pairing:** Put your Xbox controller into sync mode. The Xiao will automatically handshake with the first available BLE controller.

## Viewing Code
* `To see the code of all my robots that use ESP32 Xiao (Or just ESP32), click view branches and select the robot you want to see code for.

