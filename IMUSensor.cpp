#include "IMUSensor.h"

IMUSensor::IMUSensor(){
    address = 0x68;

    angle = 0.0;
    gyroRate = 0.0;

    accAngle =0.0;
    alpha =0.98;

    gyroOffsetX = 0.0;
    gyroOffsetY = 0.0;
    gyroOffsetZ = 0.0;

    lastTime = 0;
}

void IMUSensor::writeRegister(uint8_t reg, uint8_t value){
    Wire.beginTransmission(address);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

uint8_t IMUSensor::readRegister(uint8_t reg){
    Wire.beginTransmission(address);
    Wire.write(reg);
    Wire.endTransmission(false);

    Wire.requestFrom(address, (uint8_t)1);

    if (Wire.available()){
        return Wire.read();
    }

    return 0;
}

void IMUSensor::begin(){
    
    Wire.begin(21, 22);

    delay(100);

    Serial.println("Starting MPU6050...");

    writeRegister(0x6B, 0x00);

    delay(100);

    writeRegister(0x1C, 0x10);

    writeRegister(0x1B, 0x08);

    writeRegister(0x1A, 0x03);

    delay(100);

    Serial.println("MPU6050 initialized!");

    Serial.print("WHO_AM_I = 0x");
    Serial.println(readRegister(0x75), HEX);

    angle = 0.0;
    gyroRate = 0.0;
    accAngle =0.0;

    lastTime = millis();
}

void IMUSensor::calibrateGyro(){
    Serial.println();
    Serial.println("=== GYRO CALIBRATION ===");
    Serial.println("Keep the MPU6050 COMPLETELY STILL!");
    Serial.println("Calibration starting...");

    
    delay(2000);

    const int samples = 1000;

    long sumX = 0;
    long sumY = 0;
    long sumZ = 0;

    for (int i = 0; i < samples; i++)
    {
        
        Wire.beginTransmission(address);
        Wire.write(0x43);
        Wire.endTransmission(false);

        
        Wire.requestFrom(address, (uint8_t)6);

        if (Wire.available() >= 6){
            int16_t gyroX = (Wire.read() << 8) | Wire.read();
            int16_t gyroY = (Wire.read() << 8) | Wire.read();
            int16_t gyroZ = (Wire.read() << 8) | Wire.read();

            sumX += gyroX;
            sumY += gyroY;
            sumZ += gyroZ;
        }

        delay(2);
    }

    
    gyroOffsetX = ((float)sumX / samples) / 65.5;
    gyroOffsetY = ((float)sumY / samples) / 65.5;
    gyroOffsetZ = ((float)sumZ / samples) / 65.5;

    Serial.println();
    Serial.println("Calibration complete!");

    Serial.print("Gyro X Offset: ");
    Serial.println(gyroOffsetX, 4);

    Serial.print("Gyro Y Offset: ");
    Serial.println(gyroOffsetY, 4);

    Serial.print("Gyro Z Offset: ");
    Serial.println(gyroOffsetZ, 4);

    Serial.println("========================");
    Serial.println();
}

void IMUSensor::readSensorData()
{
    
    Wire.beginTransmission(address);
    Wire.write(0x3B);
    Wire.endTransmission(false);

   
    Wire.requestFrom(address, (uint8_t)14);

    if (Wire.available() < 14){
        Serial.println("Sensor data read failed!");
        return;
    }

    int16_t accelX = (Wire.read() << 8) | Wire.read();
    int16_t accelY = (Wire.read() << 8) | Wire.read();
    int16_t accelZ = (Wire.read() << 8) | Wire.read();

    
    Wire.read();
    Wire.read();

    int16_t gyroX = (Wire.read() << 8) | Wire.read();
    int16_t gyroY = (Wire.read() << 8) | Wire.read();
    int16_t gyroZ = (Wire.read() << 8) | Wire.read();
  
    float ax = (float)accelX / 4096.0;
    float ay = (float)accelY / 4096.0;
    float az = (float)accelZ / 4096.0;

    float gx = ((float)gyroX / 65.5) - gyroOffsetX;
    float gy = ((float)gyroY / 65.5) - gyroOffsetY;
    float gz = ((float)gyroZ / 65.5) - gyroOffsetZ;

    gyroRate = gx;

    angle = atan2(ay, az) * 180.0 / PI;

    unsigned long currentTime = millis();
    float dt = (currentTime - lastTime)/1000.0;
    lastTime =currentTime;
    
    if(dt > 0.0 && dt < 1.0){
        angle = alpha * (angle + gyroRate * dt) + (1.0 - alpha)* accAngle;
    }
   
    Serial.print("Accel X: ");
    Serial.print(ax, 2);

    Serial.print(" | Y: ");
    Serial.print(ay, 2);

    Serial.print(" | Z: ");
    Serial.print(az, 2);

    Serial.print(" | Angle: ");
    Serial.print(angle, 2);

    Serial.print(" | Gyro X: ");
    Serial.println(gyroRate, 2);
}

void IMUSensor::update(){
    readSensorData();
}

float IMUSensor::getAngle(){
    return angle;
}

float IMUSensor::getGyroRate(){
    return gyroRate;
}