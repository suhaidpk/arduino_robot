/*
 * Multi-Function Arduino Robot
 * ============================
 * Features:
 *   1. Obstacle Avoidance  - Uses HC-SR04 ultrasonic sensor + servo for L/R scanning
 *   2. Bluetooth Control   - Drive via smartphone app (F/B/L/R/S commands)
 *   3. Voice Control       - Drive via voice commands through Bluetooth app
 *
 * Hardware:
 *   - Arduino UNO
 *   - L293D Motor Driver Shield
 *   - HC-SR04 Ultrasonic Sensor  (Echo → A0, Trig → A1)
 *   - SG90 Servo Motor           (Signal → Pin 10 via shield)
 *   - HC-05 Bluetooth Module     (TX → RX, RX → TX on shield)
 *   - 4× DC Gear Motors          (M1–M4 on shield)
 *   - 2× Li-Ion 18650 batteries
 *
 * Usage:
 *   Uncomment ONE function in loop() to select the operating mode.
 *   When uploading code, ALWAYS disconnect the Bluetooth RX/TX wires first,
 *   then reconnect after uploading.
 *
 * Reference: https://srituhobby.com/how-to-make-a-multi-function-arduino-robot/
 * Library:   AFMotor (Adafruit Motor Shield Library v1)
 */

#include <Servo.h>
#include <AFMotor.h>

// ── Pin Definitions ───────────────────────────────────────────────
#define ECHO_PIN   A0   // Ultrasonic sensor Echo
#define TRIG_PIN   A1   // Ultrasonic sensor Trigger
#define SERVO_PIN  10   // Servo motor signal (via motor shield header)

// ── Tuning Parameters ─────────────────────────────────────────────
#define MOTOR_SPEED   170   // PWM speed for all 4 motors (0–255)
#define SERVO_CENTER  103   // Servo centre position in degrees
#define OBSTACLE_CM    12   // Distance threshold (cm) to trigger avoidance

// ── Global Variables ──────────────────────────────────────────────
char    btValue;    // Latest byte from Bluetooth serial
int     distance;   // Current forward distance (cm)
int     leftDist;   // Left-scan distance (cm)
int     rightDist;  // Right-scan distance (cm)

// ── Object Instantiation ──────────────────────────────────────────
Servo servo;

AF_DCMotor M1(1);   // Front-left
AF_DCMotor M2(2);   // Rear-left
AF_DCMotor M3(3);   // Front-right
AF_DCMotor M4(4);   // Rear-right

// ═════════════════════════════════════════════════════════════════
void setup() {
  Serial.begin(9600);

  // Ultrasonic sensor
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Servo
  servo.attach(SERVO_PIN);
  servo.write(SERVO_CENTER);

  // Motor speeds
  M1.setSpeed(MOTOR_SPEED);
  M2.setSpeed(MOTOR_SPEED);
  M3.setSpeed(MOTOR_SPEED);
  M4.setSpeed(MOTOR_SPEED);
}

// ── SELECT ONE MODE BELOW ────────────────────────────────────────
void loop() {
  // Obstacle();         // Mode 1 – Autonomous obstacle avoidance
  // Bluetoothcontrol(); // Mode 2 – Bluetooth joystick control
  // voicecontrol();     // Mode 3 – Voice command control
}

// ═════════════════════════════════════════════════════════════════
// MODE 1 – OBSTACLE AVOIDANCE
// ─────────────────────────────────────────────────────────────────
// Servo scans left (180°) and right (20°) when an obstacle is
// detected within OBSTACLE_CM.  Robot turns toward the side with
// more free space.
// ═════════════════════════════════════════════════════════════════
void Obstacle() {
  distance = ultrasonic();

  if (distance <= OBSTACLE_CM) {
    Stop();
    backward();
    delay(100);
    Stop();

    // Scan left, return servo, scan right
    leftDist  = leftsee();
    servo.write(SERVO_CENTER);
    delay(800);
    rightDist = rightsee();
    servo.write(SERVO_CENTER);

    if (leftDist < rightDist) {
      // More space on the right → turn right
      right();
      delay(500);
      Stop();
      delay(200);
    } else if (leftDist > rightDist) {
      // More space on the left → turn left
      left();
      delay(500);
      Stop();
      delay(200);
    }
    // If equal, loop again and re-evaluate
  } else {
    forward();
  }
}

