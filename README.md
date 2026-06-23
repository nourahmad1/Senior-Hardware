# IRFPE – Interactive Robot for Programming Education

Firmware for a two-ESP32 educational robot built as a senior capstone project at the Arab American University, Faculty of Engineering, Department of Computer Systems Engineering.

**Team:** Nour Albzoor, Diana Ghannam, Misk Haneef
**Supervisor:** Dr. Sami Awad

## Overview

The robot is controlled from a companion mobile app over Bluetooth Low Energy (BLE). Commands are parsed by a master ESP32 and relayed over ESP-NOW to a second ESP32 that drives the outputs not directly attached to the master. The system is split into two independent firmware projects, one per microcontroller.

| | ESP1 (Master) | ESP2 (Slave) |
|---|---|---|
| Role | BLE server, command router, robot logic | Peripheral I/O execution |
| Peripherals | DC motors (L298N), TFT display (ST7789), 4x HC-SR04 ultrasonic sensors | RGB LED, DFPlayer Mini (audio) |
| Link to phone | BLE | — |
| Link to other ESP32 | ESP-NOW (sends commands, receives sensor data) | ESP-NOW (receives commands, sends sensor data) |

```
 Mobile App
     │  BLE
     ▼
 ┌─────────┐   ESP-NOW (commands)   ┌─────────┐
 │  ESP1   │ ──────────────────────▶│  ESP2   │
 │ Master  │◀────────────────────── │  Slave  │
 └─────────┘   ESP-NOW (sensors)    └─────────┘
   │  │  │                              │  │
   │  │  └─ TFT Display (ST7789)        │  └─ RGB LED
   │  └──── HC-SR04 x4 (ultrasonic)     └──── DFPlayer Mini (audio)
   └─────── DC Motors (L298N)
```

## Repository Structure

```
ESP1_Master/
└── esp1/                      ← open this folder's .ino in Arduino IDE
    ├── esp1.ino                  Setup/loop entry point
    ├── ble.cpp / ble.h           BLE server, command parsing, routing
    ├── espnow_master.cpp/.h      ESP-NOW init + sending commands to ESP2
    ├── espnow_master_ext.cpp/.h  ESP-NOW receive callback for sensor data
    ├── motors.cpp / motors.h     L298N motor driver + drift compensation
    ├── robot.cpp / robot.h       Grid position, heading, high-level movement
    ├── position.cpp / position.h Ultrasonic-based position verification
    ├── screen.cpp / screen.h     ST7789 TFT graphics (faces, numbers, sunflower stages)
    └── commands.h                Shared packet structs (commands + sensor data)

ESP2_Slave/
└── esp2/                      ← open this folder's .ino in Arduino IDE
    ├── esp2.ino                  Setup/loop entry point
    ├── espnow_slave.cpp / .h     ESP-NOW init + command receive callback
    ├── rgb.cpp / rgb.h           RGB LED driver
    ├── audio.cpp / audio.h       DFPlayer Mini driver
    └── commands.h                Shared packet struct (commands)
```

> **Note:** the Arduino IDE requires a sketch's `.ino` file to live in a folder with the *exact same name* as the file (minus the extension). That's why `esp1.ino` sits inside `ESP1_Master/esp1/` rather than directly inside `ESP1_Master/`.

## ESP1 — Master

Responsibilities:
- Hosts a BLE GATT server named **`RoboLearn`** that the mobile app connects to.
- Parses incoming BLE command strings and either:
  - executes them locally (TFT screen graphics, robot movement), or
  - forwards them to ESP2 over ESP-NOW (RGB LED, audio).
- Drives two DC motors through an L298N driver, with a compensation delay (`MOTOR_B_EXTRA_MS`) to correct for Motor B being weaker than Motor A, preventing drift on straight-line moves.
- Tracks the robot's position on a 4x4 grid (`robot.cpp`) and can verify/correct its position using ultrasonic distance readings pulled from ESP2 (`position.cpp`).
- Drives a 2.4" ST7789 TFT display to show numbers, emoji-style faces, and sunflower growth stages as educational feedback.

### BLE Service

| | UUID |
|---|---|
| Service | `12345678-1234-1234-1234-123456789001` |
| Characteristic (write) | `12345678-1234-1234-1234-123456789002` |

