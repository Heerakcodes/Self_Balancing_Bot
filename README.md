# Self-Balancing Robot

An ESP32-based two-wheel self-balancing robot using an MPU6050 IMU, two stepper motors, and STEP/DIR/ENABLE motor drivers.

## Project files

| File | Purpose |
| --- | --- |
| `TASK.ino` | Main program: reads the IMU, runs PID control, and commands both motors. |
| `IMUSensor.*` | MPU6050 I2C communication, gyro calibration, and angle estimation. |
| `PIDController.*` | PID calculation that converts tilt error into motor speed. |
| `StepperMotor.*` | Non-blocking STEP/DIR/ENABLE driver control. |

## Hardware configuration

The code is written for an ESP32 and an MPU6050 at I2C address `0x68`.

| Connection | ESP32 GPIO |
| --- | ---: |
| MPU6050 SDA | 21 |
| MPU6050 SCL | 22 |
| Left driver STEP | 25 |
| Left driver DIR | 26 |
| Left driver ENABLE | 33 |
| Right driver STEP | 19 |
| Right driver DIR | 18 |
| Right driver ENABLE | 17 |

All components must share a common ground. Use a suitable external power supply for the stepper motors; do not power motors from the ESP32’s 3.3 V pin.

## Before first use

1. Upload `TASK.ino` to the ESP32.
2. Open Serial Monitor at **115200 baud**.
3. Keep the robot completely still during gyro calibration.
4. Test with the wheels lifted off the ground first.
5. Tilt the robot gently. The wheels must move toward the falling direction.
6. If a motor rotates incorrectly, change:

   ```cpp
   const int RIGHT_MOTOR_SIGN = 1;
