# Wiring Reference

Complete pin-by-pin connection table for the multi-function Arduino robot.

---

## HC-SR04 Ultrasonic Sensor

| Sensor Pin | Connects To         |
|------------|---------------------|
| VCC        | 5 V (motor shield)  |
| GND        | GND                 |
| TRIG       | Arduino A1          |
| ECHO       | Arduino A0          |

---

## SG90 Servo Motor

| Servo Wire | Connects To                    |
|------------|--------------------------------|
| Brown/Black (GND)  | GND                  |
| Red (VCC)          | 5 V (motor shield)   |
| Orange (Signal)    | Pin 10 (servo header on shield) |

---

## HC-05 Bluetooth Module

| Module Pin | Connects To              | Notes                              |
|------------|--------------------------|------------------------------------|
| VCC        | 5 V                      |                                    |
| GND        | GND                      |                                    |
| TX         | Arduino RX               | ⚠️ Disconnect before uploading code |
| RX         | Arduino TX               | ⚠️ Disconnect before uploading code |

> The HC-05 operates at 3.3 V logic. Most breakout boards include an onboard
> regulator and level shifter, but verify yours before connecting directly to 5 V TX.

---

## DC Gear Motors → Motor Shield

| Motor Position | Shield Terminal |
|----------------|-----------------|
| Front-left     | M1              |
| Rear-left      | M2              |
| Front-right    | M3              |
| Rear-right     | M4              |

If a motor spins in the wrong direction, swap its two wires in the terminal block.

---

## Power

| Connection               | Detail                              |
|--------------------------|-------------------------------------|
| Battery holder (+)       | EXT_PWR (+) on motor shield         |
| Battery holder (−)       | EXT_PWR (−) on motor shield         |
| Battery voltage          | 2× 18650 in series ≈ 7.4 V nominal |
| Arduino powered via      | Shield VIN rail (regulated to 5 V)  |

---

## Upload Checklist

1. ☐ Disconnect HC-05 **TX and RX** wires from Arduino
2. ☐ Select **Board: Arduino UNO** in Arduino IDE
3. ☐ Select correct **Port**
4. ☐ Uncomment desired mode in `loop()` and upload
5. ☐ Reconnect HC-05 **TX and RX** wires
6. ☐ Power cycle the robot
