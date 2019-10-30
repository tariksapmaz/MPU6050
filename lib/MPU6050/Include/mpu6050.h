// General Variables on I2C and Program Algorithms
#include "stdint.h"
#include "stdbool.h"
#include "math.h"

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_i2c.h"

#define MPU6050_DEFAULT_TIMEOUT_VALUE 5 * 1000 // 5 Seconds

//**************** MPU6050 REGISTER ****************//

// Sampling Rate Calculation:
// Sample Rate = Gyroscope Output Rate / (1 + SMPLRT_DIV)
// For example, if we define the gyroscope sampling rate as 8 kHz,
// and set the value of "SMPLRT_DIV" to 31,
// then our sampling rate is 250 Hz.
// for more details, please look at page 11 in MPU-6000 and
// MPU-6050 Register Map and Descriptions Revision 4.2 PDF
//
// NOTE: You cannot set accelerometer rate than 1kHz.
typedef enum {
	SR8000 = 0,
	SR4000Hz = 1,
	SR2000Hz = 3,
	SR1000Hz = 7,
	SR500Hz = 15,
	SR250Hz = 31,
	SR125Hz = 63,
	SR100Hz = 79
} MPU6050_SamplingRate;

// Accelerometer Range Configuration
// Register Value    |   Range Value
//        0			 |	   +-2g
//		  1 		 |     +-4g
//        2 		 |     +-8g
//        3 		 | 	   +-16g
typedef enum {
	AR2 = 0, AR4 = 1, AR8 = 2, AR16 = 3
} MPU6050_AccRange;

// Gyroscope Range Configuration
// Register Value    |   Range Value
//        0			 |	 +-250  deg/sec
//		  1 		 |   +-500  deg/sec
//        2 		 |   +-1000 deg/sec
//        3 		 | 	 +-2000 deg/sec
typedef enum {
	GR250 = 0, GR500 = 1, GR1000 = 2, GR2000 = 3
} MPU6050_GyroRange;

// Defined Address List
typedef enum {
	MPU6050_REGISTER_ADDR_POWER_MANAGEMENT_2 = 0x6B,
	MPU6050_REGISTER_ADDR_SAMPLING_RATE = 0x19,
	MPU6050_REGISTER_ADDR_GYRO_RANGE_CONFIG = 0x1B,
	MPU6050_REGISTER_ADDR_ACCEL_RANGE_CONFIG = 0x1C,
	MPU6050_REGISTER_ADDR_INTERRUPT_ENABLE = 0x38,
	MPU6050_REGISTER_ADDR_INTERRUPT_CONFIG = 0x37,
	MPU6050_REGISTER_ADDR_ACCEL_XOUT_H = 0x3B,
	MPU6050_REGISTER_ADDR_ACCEL_YOUT_H = 0x3D,
	MPU6050_REGISTER_ADDR_ACCEL_YOUT_Z = 0x3F,
	MPU6050_REGISTER_ADDR_TEMP_OUT_H = 0x41,
	MPU6050_REGISTER_ADDR_GYRO_XOUT_H = 0x43,
	MPU6050_REGISTER_ADDR_GYRO_YOUT_H = 0x45,
	MPU6050_REGISTER_ADDR_GYRO_ZOUT_H = 0x47,
} MPU6050_RegisterMap;

//**************** MPU6050 REGISTER ****************//

//* General Variables for MPU6050 6-Axis Sensor *//
struct MPU6050_Properties {
	/* Some Special Variables Part */
	uint8_t I2CAddress;
	MPU6050_AccRange accRange;
	MPU6050_GyroRange gyroRange;
	MPU6050_SamplingRate srRate;

	/* Data Part */
	uint16_t xRawAccValue;
	uint16_t yRawAccValue;
	uint16_t zRawAccValue;

	uint16_t xRawGyroValue;
	uint16_t yRawGyroValue;
	uint16_t zRawGyroValue;

	uint16_t rawTempValue;
	float tempValue;

	float xAccValue;
	float yAccValue;
	float zAccValue;

	float xGyroValue;
	float yGyroValue;
	float zGyroValue;
} MPU6050Properties;

//**************** MPU6050 ERROR CODES ****************//

typedef enum {
	NoError,
	UninitAddr,
	UninitI2CInterface,
	BusyI2CInterface,
	TimeoutI2CInterface,
	ErrorI2CInterface
} MPU6050_ErrorCode;

static const char *MPU6050_ERROR_STRING[] = { "No Error",
		"MPU6050 I2C Address is not initialized",
		"MC's I2C Interface is not initialized", "Busy I2C Interface",
		"Timeout occured in I2C Interface",
		"Unknown error occured in I2C Interface" };

struct MPU6050_Error {
	int errorCode;
	const char *errorString;
} MPU6050ErrorProperties;

//**************** MPU6050 ERROR CODES ****************//

//**************** MPU6050 CODES ****************//

