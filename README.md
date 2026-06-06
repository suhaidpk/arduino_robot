# Obstacle Avoiding Multi-function Robot 🤖

A 4-wheeled Arduino robot with three switchable operating modes: **autonomous obstacle avoidance**, **Bluetooth joystick control**, and **Bluetooth voice control**.


---

## Features

| Mode | Description |
|------|-------------|
| **Obstacle Avoidance** | Servo-mounted ultrasonic sensor scans left & right; robot automatically steers away from obstacles |
| **Bluetooth Control** | Drive with a smartphone app using on-screen joystick buttons |
| **Voice Control** | Issue spoken commands via a Bluetooth voice-control app |

---

## Hardware

| Component | Qty | Notes |
|-----------|-----|-------|
| Arduino UNO | 1 | Main microcontroller |
| L293D Motor Driver Shield | 1 | Adafruit-compatible; stacks onto UNO |
| HC-SR04 Ultrasonic Sensor | 1 | Echo → A0, Trig → A1 |
| SG90 Servo Motor | 1 | Mounts sensor for L/R scanning; signal → Pin 10 |
| HC-05 Bluetooth Module | 1 | Serial comms with smartphone |
| DC Gear Motors (TT) | 4 | Connected to M1–M4 on shield |
| Robot Wheels | 4 | Press-fit onto gear motor shafts |
| 18650 Li-Ion Battery Holder (2-cell) | 1 | Powers the whole system |
| 18650 Li-Ion Batteries | 2 | ~7.4 V nominal |
| Foam board / Cardboard | — | Chassis material |
| Jumper wires | — | Various lengths |

---

## Wiring / Circuit

```
HC-SR04
  VCC  → 5 V  (shield)
  GND  → GND
  TRIG → A1
  ECHO → A0

SG90 Servo
  Signal → Pin 10  (via shield servo header)
  VCC    → 5 V
  GND    → GND

HC-05 Bluetooth
  VCC → 5 V
  GND → GND
  TX  → RX (Arduino / shield serial)
  RX  → TX (Arduino / shield serial)  ← disconnect before uploading!

Motors
  Front-left  → M1
  Rear-left   → M2
  Front-right → M3
  Rear-right  → M4

Battery Holder → EXT_PWR terminals on motor shield (7–12 V)
```

> ⚠️ **Important:** Always disconnect the Bluetooth **RX/TX** jumper wires before uploading firmware, then reconnect afterwards.

---

## Software Setup

### 1. Install the AFMotor Library

Download the Adafruit AF Motor library (v1) and install it in the Arduino IDE:

- Arduino IDE → **Sketch → Include Library → Add .ZIP Library…**
- Or install via **Library Manager** — search `Adafruit Motor Shield`

### 2. Open the Sketch

```
src/robot/robot.ino
```

### 3. Select Operating Mode

Inside `loop()`, uncomment **exactly one** function:

```cpp
void loop() {
  Obstacle();           // Mode 1 – autonomous avoidance
  // Bluetoothcontrol();  // Mode 2 – joystick app
  // voicecontrol();      // Mode 3 – voice app
}
```

### 4. Upload

- Board: **Arduino UNO**
- Disconnect Bluetooth TX/RX → Upload → Reconnect

---

## Bluetooth Apps

### Joystick Control
Use any Bluetooth RC car app that sends the following characters:

| Button | Character |
|--------|-----------|
| Forward | `F` |
| Backward | `B` |
| Left | `L` |
| Right | `R` |
| Stop | `S` |

### Voice Control
Use a Bluetooth voice app configured with these mappings:

| Voice Command | Character sent |
|---------------|----------------|
| "move forward" | `^` |
| "move back" | `-` |
| "turn left" | `<` |
| "turn right" | `>` |
| "stop" | `*` |

---

## How Obstacle Avoidance Works

```
┌─────────────────────────────────────────┐
│  Measure forward distance               │
│                                         │
│  distance ≤ 12 cm?                      │
│  ├── YES → Stop → Brief reverse         │
│  │         Scan LEFT  (servo @ 180°)    │
│  │         Scan RIGHT (servo @  20°)    │
│  │         leftDist > rightDist?        │
│  │           ├── YES → Turn LEFT        │
│  │           └── NO  → Turn RIGHT       │
│  └── NO  → Move FORWARD                 │
└─────────────────────────────────────────┘
```

The servo returns to centre (103°) between scans. Turn duration is fixed at 500 ms; adjust `delay(500)` in `Obstacle()` to tune turning angle.

---

## Key Constants

| Constant | Default | Description |
|----------|---------|-------------|
| `MOTOR_SPEED` | `170` | PWM duty cycle for all motors (0–255) |
| `SERVO_CENTER` | `103` | Servo centre position (degrees) |
| `OBSTACLE_CM` | `12` | Avoidance trigger distance (cm) |

---

## Repository Structure

```
arduino_robot/
├── src/
│   └── robot/
│       └── robot.ino   ← Main sketch
├── docs/
│   └── build_steps.md            ← Physical assembly guide
├── schematics/
│   └── wiring_notes.md           ← Connection reference
└── README.md
```

---

## License

This project is shared for educational purposes. Original tutorial by [SriTu Hobby](https://srituhobby.com).
