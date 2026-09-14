# Mecanum Line Follower Robot

This repository contains the completed V1 prototype of an autonomous Mecanum line-following robot. It uses an Arduino Uno, a five-channel infrared sensor array, a TB6612FNG motor driver, four DC geared motors, and Mecanum wheels.

<p align="center">
  <img src="media/v1_topview.png" width="46%" alt="Top view of the V1 Mecanum line follower">
  <img src="media/v1_sideview.png" width="46%" alt="Side view of the V1 Mecanum line follower">
</p>

---

## 📂 Project Overview

The V1 prototype follows a black line on a bright surface. Its five IR sensors estimate the line position, and a PID-style controller adjusts the left and right motor groups for differential steering.

The V1 drivetrain intentionally uses grouped left/right motor control. Independent four-wheel control is implemented in the current project, [ESP32 Mecanum Car](https://github.com/leandro-3rne/mecanum-car), which adds Wi-Fi browser control, ESP-NOW remote control, an OLED interface, and an updated PID line-following mode.

---

## 🎥 V1 Demonstration

<p align="center">
  <img src="media/line_following_v1.gif" width="700" alt="V1 robot following a line">
</p>

---

## 🔩 Hardware

| Component                 | Purpose                      |
| ------------------------- | ---------------------------- |
| Arduino Uno               | Main controller               |
| TB6612FNG                 | Dual-channel motor driver    |
| 5-channel IR sensor array | Line position detection      |
| 4× yellow TT DC motors    | Locomotion                   |
| 4× Mecanum wheels         | Robot platform               |
| 4× AA NiMH batteries      | Motor supply                 |
| USB power bank            | Arduino supply               |

The Arduino and the motors use separate power sources. Both sides share a common ground so that the motor-control signals and sensor readings use the same reference.

```text
USB power bank ──▶ Arduino Uno ──▶ Sensor array + TB6612FNG logic

4× AA NiMH ──────▶ TB6612FNG motor supply ──▶ DC motors
```

---

## 👁️ Line Detection

The sensors are connected to `A0`–`A4` from left to right. A bright-floor reference is used to estimate the strength of the black line:

$$
s_i = \max(0, r_{floor_i} - r_i)
$$

The weighted average of the five sensor values provides the line position. The controller then applies a PID-style correction to the two motor groups:

$$
p = \frac{\sum_{i=1}^{5} w_i s_i}{\sum_{i=1}^{5} s_i}
$$

$$
u = K_Pe + K_I\sum e + K_D(e-e_{previous})
$$

```text
Sensor:  S1     S2     S3     S4     S5
Weight: -2.8   -1.0    0.0   +1.0   +2.8
```

The current tuning values are:

| Parameter   | Value |
| ----------- | ----- |
| $K_P$       | `130` |
| $K_I$       | `0`   |
| $K_D$       | `50`  |
| Base speed  | `140` |

If all sensors lose the line, the robot stops until the line is detected again.

---

## 📁 Repository Layout

```text
.
├── media/                         # V1 photos, diagram, and demonstration GIF
├── scripts/v1/
│   └── pid-line-follower-v1/
│       └── pid-line-follower-v1.ino
├── LICENSE
└── README.md
```

---

## 🔗 Current Development

For the current hardware and firmware, use [ESP32 Mecanum Car](https://github.com/leandro-3rne/mecanum-car), which provides wireless control modes and full Mecanum movement.

---

## 📄 License

This project is licensed under the MIT License. See [`LICENSE`](LICENSE) for the full license text.
