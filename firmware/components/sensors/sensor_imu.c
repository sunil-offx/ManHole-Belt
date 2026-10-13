/**
 * @file sensor_imu.c
 * @brief MPU6050 6-axis IMU driver implementation for 50 Hz motion and fall detection.
 */

#include "sensor_imu.h"
#include "hal_i2c.h"
#include "hal_timer.h"
#include <math.h>

#define MPU6050_ADDR         0x68
#define MPU6050_PWR_MGMT_1   0x6B
#define MPU6050_ACCEL_CONFIG 0x1C
#define MPU6050_ACCEL_XOUT_H 0x3B

static float s_ax = 0.0f;
static float s_ay = 0.0f;
static float s_az = 0.98f;
static uint32_t s_last_read_ms = 0;

static esp_err_t imu_init(void) {
    hal_i2c_init(HAL_I2C_PORT_0, PIN_I2C_SDA, PIN_I2C_SCL, 400000);
    
    /* Wake up MPU6050: write 0x00 to PWR_MGMT_1 */
    uint8_t pwr_cmd[2] = {MPU6050_PWR_MGMT_1, 0x00};
    hal_i2c_write(HAL_I2C_PORT_0, MPU6050_ADDR, pwr_cmd, sizeof(pwr_cmd));
    
    /* Configure Accel Full Scale Range: +/- 8g (AFS_SEL = 2, value = 0x10) */
    uint8_t accel_cfg[2] = {MPU6050_ACCEL_CONFIG, 0x10};
    hal_i2c_write(HAL_I2C_PORT_0, MPU6050_ADDR, accel_cfg, sizeof(accel_cfg));
    return ESP_OK;
}

esp_err_t sensor_imu_read_accel(float *ax, float *ay, float *az) {
    uint32_t now = hal_get_time_ms();
    if (now - s_last_read_ms < 10 && s_last_read_ms > 0) {
        if (ax) *ax = s_ax;
        if (ay) *ay = s_ay;
        if (az) *az = s_az;
        return ESP_OK;
    }

    uint8_t reg = MPU6050_ACCEL_XOUT_H;
    uint8_t data[6] = {0};
    esp_err_t err = hal_i2c_write_read(HAL_I2C_PORT_0, MPU6050_ADDR, &reg, 1, data, 6);
    if (err == ESP_OK) {
        int16_t raw_ax = (int16_t)((data[0] << 8) | data[1]);
        int16_t raw_ay = (int16_t)((data[2] << 8) | data[3]);
        int16_t raw_az = (int16_t)((data[4] << 8) | data[5]);
        
        /* For +/- 8g range, sensitivity is 4096 LSB/g */
        s_ax = (float)raw_ax / 4096.0f;
        s_ay = (float)raw_ay / 4096.0f;
        s_az = (float)raw_az / 4096.0f;
        s_last_read_ms = now;
    }

    if (ax) *ax = s_ax;
    if (ay) *ay = s_ay;
    if (az) *az = s_az;
    return ESP_OK;
}

static esp_err_t ax_read(float *value) { return sensor_imu_read_accel(value, NULL, NULL); }
static esp_err_t ay_read(float *value) { return sensor_imu_read_accel(NULL, value, NULL); }
static esp_err_t az_read(float *value) { return sensor_imu_read_accel(NULL, NULL, value); }
static esp_err_t imu_cal(void) { return ESP_OK; }

const sensor_driver_t sensor_driver_imu_ax = {
    .init = imu_init,
    .read = ax_read,
    .calibrate = imu_cal,
    .name = "IMU_Accel_X",
    .min_value = -8.0f,
    .max_value = 8.0f,
    .sample_period_ms = 20 /* 50 Hz */
};

const sensor_driver_t sensor_driver_imu_ay = {
    .init = imu_init,
    .read = ay_read,
    .calibrate = imu_cal,
    .name = "IMU_Accel_Y",
    .min_value = -8.0f,
    .max_value = 8.0f,
    .sample_period_ms = 20 /* 50 Hz */
};

const sensor_driver_t sensor_driver_imu_az = {
    .init = imu_init,
    .read = az_read,
    .calibrate = imu_cal,
    .name = "IMU_Accel_Z",
    .min_value = -8.0f,
    .max_value = 8.0f,
    .sample_period_ms = 20 /* 50 Hz */
};