### BLE Commands handled directly on ESP1

| Command | Action |
|---|---|
| `SUN` | Show sun graphic |
| `SLF` | Show seedling graphic |
| `LF` | Show young leaves graphic |
| `SFL` | Show full sunflower graphic |
| `HPY` | Show happy face |
| `SAD` | Show sad face |
| `D0`,`D1`,`D2`,`D3`,`D5`,`D10` | Show number on screen |
| `CLR` | Clear / reset screen |
| `MF` | Move forward one cell |
| `MB` | Move backward one cell |
| `TR` | Turn right 90° |
| `TL` | Turn left 90° |

Any other command string is forwarded as-is to ESP2 over ESP-NOW.

### Motor pins (L298N)

| Signal | GPIO |
|---|---|
| ENA | 33 |
| ENB | 13 |
| IN1 | 25 |
| IN2 | 26 |
| IN3 | 27 |
| IN4 | 14 |

### TFT pins (ST7789, SPI)

| Signal | GPIO |
|---|---|
| CS | 22 |
| RST | 17 |
| DC | 16 |

## ESP2 — Slave

Responsibilities:
- Receives command strings from ESP1 over ESP-NOW and executes them.
- Drives an RGB LED.
- Drives a DFPlayer Mini MP3 module for audio feedback (8 tracks).
- (Per `espnow_master_ext`/`position.cpp` on ESP1) reports back ultrasonic sensor readings on request so ESP1 can verify the robot's position on the grid.

### Commands handled on ESP2

| Command | Action |
|---|---|
| `LR` | RGB LED red |
| `LG` | RGB LED green |
| `LB` | RGB LED blue |
| `LY` | RGB LED yellow |
| `SRGB` | RGB LED off |
| `PL1`–`PL8` | Play audio track 1–8 |
| `SP` | Stop audio |

### RGB LED pins

| Signal | GPIO |
|---|---|
| RED | 14 |
| GREEN | 12 |
| BLUE | 13 |

### DFPlayer Mini pins

| Signal | GPIO |
|---|---|
| RX (ESP2 receives on) | 27 |
| TX (ESP2 transmits on) | 26 |

## ESP-NOW Communication

ESP1 and ESP2 talk directly over ESP-NOW (no Wi-Fi router needed):

- **ESP1 → ESP2:** `CommandPacket { char cmd[16] }` — drives RGB LED, audio, and a `SENSE` request for sensor data.
- **ESP2 → ESP1:** `SensorPacket { float d1, d2, d3, d4 }` — front/back/left/right ultrasonic distances, used by ESP1's position-verification logic.

Each board's MAC address must be hard-coded as the peer address on the other side (`slaveAddress[]` in `espnow_master.cpp` on ESP1; ESP2 listens via its registered receive callback). If you re-flash either board, double-check the peer MAC address still matches.

## Building

Both sketches are built with the Arduino IDE (or `arduino-cli`) targeting an ESP32 dev board.

Required libraries:
- `BLEDevice` / `BLEServer` / `BLEUtils` (built into the ESP32 Arduino core)
- `Adafruit_GFX`
- `Adafruit_ST7789`
- `DFRobotDFPlayerMini`
- `esp_now` / `WiFi` (ESP32 Arduino core)

Steps:
1. Open `ESP1_Master/esp1/esp1.ino` in Arduino IDE, select your ESP32 board, and flash the master unit.
2. Open `ESP2_Slave/esp2/esp2.ino` in Arduino IDE, select your ESP32 board, and flash the slave unit.
3. Confirm the MAC address hard-coded in `espnow_master.cpp` matches the actual slave board (`WiFi.macAddress()` printed over Serial can help verify this).
4. Pair the mobile app with the `RoboLearn` BLE device and start sending commands.

## Notes

- The 4x4 grid layout and expected ultrasonic distances used in `position.cpp` assume a 120x140 cm track with the robot in a fixed orientation; recalibrate `EXPECTED[][]` if the track dimensions change.
- `MOTOR_B_EXTRA_MS` in `motors.h` is a tunable constant for fixing forward/backward drift; adjust if a different chassis or motor pair is used.
