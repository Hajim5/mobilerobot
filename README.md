# 🤖 Mobile Arm Robot (Keyestudio KS0560)

A mobile robotic platform developed using the **Keyestudio KS0560 4WD Mecanum Robot**. This project combines omnidirectional mobile navigation with a robotic arm, enabling autonomous movement, object manipulation, and sensor integration.

The project is developed using **Visual Studio Code** with **PlatformIO** for easier project management, library handling, and firmware development.

---

## 📖 Overview

This repository contains the source code and documentation for a mobile arm robot capable of:

- Omnidirectional movement using mecanum wheels
- Line tracking navigation
- 90° precision turning
- Object manipulation using a robotic arm
- Sensor-based autonomous navigation
- Manual control using an IR remote
- Modular code structure for future expansion

---

## ✨ Features

- 🚗 Mecanum wheel movement
  - Forward
  - Reverse
  - Left strafe
  - Right strafe
  - Rotation

- 🤖 Robotic arm control

- 📍 Line tracking navigation

- 📏 Ultrasonic obstacle detection

- 🎮 IR remote control

- 💡 LED status indication

- 🔧 Easy to modify and expand

---

## 🛠 Hardware

- Keyestudio KS0560 4WD Mecanum Robot
- Keyestudio UNO PLUS Board
- Line Tracking Sensor
- Ultrasonic Sensor
- Servo Motor
- IR Receiver
- DC Motors
- Mecanum Wheels

---

## 💻 Software

| Software | Version |
|----------|---------|
| Visual Studio Code | Latest |
| PlatformIO | Latest |
| Arduino Framework | AVR |
| C++ | C++11+ |

---

## 📁 Project Structure

```text
Mobile-Arm-Robot/
│
├── include/            # Header files
├── lib/                # Custom libraries
├── src/                # Main source code
├── test/               # Unit tests
├── platformio.ini      # PlatformIO configuration
└── README.md
```

---

## 🚀 Getting Started

### 1. Clone the repository

```bash
git clone https://github.com/yourusername/mobile-arm-robot.git
```

### 2. Open with VS Code

Open the project folder using **Visual Studio Code**.

### 3. Install PlatformIO

Install the **PlatformIO IDE** extension from the VS Code Marketplace.

### 4. Build

```bash
PlatformIO: Build
```

or

```bash
pio run
```

### 5. Upload

```bash
PlatformIO: Upload
```

or

```bash
pio run --target upload
```

---

## 📦 Libraries

Example libraries used in this project:

- Servo
- IRremote
- NewPing
- Keyestudio motor libraries
- Custom mecanum drive library

---

## ⚙ Supported Functions

| Function | Status |
|----------|--------|
| Forward Movement | ✅ |
| Reverse Movement | ✅ |
| Left/Right Strafe | ✅ |
| 90° Rotation | ✅ |
| Line Following | ✅ |
| Ultrasonic Detection | ✅ |
| IR Remote Control | ✅ |
| Robotic Arm Control | 🚧 In Progress |
| Autonomous Navigation | 🚧 In Progress |

---

## 📸 Demo

Images and videos will be added in future updates.

```
/images
/videos
```

---

## 🔄 Future Improvements

- PID motion control
- Encoder-based localization
- Odometry
- Inverse kinematics for robotic arm
- ROS integration
- Computer vision
- Camera-based object detection
- SLAM
- Wi-Fi control
- Mobile application
- AI-assisted object manipulation

---

## 📚 References

- Keyestudio KS0560 Documentation
- PlatformIO Documentation
- Arduino Documentation

---

## 📄 License

This project is released under the MIT License.

---

## 👤 Author

**Ahmad Hazim**

GitHub: https://github.com/yourusername

---

### ⭐ If you find this project useful, consider giving it a star!
````


Task Group 5 (Data Titan)

Group 5: The Outdoor Campus Delivery Rover

Mission Brief: A rugged robot that delivers food across
a university campus, dealing with outdoor elements
and crosswalks.
Key Behaviors for the FSM:

Start: Idle/Docked at the student union.
Trigger: App order received, transitions to GPS
Navigation.

Crosswalk Protocol: When it reaches a road
(detected by map coordinates), it transitions to
Waiting for Walk Signal. It uses a camera to look for
the green pedestrian light before transitioning back
to GPS Navigation.

Weather Failsafe: If its moisture sensor detects
heavy rain, it transitions to Seeking Shelter (driving
to the nearest covered waypoint) and waits there
until the rain stops.

Delivery: Arrives at the dorm and enters a "Waiting
for PIN" state. Once the student inputs the correct
PIN via the app, the rover opens its hatch and
signals the dorm's automated portico arm. The arm
safely extracts the food package and hands it to
the student. Once the hatch area is clear, the rover
seals itself and goes home
