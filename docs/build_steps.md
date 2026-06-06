# Physical Build Steps

Step-by-step assembly guide for the multi-function Arduino robot chassis.

---

## Step 1 – Gather Components

Collect all parts before starting:
- Arduino UNO, L293D motor shield, HC-SR04, HC-05, SG90 servo
- 4× DC gear motors + 4× wheels
- 18650 battery holder + 2 batteries
- Foam board (chassis base), cardboard (servo mount), jumper wires

---

## Step 2 – Cut the Chassis

Cut a rectangle of foam board large enough to mount four gear motors
(two along each long side), with clearance for the Arduino stack in the centre.
A typical size is approximately **18 cm × 12 cm**, but adjust to suit your motors.

---

## Step 3 – Mount the Gear Motors

Hot-glue the four DC gear motors to the underside of the foam board:
- Two motors on the left side, two on the right side
- Shafts pointing outward so wheels clear the board edge

---

## Step 4 – Mount Arduino + Motor Shield

Stack the L293D motor shield onto the Arduino UNO, then hot-glue the
Arduino+shield stack to the top centre of the foam board.

---

## Step 5 – Route Motor Wires

Cut or drill two small holes on either side of the Arduino footprint.
Feed each pair of motor wires up through these holes so they reach the
motor shield terminals cleanly.

---

## Step 6 – Connect Motors to Shield

Refer to the wiring table in `wiring_notes.md`.  
Screw or clamp each motor wire pair into the M1–M4 terminal blocks on the shield.

Motor rotation direction convention (looking from above):
- M1 = Front-left
- M2 = Rear-left
- M3 = Front-right
- M4 = Rear-right

If a motor spins the wrong way after testing, swap its two wires.

---

## Step 7 – Servo + Sensor Mount

1. Cut a small L-shaped bracket from cardboard or foam board.
2. Hot-glue the SG90 servo onto the front of the chassis, horn pointing forward.
3. Attach the HC-SR04 ultrasonic sensor to the servo horn so it faces forward.
   Use a rubber band, small zip tie, or hot glue (remove if you need to replace it).

---

## Step 8 – Wire Servo + Ultrasonic Sensor

Connect according to `wiring_notes.md`:
- Servo signal wire → Pin 10 header on shield
- HC-SR04 Trig → A1, Echo → A0, VCC → 5 V, GND → GND

---

## Step 9 – Mount + Wire Bluetooth Module

Hot-glue the HC-05 Bluetooth module to the top of the chassis where it is
accessible. Connect:
- VCC → 5 V, GND → GND
- TX → Arduino RX, RX → Arduino TX (via shield serial pins)

---

## Step 10 – Battery Holder

Hot-glue the 18650 battery holder to the underside or rear of the chassis.
Connect its output leads to the **EXT_PWR** terminal on the motor shield
(observe polarity: red → +, black → −).

---

## Step 11 – Attach Wheels + Install Batteries

Press the four wheels onto the motor shafts (they should be a firm press-fit).
Insert two fully-charged 18650 cells into the battery holder.

---

## Step 12 – Upload Firmware

See `README.md` for software setup and mode selection.

---

## Tips

- Test each motor direction individually with a short sketch before final assembly.
- Keep jumper wire lengths short to avoid loose connections from vibration.
- Add a small power switch in series with the battery positive lead for convenience.
- If the robot drifts to one side, fine-tune the `MOTOR_SPEED` or use per-motor speed values.
