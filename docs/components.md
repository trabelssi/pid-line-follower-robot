# Components List

This document provides detailed information about each component used in the PID Line Following Robot.

## Component Overview

| Component | Quantity | Purpose |
|---|---|---|
| Arduino UNO | 1 | Main microcontroller |
| L298N Motor Driver | 1 | Motor speed & direction control |
| DC Motor (Yellow) | 2 | Drive wheels (left & right) |
| IR Sensor Module | 2 | Left & right line detection |
| Breadboard | 1 | Wire distribution |
| 9V Battery | 1 | Power supply |
| Jumper Wires | — | Component connections |

---

## Detailed Component Information

### Arduino UNO

**Purpose:** Main microcontroller that runs the PID control algorithm

**Specifications:**
- Microcontroller: ATmega328P
- Operating Voltage: 5V
- Digital I/O Pins: 14 (6 PWM outputs)
- Clock Speed: 16 MHz

**Role in Project:**
- Reads sensor data from IR modules
- Calculates PID correction values
- Controls motor driver via digital and PWM signals
- Provides serial debugging output

---

### L298N Motor Driver Module

**Purpose:** Controls the speed and direction of both DC motors

**Specifications:**
- Dual H-Bridge motor driver
- Operating Voltage: 5-35V
- Maximum Current: 2A per channel
- Built-in 5V regulator (powers Arduino)
- PWM speed control support

**Features:**
- IN1, IN2: Control left motor direction
- IN3, IN4: Control right motor direction
- ENA, ENB: PWM speed control for left and right motors
- 12V input from battery
- 5V output to power Arduino

**Role in Project:**
- Provides bidirectional control for both motors
- Converts Arduino's low-current PWM signals to high-current motor control
- Powers the Arduino through its onboard 5V regulator

---

### DC Motors (Yellow)

**Purpose:** Drive the left and right wheels for differential steering

**Specifications:**
- Type: Hobby DC gear motors
- Operating Voltage: 3-12V
- Typical RPM: 200 (at 6V)
- Built-in reduction gearbox

**Role in Project:**
- Left motor and right motor can be controlled independently
- Differential speed creates turning motion
- Forward motion when both motors run at equal speed

---

### IR Sensor Modules

**Purpose:** Detect the black line on a white surface

**Specifications:**
- Type: Digital IR obstacle/line detection module
- Detection Range: 2-30cm (adjustable via potentiometer)
- Output: Digital HIGH (white) or LOW (black)
- Operating Voltage: 3.3-5V

**Features:**
- Built-in IR LED transmitter
- IR phototransistor receiver
- LM393 comparator for digital output
- Sensitivity adjustment potentiometer

**Configuration:**
- **Left Sensor:** Positioned on the left edge of the robot
- **Right Sensor:** Positioned on the right edge of the robot
- Both sensors should be adjusted to detect the line at approximately 1-2cm height

**Role in Project:**
- Left sensor detects if robot is drifting right (off the line on left side)
- Right sensor detects if robot is drifting left (off the line on right side)
- Binary sensor readings form the basis of error calculation in PID

---

### Breadboard

**Purpose:** Provides a central connection point for organizing wires

**Specifications:**
- Standard 400-point solderless breadboard
- Power rails for 5V and GND distribution

**Role in Project:**
- Distributes 5V and GND to both IR sensors
- Organizes jumper wire connections
- Allows easy modification and debugging

---

### 9V Battery

**Purpose:** Main power source for the robot

**Specifications:**
- Type: 9V alkaline or rechargeable battery
- Capacity: ~500mAh (typical alkaline)

**Role in Project:**
- Powers L298N motor driver
- L298N regulates 9V down to 5V for Arduino
- Provides sufficient current for motors under load

---

### Jumper Wires

**Purpose:** Connect all components together

**Types Used:**
- Male-to-male (breadboard connections)
- Male-to-female (Arduino to modules)
- Female-to-female (motor to driver)

**Role in Project:**
- Sensor signal wires
- Motor driver control signals
- Power and ground distribution

---

## Component Selection Notes

This component selection prioritizes:
- **Simplicity:** Minimal sensor setup (2 IR sensors instead of 5-sensor arrays)
- **Affordability:** Common hobby robotics components
- **Reliability:** Well-tested modules with stable digital outputs
- **Availability:** All components readily available from electronics suppliers

The 2-sensor configuration is sufficient for smooth line following when paired with a properly tuned PID controller, demonstrating that sophisticated behavior doesn't require complex hardware.