// ═════════════════════════════════════════════════════════════════
// MODE 2 – BLUETOOTH CONTROL
// ─────────────────────────────────────────────────────────────────
// App sends single characters over Bluetooth:
//   F = Forward | B = Backward | L = Left | R = Right | S = Stop
// ═════════════════════════════════════════════════════════════════
void Bluetoothcontrol() {
  if (Serial.available() > 0) {
    btValue = Serial.read();
    Serial.println(btValue);   // Echo for debugging
  }

  if      (btValue == 'F') { forward();  }
  else if (btValue == 'B') { backward(); }
  else if (btValue == 'L') { left();     }
  else if (btValue == 'R') { right();    }
  else if (btValue == 'S') { Stop();     }
}

// ═════════════════════════════════════════════════════════════════
// MODE 3 – VOICE CONTROL
// ─────────────────────────────────────────────────────────────────
// Voice app maps spoken words to special characters over Bluetooth:
//   ^  = "move forward"  |  -  = "move back"
//   <  = "turn left"     |  >  = "turn right"
//   *  = "stop"
// Servo checks clearance before executing left/right turns.
// ═════════════════════════════════════════════════════════════════
void voicecontrol() {
  if (Serial.available() > 0) {
    btValue = Serial.read();
    Serial.println(btValue);

    if (btValue == '^') {
      forward();

    } else if (btValue == '-') {
      backward();

    } else if (btValue == '<') {
      leftDist = leftsee();
      servo.write(SERVO_CENTER);
      if (leftDist >= 10) {
        left();
        delay(500);
        Stop();
      } else {
        Stop();   // Obstacle on left; don't turn
      }

    } else if (btValue == '>') {
      rightDist = rightsee();
      servo.write(SERVO_CENTER);
      if (rightDist >= 10) {
        right();
        delay(500);
        Stop();
      } else {
        Stop();   // Obstacle on right; don't turn
      }

    } else if (btValue == '*') {
      Stop();
    }
  }
}

// ═════════════════════════════════════════════════════════════════
// SENSOR HELPERS
// ═════════════════════════════════════════════════════════════════

// Returns distance in centimetres (HC-SR04)
int ultrasonic() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(4);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long t  = pulseIn(ECHO_PIN, HIGH);
  long cm = t / 29 / 2;
  return (int)cm;
}

// Rotate servo to look LEFT, measure, return distance
int leftsee() {
  servo.write(180);
  delay(800);
  return ultrasonic();
}

// Rotate servo to look RIGHT, measure, return distance
int rightsee() {
  servo.write(20);
  delay(800);
  return ultrasonic();
}

// ═════════════════════════════════════════════════════════════════
// MOTOR PRIMITIVES
// ═════════════════════════════════════════════════════════════════

void forward() {
  M1.run(FORWARD);  M2.run(FORWARD);
  M3.run(FORWARD);  M4.run(FORWARD);
}

void backward() {
  M1.run(BACKWARD); M2.run(BACKWARD);
  M3.run(BACKWARD); M4.run(BACKWARD);
}

// Left motors backward + Right motors forward → pivot left
void left() {
  M1.run(FORWARD);  M2.run(FORWARD);
  M3.run(BACKWARD); M4.run(BACKWARD);
}

// Left motors forward + Right motors backward → pivot right
void right() {
  M1.run(BACKWARD); M2.run(BACKWARD);
  M3.run(FORWARD);  M4.run(FORWARD);
}

void Stop() {
  M1.run(RELEASE); M2.run(RELEASE);
  M3.run(RELEASE); M4.run(RELEASE);
}
