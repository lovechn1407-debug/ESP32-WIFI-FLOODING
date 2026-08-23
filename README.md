
<![CDATA[
<p align="center">
  <img src="banner.png" alt="ESP32 SSID Spammer Banner" width="100%"/>
</p>

<h1 align="center">📡 ESP32 SSID Spammer</h1>

<p align="center">
  <b>A raw 802.11 beacon frame flooder for the ESP32 — built with Arduino Framework & ESP-IDF</b><br/>
  Simulate dozens of fake Wi-Fi networks in real time. For educational and testing purposes only.
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Platform-ESP32-blue?style=for-the-badge&logo=espressif" />
  <img src="https://img.shields.io/badge/Framework-Arduino-teal?style=for-the-badge&logo=arduino" />
  <img src="https://img.shields.io/badge/Language-C%2B%2B-orange?style=for-the-badge&logo=cplusplus" />
  <img src="https://img.shields.io/badge/License-MIT-green?style=for-the-badge" />
</p>

---

## 📋 Table of Contents

- [Overview](#-overview)
- [Features](#-features)
- [Project Structure](#-project-structure)
- [Hardware Requirements](#-hardware-requirements)
- [Prerequisites](#-prerequisites)
- [Installation & Setup](#-installation--setup)
- [Uploading to ESP32](#-uploading-to-esp32)
- [Customization](#-customization)
- [Serial Monitor Output](#-serial-monitor-output)
- [How It Works](#-how-it-works)
- [Troubleshooting](#-troubleshooting)
- [Legal Disclaimer](#-legal-disclaimer)

---

## 🔍 Overview

**ESP32 SSID Spammer** is a beacon frame generator that uses the ESP32's raw Wi-Fi packet injection capability (`esp_wifi_80211_tx`) to broadcast multiple fake Wi-Fi networks simultaneously — without connecting to any router or access point.

Every device scanning for Wi-Fi nearby will see all the spoofed SSIDs appear in their network list.

---

## ✨ Features

- 📶 Broadcasts **20+ fake Wi-Fi networks** simultaneously
- 🔐 Supports both **Open** and **WPA2** security simulation
- 🔄 **Automatic channel selection** (random start on channels 1–14)
- 🎭 **Unique spoofed MAC addresses** per network (incremental per SSID)
- 🧠 SSID list stored in **flash memory (PROGMEM)** to save RAM
- 📊 **Real-time packet rate** output via Serial Monitor
- ⚙️ Clean **object-oriented C++ library** architecture

---

## 📁 Project Structure

```
ESP32_SSID_Spammer/
├── ESP32_SSID_Spammer.ino   # Main entry point (setup + loop)
├── spammer.hpp              # Class declaration & documentation
├── spammer.cpp              # Core logic: beacon construction & injection
├── banner.png               # Repository banner
└── README.md                # This file
```

---

## 🔧 Hardware Requirements

| Component | Details |
|:----------|:--------|
| **Microcontroller** | ESP32 (any variant: DevKit V1, WROOM-32, NodeMCU-32S, etc.) |
| **Connection** | Micro-USB or USB-C **data cable** (not charge-only!) |
| **External Components** | ❌ None required — only the ESP32 board |

> ✅ No wiring, breadboard, or sensors needed. The project uses the ESP32's built-in Wi-Fi radio.

---

## 📦 Prerequisites

### 1. Install Arduino IDE
Download and install **Arduino IDE 2.x** from:
👉 https://www.arduino.cc/en/software

### 2. Add ESP32 Board Support

1. Open Arduino IDE → **File** → **Preferences**
2. In *"Additional Boards Manager URLs"*, add:
   ```
   https://espressif.github.io/arduino-esp32/package_esp32_index.json
   ```
3. Go to **Tools** → **Board** → **Boards Manager**
4. Search for **`esp32`** by *Espressif Systems*
5. Click **Install** (use version **3.x** or newer)

---

## 🚀 Installation & Setup

### Step 1 — Download / Clone the Repository

**Option A — Clone with Git:**
```bash
git clone https://github.com/YOUR_USERNAME/ESP32_SSID_Spammer.git
```

**Option B — Download ZIP:**
- Click the green **`Code`** button → **Download ZIP**
- Extract the folder to your desired location

### Step 2 — Open the Project

1. Open **Arduino IDE**
2. Go to **File** → **Open**
3. Navigate to the extracted folder and open **`ESP32_SSID_Spammer.ino`**

> ⚠️ Make sure `spammer.cpp` and `spammer.hpp` are in the **same folder** as the `.ino` file. Arduino IDE will automatically include all `.cpp` and `.h` files in the sketch folder.

### Step 3 — Select Your Board & Port

Go to **Tools** and configure:

| Setting | Value |
|:--------|:------|
| **Board** | `ESP32 Dev Module` (or your specific variant) |
| **CPU Frequency** | `240MHz (WiFi/BT)` |
| **Flash Frequency** | `80MHz` |
| **Upload Speed** | `921600` |
| **Port** | Your ESP32's COM port (e.g., `COM3`, `COM4`) |

> 💡 To find your COM port: **Device Manager** → **Ports (COM & LPT)** → look for *Silicon Labs CP210x* or *CH340*.

---

## ⚡ Uploading to ESP32

1. Plug your ESP32 into your PC via USB
2. Click the **Upload** button (➡️ arrow icon) in Arduino IDE
3. If the upload gets **stuck at `Connecting...`**:
   - Press and **hold the BOOT button** on the ESP32
   - Keep holding until you see `Uploading...` in the log
   - Then release
4. Wait for `Done uploading` message

---

## 📡 Serial Monitor Output

After uploading, open the **Serial Monitor**:
- **Tools** → **Serial Monitor** (or `Ctrl + Shift + M`)
- Set baud rate to: **`115200`**

You will see:
```
Starting SSID Spammer...
Spamming initiated!
780 packets/s
810 packets/s
792 packets/s
```

On nearby phones/laptops, open Wi-Fi settings and scan — you will see networks like:
```
WIFI FLOODING STARTED 01
WIFI FLOODING STARTED 02
WIFI FLOODING STARTED 03
...
WIFI FLOODING STARTED 20
```

---

## ⚙️ Customization

### Change the SSID List

Open `spammer.cpp` and edit the `targetSSIDs` array at the top of the file:

```cpp
const char targetSSIDs[] PROGMEM = {
  "My Custom Network 1\n"
  "Free Airport WiFi\n"
  "FBI Surveillance Van\n"
  "Your Custom SSID Here\n"
  // Add more lines...
};
```

> **Rules:**
> - Each entry must be **unique** (duplicate names will be merged into one by Wi-Fi scanners)
> - Each name must end with `\n`
> - Maximum **32 characters** per SSID name

### Enable WPA2 Simulation

In `ESP32_SSID_Spammer.ino`, change the constructor argument:

```cpp
// false = Open networks (default)
// true  = WPA2 protected (shows lock icon on devices)
spammer spmr(true);
```

---

## 🔬 How It Works

```
┌─────────────────────────────────────────────────────────────┐
│                    ESP32 SSID Spammer                       │
│                                                             │
│  1. Build 109-byte 802.11 Beacon Frame                      │
│  2. Set SSID name + length into frame bytes [37–69]         │
│  3. Set spoofed MAC address into bytes [10–21]              │
│  4. Set current Wi-Fi channel into byte [82]                │
│  5. Call esp_wifi_80211_tx() → inject raw packet            │
│  6. Repeat for each SSID, incrementing MAC address          │
│  7. Log packets/sec to Serial every 3 seconds               │
└─────────────────────────────────────────────────────────────┘
```

| Component | Role |
|:----------|:-----|
| `esp_wifi_80211_tx(WIFI_IF_STA, ...)` | Injects raw 802.11 frames directly into the air |
| `PROGMEM` | Stores SSID strings in flash to save heap RAM |
| MAC Spoofing | `_macAddress[5]` incremented per SSID for uniqueness |
| Channel Hopping | Randomly selected channel 1–14 on startup |
| Beacon Interval | `0x03E8` = 1000 TU (~1 second), standard AP behavior |

---

## 🛠️ Troubleshooting

| Problem | Solution |
|:--------|:---------|
| Upload stuck at `Connecting...` | Hold the **BOOT** button while uploading |
| `cannot convert 'esp_interface_t' to 'wifi_interface_t'` | Use ESP32 Arduino Core **v3.x** — this project is patched for it. Make sure your board package is up to date |
| Only 1 hotspot visible in scan | Ensure all SSIDs in `spammer.cpp` have **unique names** |
| No COM port visible | Install the CP2102 or CH340 USB driver for your ESP32 board |
| `0 packets/s` in Serial Monitor | Try a different Wi-Fi channel or check `WiFi.mode(WIFI_MODE_STA)` is set |

---

## ⚖️ Legal Disclaimer

> **This project is strictly for educational, research, and controlled testing purposes.**
>
> Transmitting fake beacon frames on shared or public Wi-Fi channels may:
> - Violate the **FCC Part 15 regulations** (US)
> - Violate **OFCOM regulations** (UK)
> - Violate **IT Act Section 66** (India)
> - Constitute illegal radio interference under your local telecommunications law
>
> **Always test in controlled, private environments or inside RF shielding enclosures.**
> The author and contributors are not responsible for any misuse of this code.

---

## 📄 License

This project is licensed under the **MIT License** — see the [LICENSE](LICENSE) file for details.

---

<p align="center">Made with ❤️ for the ESP32 & Arduino community</p>
]]>
