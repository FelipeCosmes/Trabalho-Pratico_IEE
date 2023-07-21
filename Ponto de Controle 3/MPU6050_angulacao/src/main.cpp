#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

float acc_x = 0.0;
float acc_y = 0.0;
float acc_z = 0.0;

float offset_x = 0.119;
float offset_y = -0.008;
float offset_z = 3.050;

float pitchAcc, rollAcc, yawAcc = 0;    // Accelerometer angles
float pitchGyro, rollGyro, yawGyro = 0; // Gyroscope angles
unsigned long prevTime = 0;             // Timer
void setup(void) {
    Serial.begin(115200);
    while (!Serial)
        delay(10);

    Serial.println("Adafruit MPU6050 test!");
    if (!mpu.begin()) {
        Serial.println("Failed to find MPU6050 chip");
        while (1) {
            delay(10);
        }
    }

    Serial.println("MPU6050 Found!");
    mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
    Serial.print("Accelerometer range set to: ");
    switch (mpu.getAccelerometerRange()) {
    case MPU6050_RANGE_2_G:
        Serial.println("+-2G");
        break;
    case MPU6050_RANGE_4_G:
        Serial.println("+-4G");
        break;
    case MPU6050_RANGE_8_G:
        Serial.println("+-8G");
        break;
    case MPU6050_RANGE_16_G:
        Serial.println("+-16G");
        break;
    }

    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    Serial.print("Gyro range set to: ");
    switch (mpu.getGyroRange()) {
    case MPU6050_RANGE_250_DEG:
        Serial.println("+- 250 deg/s");
        break;
    case MPU6050_RANGE_500_DEG:
        Serial.println("+- 500 deg/s");
        break;
    case MPU6050_RANGE_1000_DEG:
        Serial.println("+- 1000 deg/s");
        break;
    case MPU6050_RANGE_2000_DEG:
        Serial.println("+- 2000 deg/s");
        break;
    }

    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    Serial.print("Filter bandwidth set to: ");
    switch (mpu.getFilterBandwidth()) {
    case MPU6050_BAND_260_HZ:
        Serial.println("260 Hz");
        break;
    case MPU6050_BAND_184_HZ:
        Serial.println("184 Hz");
        break;
    case MPU6050_BAND_94_HZ:
        Serial.println("94 Hz");
        break;
    case MPU6050_BAND_44_HZ:
        Serial.println("44 Hz");
        break;
    case MPU6050_BAND_21_HZ:
        Serial.println("21 Hz");
        break;
    case MPU6050_BAND_10_HZ:
        Serial.println("10 Hz");
        break;
    case MPU6050_BAND_5_HZ:
        Serial.println("5 Hz");
        break;
    }
    Serial.println("");
    delay(100);
}

void loop() {
    sensors_event_t a, g, temp;                   // Create the sensor event variable
    mpu.getEvent(&a, &g, &temp);                  // Read the sensor
    unsigned long currentTime = millis();         // Current time
    float dt = (currentTime - prevTime) / 1000.0; // Calculate delta time in seconds
    prevTime = currentTime;                       // Previous time is stored so we can calculate dt
    // Calculate pitch and roll using the accelerometer data
    acc_x = a.acceleration.x - offset_x;
    acc_y = a.acceleration.y - offset_y;
    acc_z = a.acceleration.z - offset_z;

    // Calculate pitch, roll and yaw using the gyro data
    pitchGyro += g.gyro.y * dt;
    rollGyro += g.gyro.x * dt;
    yawGyro += g.gyro.z * dt;

    // Calculate roll and pitch angles
    rollAcc = atan2(acc_y, acc_z) * RAD_TO_DEG;
    pitchAcc = atan2(-acc_x, sqrt(acc_y * acc_y + acc_z * acc_z)) * RAD_TO_DEG;
    yawAcc = atan2(acc_z, sqrt(acc_x * acc_x + acc_y * acc_y)) * RAD_TO_DEG;

    Serial.print("X:");
    Serial.print(acc_x);
    Serial.print(" Y:");
    Serial.print(acc_y);
    Serial.print(" Z:");
    Serial.print(acc_z);

    Serial.print(" pitchGyro:");
    Serial.print(pitchGyro);
    Serial.print(" rollGyro:");
    Serial.print(rollGyro);
    Serial.print(" yawGyro:");
    Serial.print(yawGyro);

    Serial.print(" RollAcc:");
    Serial.print(rollAcc);
    Serial.print(" PitchAcc:");
    Serial.print(pitchAcc);
    Serial.print(" YawAcc:");
    Serial.print(yawAcc);

    Serial.print(" RollGyro:");
    Serial.print(rollGyro * RAD_TO_DEG);
    Serial.print(" PitchGyro:");
    Serial.print(pitchGyro * RAD_TO_DEG);
    Serial.print(" YawGyro:");
    Serial.println(yawGyro * RAD_TO_DEG);
}