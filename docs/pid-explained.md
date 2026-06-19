# PID Control Explained

This document explains PID (Proportional-Integral-Derivative) control in simple terms, specifically in the context of line following robots.

## What is PID Control?

PID is a feedback control algorithm that continuously calculates an error value (the difference between a desired setpoint and a measured value) and applies a correction based on three components:

- **P**roportional
- **I**ntegral  
- **D**erivative

Think of it like steering a car to stay in your lane:
- **P** tells you how far off you are from the center
- **I** reminds you if you've been drifting consistently
- **D** warns you how quickly you're veering off course

---

## PID Control Loop Diagram

```
                 ┌─────────────┐
    Setpoint     │             │  Correction
  ──────────────►│ PID Control │──────────────► Motor Speed
                 │             │
                 └──────▲──────┘
                        │
                  IR Sensors
                (Error feedback)
```

The robot continuously:
1. Measures its position using IR sensors
2. Calculates the error (how far off the line it is)
3. Applies PID calculation to determine correction
4. Adjusts motor speeds to correct the error
5. Repeats the cycle

### Complete Control Flow

![PID Line Follower Control Flow](pid_line_follower_control_flow.png)

The diagram above shows the complete control flow including sensor reading, error calculation, PID computation, and motor control logic.

---

## The Three Components

### Proportional (Kp)

**What it does:** Responds to the current error

**Formula:** `P = Kp × error`

**In line following:**
- When the left sensor goes off the line, error = -1
- The correction is proportional to this error
- Higher Kp = stronger, faster correction

**Effect of increasing Kp:**
- ✅ Faster response to errors
- ✅ Robot follows the line more aggressively
- ❌ Too high → oscillation (robot zigzags)
- ❌ Can overshoot the line

**Effect of decreasing Kp:**
- ✅ Smoother, gentler corrections
- ❌ Slower response
- ❌ May not correct fast enough on sharp turns

---

### Integral (Ki)

**What it does:** Responds to accumulated past errors

**Formula:** `I = Ki × (sum of all errors over time)`

**In line following:**
- If the robot consistently drifts slightly left, the integral accumulates
- Eventually creates a correction even if current error is small
- Eliminates steady-state error (persistent drift)

**Effect of increasing Ki:**
- ✅ Eliminates consistent drift or bias
- ❌ Can cause instability and oscillation
- ❌ Integral windup (accumulated error becomes too large)

**Effect of decreasing Ki:**
- ✅ More stable behavior
- ❌ May allow persistent drift

**Note:** For simple 2-sensor line followers, Ki is often set to **0** because:
- The robot quickly crosses back and forth over the line
- There's rarely sustained steady-state error
- Adding Ki can introduce instability without significant benefit

---

### Derivative (Kd)

**What it does:** Responds to the rate of change of error

**Formula:** `D = Kd × (current error - previous error)`

**In line following:**
- Predicts future error based on current trend
- If error is increasing rapidly, applies stronger correction
- Acts as a damping force to prevent overshooting

**Effect of increasing Kd:**
- ✅ Reduces oscillation
- ✅ Smoother path following
- ✅ Dampens overshoot
- ❌ Too high → sluggish response
- ❌ Sensitive to sensor noise

**Effect of decreasing Kd:**
- ✅ Less sensitive to noise
- ❌ More oscillation
- ❌ Overshooting on sharp turns

---

## Complete PID Formula

```
correction = (Kp × error) + (Ki × integral) + (Kd × derivative)
```

Where:
- **error** = current deviation from setpoint
- **integral** = sum of all past errors
- **derivative** = (current error - last error)

This correction value is then used to adjust motor speeds:
```
leftMotorSpeed = baseSpeed - correction
rightMotorSpeed = baseSpeed + correction
```

---

## PID Tuning Guide for Line Followers

Follow this step-by-step process:

### Step 1: Start with P Only
```cpp
Kp = 1.0
Ki = 0.0
Kd = 0.0
```
- Test the robot
- If it doesn't correct enough, increase Kp
- If it oscillates wildly, decrease Kp
- Find a Kp where the robot follows with slight oscillation

### Step 2: Add D to Smooth
```cpp
Kp = 2.0  // (from step 1)
Ki = 0.0
Kd = 0.5
```
- Add derivative to dampen oscillations
- Increase Kd until path smooths out
- If robot becomes sluggish, reduce Kd

### Step 3: Add I Only If Needed
```cpp
Kp = 2.0
Ki = 0.01  // Start very small
Kd = 1.0
```
- Only add if robot has persistent drift
- Start with very small Ki values (0.01 or less)
- For 2-sensor systems, usually keep Ki = 0

---

## Real-World Example

**Default values in our code:**
```cpp
float Kp = 2.0;
float Ki = 0.0;
float Kd = 1.0;
```

**Why these values?**
- **Kp = 2.0:** Provides strong correction without excessive oscillation
- **Ki = 0.0:** Not needed for this simple 2-sensor setup
- **Kd = 1.0:** Dampens oscillation and smooths the path

**Tuning for different tracks:**
- **Gentle curves:** Can use lower Kp (1.5), higher Kd (1.5) for smooth path
- **Sharp turns:** Need higher Kp (3.0), moderate Kd (0.5-1.0) for aggressive correction
- **High speed:** Increase Kd to prevent overshooting
- **Low speed:** Can reduce Kd, focus on Kp

---

## Common Problems and Solutions

| Problem | Likely Cause | Solution |
|---------|-------------|----------|
| Robot oscillates back and forth | Kp too high | Decrease Kp |
| Robot slowly drifts off line | Kp too low | Increase Kp |
| Robot overshoots on turns | Kd too low | Increase Kd |
| Robot responds sluggishly | Kd too high | Decrease Kd |
| Robot has consistent drift | Needs integral | Add small Ki (0.01-0.05) |
| Unstable, erratic behavior | Ki too high | Decrease Ki or set to 0 |

---

## Why PID Works for Line Following

Traditional "bang-bang" control (full left or full right) causes:
- Jerky motion
- Excessive zigzagging
- Wear on motors and chassis
- Slower overall speed

PID control provides:
- **Smooth corrections** proportional to error
- **Predictive damping** to prevent overshoot
- **Adaptability** to different track conditions
- **Higher average speed** with less oscillation

The result is a robot that gracefully glides along the line rather than frantically jerking back and forth.

---

## Advanced Concepts

### Sample Rate
The `delay(10)` in the main loop creates a ~100Hz control loop. Faster sampling generally improves control but increases computational load.

### Integral Windup Prevention
If using Ki, limit integral accumulation:
```cpp
integral = constrain(integral, -100, 100);
```

### Setpoint
In our implementation, the setpoint is implicitly **0** (centered on line). Both sensors detecting the line produces error = 0, which is our desired state.

### Multi-Sensor Arrays
Robots with 5+ sensors can calculate more precise error values (e.g., -2, -1, 0, 1, 2) allowing finer position control and smoother PID response.

---

## Conclusion

PID control transforms a simple sensor system into intelligent behavior. By understanding how Kp, Ki, and Kd affect robot motion, you can tune your line follower to perform optimally on any track.

**Key Takeaway:** Start simple (P only), add complexity as needed (D for smoothness), and keep Ki = 0 unless you have persistent drift. Monitor Serial output to see PID values in real-time and adjust accordingly.

Happy tuning! 🤖