/**
 * MPU6050, has a several power management .
 * At the start of the device, MPU6050 is asleep.
 * The device must therefore be wakened. From datasheet
 * PWR_MGMT_2 register (0x6C) has a first two bits (MSB) is LP_WAKE_CTRL bits.
 * You can set these bits to 0 (which means 1.25 Hz from datasheet). After that
 * the device is now ready to send you data.
 *
 * @brief Wake up Sensor (Check out power management on MPU6050 sensor)
 * @param current i2c interface for MPU6050
 * @return process result
 */
bool MPU6050WakeUpSensor(I2C_HandleTypeDef *I2Cx);

/**
 * MPU6050 has a several accelerometer range. In this way,
 * we can adjust accelerometer range for more accuracy.
 * (Accelerometer Configuration Register 0x1C, Bit4:Bit3)
 *
 * @brief Set Accelerometer Range
 * @param current i2c interface for MPU6050
 * @param MPU6050 accelerometer range value
 * @return process result
 */
bool MPU6050SetAccelerometerRange(I2C_HandleTypeDef *I2Cx, MPU6050_AccRange accRange);

/**
 * MPU6050 has a several gyro range. In this way,
 * we can adjust gyro range for more accuracy.
 * (Accelerometer Configuration Register 0x1B, Bit4:Bit3)
 *
 * @brief Set Accelerometer Range
 * @param current i2c interface for MPU6050
 * @param MPU6050 gyroscope range value
 * @return process result
 */
bool MPU6050SetGyroRange(I2C_HandleTypeDef *I2Cx, MPU6050_GyroRange gyRange);

/**
 * @brief Set Sampling Rate
 * @param current i2c interface for MPU6050
 * @param MPU6050 sampling rate
 * @return process result
 */
bool MPU6050SetSamplingRate(I2C_HandleTypeDef *I2Cx, MPU6050_SamplingRate sr);

/**
 * @brief Enable interrupt
 * @param current i2c interface for MPU6050
 * @return process result
 */
bool MPU6050EnableInterrupts(I2C_HandleTypeDef *I2Cx);

/**
 * @brief Init all configuration at once
 * @param current i2c interface for MPU6050
 * @return process result
 */
bool MPU6050Init(I2C_HandleTypeDef *I2Cx, MPU6050_AccRange accRange,
		MPU6050_GyroRange gyroRange, MPU6050_SamplingRate sr);

/**
 * @brief read accelerometer values
 * @param current i2c interface for MPU6050
 * @return process result
 */
bool MPU6050ReadAccelValues(I2C_HandleTypeDef *I2Cx);

//**************** MPU6050 CODES ****************//

//**************** UTILS CODES ****************//

/**
 * @brief Find I2C Dev with searching spesific range (@startAddr - @endAddr),
 * @param startAddr, start address for searching
 * @param endAddr, end address
 * return bool, process result
 */
bool MPU6050FindDevice(I2C_HandleTypeDef *I2Cx, uint8_t startAddr,
		uint8_t endAddr);

/**
 * @brief Check properties before do any process (like mpu6050 address, i2cdev interface etc...)
 * @return bool, process result
 */
bool MPU6050CheckProperties();

/**
 * Calibration offset and multiple values defined at pages 29, 30, 31 in MPU-6000 and
 * MPU-6050 Register Map and Descriptions Revision 4.2 PDF.
 *
 * From PDF,
 * Accelerometer calibration values
 * AFS_SEL 		|		Full Scale	Range	|		 LSB Sensitivity
 * -----------------------------------------------------------------
 * 0 			|			±2g 			|			16384 LSB/g
 * 1			|		    ±4g 			|			8192 LSB/g
 * 2 			|			±8g 			|			4096 LSB/g
 * 3 			|			±16g 			|			2048 LSB/g
 *
 * Gyroscope calibration values
 * FS_SEL 		|		Full Scale Range	| 		LSB Sensitivity
 * --------------------------------------------------------------------
 * 0 			|			± 250 °/s 		|			131 LSB/°/s
 * 1 			|			± 500 °/s 		|			65.5 LSB/°/s
 * 2 			|			± 1000 °/s 		|			32.8 LSB/°/s
 * 3 			|			± 2000 °/s 		|			16.4 LSB/°/s
 *
 * Temperature calibration offset and multiple value:
 * Temperature in degrees C = (raw temp) / 340 + 36.53
 * @brief Calibrate data with offset and multiples
 */
void MPU6050CalibrateData();

//**************** ERROR CODES ****************//

/**
 * @brief Error status changed, set new error code to last error code
 */
void MPU6050ErrorOccured(MPU6050_ErrorCode errorCode);

/**
 * @brief Last error code
 * @return MPU6050ErrorCode
 */
MPU6050_ErrorCode MPU6050LastErrorCode();

/**
 * @brief Last error string of current error code
 * @return const char* from ERROR_CODE_STRING
 */
const char* MPU6050LastErrorString();

//**************** ERROR CODES ****************//
