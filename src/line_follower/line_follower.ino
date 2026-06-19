/*
 * PID Line Following Robot
 * 
 * A 2-wheeled differential drive robot that follows a black line on a white
 * surface using 2 IR sensors and PID control algorithm.
 * 
 * Hardware:
 * - Arduino UNO
 * - L298N Motor Driver Module
 * - 2x DC Motors (Yellow)
 * - 2x IR Sensor Modules (Left and Right)
 * - 9V Battery
 * 
 * Competition Project - Dynamac Robotics Club (Embedded Systems Division)
 */

// ============================================================================
// PIN DEFINITIONS
// ============================================================================

// IR Sensors (Digital Input)
#define LEFT_SENSOR  2   // Left IR sensor output
#define RIGHT_SENSOR 3   // Right IR sensor output

// L298N Motor Driver Pins
#define IN1 4   // Left motor direction pin 1
#define IN2 5   // Left motor direction pin 2
#define IN3 6   // Right motor direction pin 1
#define IN4 7   // Right motor direction pin 2
#define ENA 9   // Left motor PWM speed control
#define ENB 10  // Right motor PWM speed control

// ============================================================================
// PID TUNING CONSTANTS (Adjust these to tune robot behavior)
// ============================================================================

float Kp = 2.0;   // Proportional gain - how aggressively to correct errors
float Ki = 0.0;   // Integral gain - eliminates steady-state error (usually 0 for simple line following)
float Kd = 1.0;   // Derivative gain - reduces oscillation and overshoot

// ============================================================================
// GLOBAL VARIABLES
// ============================================================================

// Base motor speed (0-255)
const int baseSpeed = 150;

// PID calculation variables
float error = 0;
float lastError = 0;
float integral = 0;
float derivative = 0;
float correction = 0;

// Sensor readings
int leftSensor = 0;
int rightSensor = 0;

// ============================================================================
// SETUP - Runs once at startup
// ============================================================================

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  Serial.println("PID Line Follower - Starting...");
  
  // Configure sensor pins as inputs
  pinMode(LEFT_SENSOR, INPUT);
  pinMode(RIGHT_SENSOR, INPUT);
  
  // Configure motor driver pins as outputs
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  
  // Initialize motors to stopped state
  stopMotors();
  
  Serial.println("Setup complete. Place robot on line.");
  delay(2000);  // 2-second delay before starting
}

// ============================================================================
// MAIN LOOP - Runs continuously
// ============================================================================

void loop() {
  // Read sensor values
  // IR sensors output LOW when detecting black line, HIGH when detecting white
  leftSensor = digitalRead(LEFT_SENSOR);
  rightSensor = digitalRead(RIGHT_SENSOR);
  
  // Calculate error based on sensor states
  // Error values:
  //  0  = Both sensors on line (centered)
  // -1  = Left sensor off line (need to turn left)
  //  1  = Right sensor off line (need to turn right)
  //  lastError = Both sensors off line (use previous correction)
  
  if (leftSensor == LOW && rightSensor == LOW) {
    // Both sensors on black line - robot is centered
    error = 0;
  } 
  else if (leftSensor == HIGH && rightSensor == LOW) {
    // Left sensor off line - robot drifted right, turn left
    error = -1;
  } 
  else if (leftSensor == LOW && rightSensor == HIGH) {
    // Right sensor off line - robot drifted left, turn right
    error = 1;
  } 
  else {
    // Both sensors off line - use last known error to recover
    error = lastError;
  }
  
  // Calculate PID correction
  integral += error;  // Accumulate error over time
  derivative = error - lastError;  // Rate of change of error
  
  correction = (Kp * error) + (Ki * integral) + (Kd * derivative);
  
  // Store current error for next iteration
  lastError = error;
  
  // Apply correction to motor speeds
  // Positive correction = turn right (slow down right motor)
  // Negative correction = turn left (slow down left motor)
  
  int leftMotorSpeed = baseSpeed - correction;
  int rightMotorSpeed = baseSpeed + correction;
  
  // Constrain motor speeds to valid PWM range (0-255)
  leftMotorSpeed = constrain(leftMotorSpeed, 0, 255);
  rightMotorSpeed = constrain(rightMotorSpeed, 0, 255);
  
  // Set motor speeds and direction (forward)
  setMotorSpeed(leftMotorSpeed, rightMotorSpeed);
  
  // Debug output to Serial Monitor
  Serial.print("L:");
  Serial.print(leftSensor);
  Serial.print(" R:");
  Serial.print(rightSensor);
  Serial.print(" | Err:");
  Serial.print(error);
  Serial.print(" Corr:");
  Serial.print(correction);
  Serial.print(" | LSpeed:");
  Serial.print(leftMotorSpeed);
  Serial.print(" RSpeed:");
  Serial.println(rightMotorSpeed);
  
  // Small delay for stability
  delay(10);
}

// ============================================================================
// MOTOR CONTROL FUNCTIONS
// ============================================================================

/**
 * Set motor speeds and move forward
 * @param leftSpeed PWM value for left motor (0-255)
 * @param rightSpeed PWM value for right motor (0-255)
 */
void setMotorSpeed(int leftSpeed, int rightSpeed) {
  // Left motor forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, leftSpeed);
  
  // Right motor forward
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, rightSpeed);
}

/**
 * Stop both motors
 */
void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
