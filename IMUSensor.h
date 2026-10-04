#include <Wire.h>
#include <MPU6050.h>

class IMUSensor{
    private:
    MPU6050 mpu;

    int16_t ax, ay, az;
    int16_t gy, gy, gz;

    public:
    
    bool begin();
    void update();

    int16_t getAccelX();
    int16_t getAccelY();
    int16_t getAccelZ();

    int16_t getGyroX();
    int16_t getGyroY();
    int16_t getGyroZ();

    
};