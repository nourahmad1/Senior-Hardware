<div align="center">

# 🤖 IRFPE
### Interactive Robot for Programming Education

*A BLE-controlled educational robot built on two communicating ESP32 microcontrollers*

![Platform](https://img.shields.io/badge/platform-ESP32-blue?style=flat-square)
![Language](https://img.shields.io/badge/language-C%2B%2B-00599C?style=flat-square)
![Comm](https://img.shields.io/badge/link-BLE%20%2B%20ESP--NOW-9b59b6?style=flat-square)
![Status](https://img.shields.io/badge/status-active-success?style=flat-square)

**Senior Capstone Project — Arab American University**
Faculty of Engineering · Department of Computer Systems Engineering

</div>

---

## 👥 Team

| Role | Name |
|---|---|
| Supervisor | Dr. Sami Awad |
| Student | Nour Albzoor |
| Student | Diana Ghannam |
| Student | Misk Haneef |


---

## 📖 Overview

IRFPE is a small robot used to teach programming concepts hands-on. A companion mobile app sends commands over **Bluetooth Low Energy**, which a master ESP32 interprets and either executes locally or relays over **ESP-NOW** to a second ESP32 handling the rest of the hardware.

The firmware is split into two independent sketches, one per microcontroller:

<table>
<tr>
<td width="50%" valign="top">

### 🧠 ESP1 — Master
- BLE server (`RoboLearn`)
- Command parsing & routing
- DC motors (L298N) + drift correction
- 4× HC-SR04 ultrasonic sensors
- ST7789 TFT display
- Grid position tracking

</td>
<td width="50%" valign="top">

### 🔊 ESP2 — Slave
- ESP-NOW command receiver
- RGB LED feedback
- DFPlayer Mini audio (8 tracks)
- Reports ultrasonic readings back to ESP1

</td>
</tr>
</table>

### System Diagram

```
                    📱  Mobile App
                         │
                         │  BLE
                         ▼
              ┌────────────────────┐
              │   ESP1 — MASTER    │
              │  (RoboLearn BLE)   │
              └─────────┬──────────┘
                         │
            ESP-NOW   ◀──┴──▶   ESP-NOW
         (commands)            (sensor data)
                         │
              ┌─────────┴──────────┐
              │    ESP2 — SLAVE    │
              └────────────────────┘

   ESP1 drives:                    ESP2 drives:
   ─────────────                   ─────────────
   🔋 DC Motors (L298N)            💡 RGB LED
   📺 TFT Display (ST7789)         🔈 DFPlayer Mini (audio)
   📏 HC-SR04 ×4 (ultrasonic)
```

---

## 📂 Repository Structure

```
ESP1_Master/
└── esp1/                        ← open in Arduino IDE
    ├── esp1.ino                    Setup / loop entry point
    ├── ble.cpp / ble.h             BLE server, command parsing, routing
    ├── espnow_master.cpp/.h        ESP-NOW init + send to ESP2
    ├── espnow_master_ext.cpp/.h    ESP-NOW receive callback (sensor data)
    ├── motors.cpp / motors.h       L298N driver + drift compensation
    ├── robot.cpp / robot.h         Grid position, heading, movement
    ├── position.cpp / position.h   Ultrasonic position verification
    ├── screen.cpp / screen.h       TFT graphics (faces, numbers, sunflower)
    └── commands.h                  Shared packet structs

ESP2_Slave/
└── esp2/                        ← open in Arduino IDE
    ├── esp2.ino                    Setup / loop entry point
    ├── espnow_slave.cpp / .h       ESP-NOW init + receive callback
    ├── rgb.cpp / rgb.h             RGB LED driver
    ├── audio.cpp / audio.h         DFPlayer Mini driver
    └── commands.h                  Shared packet struct
```

> 💡 **Why the nested folders?** The Arduino IDE requires a sketch's `.ino` file to sit in a folder with the *exact same name* — that's why `esp1.ino` lives in `ESP1_Master/esp1/` rather than directly in `ESP1_Master/`.

---

## 🧠 ESP1 — Master

**Responsibilities**
- Hosts a BLE GATT server named **`RoboLearn`**
- Parses incoming commands and either:
  - executes them locally (screen graphics, robot movement), or
  - forwards them to ESP2 over ESP-NOW (LED, audio)
- Drives two DC motors via L298N, with a tunable compensation delay (`MOTOR_B_EXTRA_MS`) to correct for Motor B being weaker than Motor A — keeps the robot driving straight
- Tracks grid position (`robot.cpp`) and can self-correct using ultrasonic readings pulled from ESP2 (`position.cpp`)
- Drives a 2.4" ST7789 TFT for visual feedback — numbers, faces, sunflower growth stages

<details>
<summary><b>📡 BLE Service UUIDs</b></summary>

| | UUID |
|---|---|
| Service | `12345678-1234-1234-1234-123456789001` |
| Characteristic (write) | `12345678-1234-1234-1234-123456789002` |

</details>

<details>
<summary><b>⌨️ BLE Commands handled on ESP1</b></summary>

| Command | Action |
|---|---|
| `SUN` | Show sun graphic |
| `SLF` | Show seedling graphic |
| `LF` | Show young leaves graphic |
| `SFL` | Show full sunflower graphic |
| `HPY` | Show happy face |
| `SAD` | Show sad face |
| `D0` `D1` `D2` `D3` `D5` `D10` | Show number on screen |
| `CLR` | Clear / reset screen |
| `MF` | Move forward one cell |
| `MB` | Move backward one cell |
| `TR` | Turn right 90° |
| `TL` | Turn left 90° |

Anything else is forwarded as-is to ESP2 over ESP-NOW.

</details>

<details>
<summary><b>🔌 Pin Map — Motors (L298N) & TFT (ST7789)</b></summary>

| Motor Signal | GPIO | | TFT Signal | GPIO |
|---|---|---|---|---|
| ENA | 33 | | CS | 22 |
| ENB | 13 | | RST | 17 |
| IN1 | 25 | | DC | 16 |
| IN2 | 26 | | | |
| IN3 | 27 | | | |
| IN4 | 14 | | | |

</details>

---

## 🔊 ESP2 — Slave

**Responsibilities**
- Receives commands from ESP1 over ESP-NOW and executes them
- Drives an RGB LED for status feedback
- Drives a DFPlayer Mini for audio feedback (8 tracks)
- Reports ultrasonic sensor readings back to ESP1 on request, for grid position verification

<details>
<summary><b>⌨️ Commands handled on ESP2</b></summary>

| Command | Action |
|---|---|
| `LR` | RGB LED red |
| `LG` | RGB LED green |
| `LB` | RGB LED blue |
| `LY` | RGB LED yellow |
| `SRGB` | RGB LED off |
| `PL1`–`PL8` | Play audio track 1–8 |
| `SP` | Stop audio |

</details>

<details>
<summary><b>🔌 Pin Map — RGB LED & DFPlayer Mini</b></summary>

| RGB Signal | GPIO | | DFPlayer Signal | GPIO |
|---|---|---|---|---|
| RED | 14 | | RX (ESP2 receives) | 27 |
| GREEN | 12 | | TX (ESP2 transmits) | 26 |
| BLUE | 13 | | | |

</details>

---

## 📶 ESP-NOW Communication

ESP1 and ESP2 talk directly over ESP-NOW — no Wi-Fi router needed.

| Direction | Packet | Purpose |
|---|---|---|
| ESP1 → ESP2 | `CommandPacket { char cmd[16] }` | LED, audio, and `SENSE` requests |
| ESP2 → ESP1 | `SensorPacket { float d1, d2, d3, d4 }` | Front / back / left / right ultrasonic distances |

> ⚠️ Each board's MAC address is hard-coded as the peer address on the other side (`slaveAddress[]` in `espnow_master.cpp`). If you re-flash either board, verify the peer MAC still matches — print `WiFi.macAddress()` over Serial to check.

---

## ⚙️ Building & Flashing

**Required libraries**

| Library | Used by |
|---|---|
| `BLEDevice` / `BLEServer` / `BLEUtils` | ESP1 (built into ESP32 core) |
| `Adafruit_GFX` | ESP1 |
| `Adafruit_ST7789` | ESP1 |
| `DFRobotDFPlayerMini` | ESP2 |
| `esp_now` / `WiFi` | Both (built into ESP32 core) |

**Steps**

1. Open `ESP1_Master/esp1/esp1.ino` in Arduino IDE → select your ESP32 board → flash the master unit
2. Open `ESP2_Slave/esp2/esp2.ino` in Arduino IDE → select your ESP32 board → flash the slave unit
3. Verify the MAC address hard-coded in `espnow_master.cpp` matches the slave board
4. Pair the mobile app with the **`RoboLearn`** BLE device and start sending commands 🚀

---

## 📝 Notes

- The 4×4 grid and expected ultrasonic distances in `position.cpp` assume a **120×140 cm** track with a fixed robot orientation — recalibrate `EXPECTED[][]` if the track changes.
- `MOTOR_B_EXTRA_MS` in `motors.h` tunes forward/backward drift correction — adjust for a different chassis or motor pair.

---

<div align="center">

*Built with ❤️ as part of a senior engineering capstone project.*

</div>
