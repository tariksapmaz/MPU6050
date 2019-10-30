/*
 *  mpu6050.c
 *
 *  Created on: Sep 28, 2019
 *      Author: Tarık Sapmaz
 */

#include "mpu6050.h"

//**************** MPU6050 CODES ****************//

bool MPU6050WakeUpSensor(I2C_HandleTypeDef *I2Cx) {

	if (MPU6050CheckProperties() == false)
		return false;

	HAL_StatusTypeDef halI2CResult;
	uint8_t i2c_buffer[8];
	i2c_buffer[0] = MPU6050_REGISTER_ADDR_POWER_MANAGEMENT_2;
	i2c_buffer[1] = 0x00;

	halI2CResult = HAL_I2C_Master_Transmit(I2Cx, MPU6050Properties.I2CAddress,
			i2c_buffer, 2, MPU6050_DEFAULT_TIMEOUT_VALUE);
	if (halI2CResult == HAL_TIMEOUT) {
		MPU6050ErrorOccured(TimeoutI2CInterface);
		return false;
	} else if (halI2CResult == HAL_ERROR) {
		MPU6050ErrorOccured(ErrorI2CInterface);
		return false;
	} else if (halI2CResult == HAL_BUSY) {
		MPU6050ErrorOccured(BusyI2CInterface);
		return false;
	}

	return true;
}

bool MPU6050SetAccelerometerRange(I2C_HandleTypeDef *I2Cx,
		MPU6050_AccRange accRange) {

	if (MPU6050CheckProperties() == false)
		return false;

	HAL_StatusTypeDef halI2CResult;
	uint8_t i2c_buffer[8];

	// Set Accelerometer Range
	i2c_buffer[0] = MPU6050_REGISTER_ADDR_ACCEL_RANGE_CONFIG;
	i2c_buffer[1] = accRange << 3;
	halI2CResult = HAL_I2C_Master_Transmit(I2Cx, MPU6050Properties.I2CAddress,
			i2c_buffer, 2, MPU6050_DEFAULT_TIMEOUT_VALUE);
	if (halI2CResult == HAL_TIMEOUT) {
		MPU6050ErrorOccured(TimeoutI2CInterface);
		return false;
	} else if (halI2CResult == HAL_ERROR) {
		MPU6050ErrorOccured(ErrorI2CInterface);
		return false;
	} else if (halI2CResult == HAL_BUSY) {
		MPU6050ErrorOccured(BusyI2CInterface);
		return false;
	}

	// Assign Variables
	MPU6050Properties.accRange = accRange;

	return true;
}

bool MPU6050SetGyroRange(I2C_HandleTypeDef *I2Cx, MPU6050_GyroRange gyRange) {

	if (MPU6050CheckProperties() == false)
		return false;

	HAL_StatusTypeDef halI2CResult;
	uint8_t i2c_buffer[8];

	// Set Gyroscope Range
	i2c_buffer[0] = MPU6050_REGISTER_ADDR_GYRO_RANGE_CONFIG;
	i2c_buffer[1] = gyRange << 3;

	halI2CResult = HAL_I2C_Master_Transmit(I2Cx, MPU6050Properties.I2CAddress,
			i2c_buffer, 2, MPU6050_DEFAULT_TIMEOUT_VALUE);
	if (halI2CResult == HAL_TIMEOUT) {
		MPU6050ErrorOccured(TimeoutI2CInterface);
		return false;
	} else if (halI2CResult == HAL_ERROR) {
		MPU6050ErrorOccured(ErrorI2CInterface);
		return false;
	} else if (halI2CResult == HAL_BUSY) {
		MPU6050ErrorOccured(BusyI2CInterface);
		return false;
	}

	// Assign Variables
	MPU6050Properties.gyroRange = gyRange;

	return true;
}

bool MPU6050SetSamplingRate(I2C_HandleTypeDef *I2Cx, MPU6050_SamplingRate sr) {

	if (MPU6050CheckProperties() == false)
		return false;

	uint8_t i2c_buffer[8];
	i2c_buffer[0] = MPU6050_REGISTER_ADDR_SAMPLING_RATE;
	i2c_buffer[1] = sr;

	HAL_StatusTypeDef halI2CResult = HAL_I2C_Master_Transmit(I2Cx,
			MPU6050Properties.I2CAddress, i2c_buffer, 2,
			MPU6050_DEFAULT_TIMEOUT_VALUE);

	if (halI2CResult == HAL_TIMEOUT) {
		MPU6050ErrorOccured(TimeoutI2CInterface);
		return false;
	} else if (halI2CResult == HAL_ERROR) {
		MPU6050ErrorOccured(ErrorI2CInterface);
		return false;
	} else if (halI2CResult == HAL_BUSY) {
		MPU6050ErrorOccured(BusyI2CInterface);
		return false;
	}

	// Assign Variables
	MPU6050Properties.srRate = sr;

	return true;
}

bool MPU6050EnableInterrupts(I2C_HandleTypeDef *I2Cx) {

	if (MPU6050CheckProperties() == false)
		return false;

	uint8_t i2c_buffer[8];
	uint8_t temp;
	i2c_buffer[0] = MPU6050_REGISTER_ADDR_INTERRUPT_ENABLE;
	i2c_buffer[1] = 0x01;

	HAL_StatusTypeDef halI2CResult;

	// Enable Interrupt
	halI2CResult = HAL_I2C_Master_Transmit(I2Cx, MPU6050Properties.I2CAddress,
			i2c_buffer, 2, MPU6050_DEFAULT_TIMEOUT_VALUE);
	if (halI2CResult == HAL_TIMEOUT) {
		MPU6050ErrorOccured(TimeoutI2CInterface);
		return false;
	} else if (halI2CResult == HAL_ERROR) {
		MPU6050ErrorOccured(ErrorI2CInterface);
		return false;
	} else if (halI2CResult == HAL_BUSY) {
		MPU6050ErrorOccured(BusyI2CInterface);
		return false;
	}

	// Interrupt Configuration
	i2c_buffer[0] = MPU6050_REGISTER_ADDR_INTERRUPT_CONFIG;
	i2c_buffer[1] = 0;

	halI2CResult = HAL_I2C_Master_Transmit(I2Cx, MPU6050Properties.I2CAddress,
			i2c_buffer, 1, MPU6050_DEFAULT_TIMEOUT_VALUE);
	if (halI2CResult == HAL_TIMEOUT) {
		MPU6050ErrorOccured(TimeoutI2CInterface);
		return false;
	} else if (halI2CResult == HAL_ERROR) {
		MPU6050ErrorOccured(ErrorI2CInterface);
		return false;
	} else if (halI2CResult == HAL_BUSY) {
		MPU6050ErrorOccured(BusyI2CInterface);
		return false;
	}

	halI2CResult = HAL_I2C_Master_Receive(I2Cx, MPU6050Properties.I2CAddress,
			&temp, 1, MPU6050_DEFAULT_TIMEOUT_VALUE);
	if (halI2CResult == HAL_TIMEOUT) {
		MPU6050ErrorOccured(TimeoutI2CInterface);
		return false;
	} else if (halI2CResult == HAL_ERROR) {
		MPU6050ErrorOccured(ErrorI2CInterface);
		return false;
	} else if (halI2CResult == HAL_BUSY) {
		MPU6050ErrorOccured(BusyI2CInterface);
		return false;
	}

	temp |= 0x10;
	i2c_buffer[0] = MPU6050_REGISTER_ADDR_INTERRUPT_CONFIG;
	i2c_buffer[1] = temp;
	halI2CResult = HAL_I2C_Master_Receive(I2Cx, MPU6050Properties.I2CAddress,
			i2c_buffer, 2, MPU6050_DEFAULT_TIMEOUT_VALUE);
	if (halI2CResult == HAL_TIMEOUT) {
		MPU6050ErrorOccured(TimeoutI2CInterface);
		return false;
	} else if (halI2CResult == HAL_ERROR) {
		MPU6050ErrorOccured(ErrorI2CInterface);
		return false;
	} else if (halI2CResult == HAL_BUSY) {
		MPU6050ErrorOccured(BusyI2CInterface);
		return false;
	}

	return true;
}

bool MPU6050Init(I2C_HandleTypeDef *I2Cx, MPU6050_AccRange accRange,
		MPU6050_GyroRange gyroRange, MPU6050_SamplingRate sr) {

	// Reset All Variables
	MPU6050Properties.I2CAddress = 0xD0;

	MPU6050ErrorProperties.errorCode = NoError;
	MPU6050ErrorProperties.errorString = MPU6050_ERROR_STRING[NoError];

	MPU6050Properties.xRawAccValue = 0;
	MPU6050Properties.yRawAccValue = 0;
	MPU6050Properties.zRawAccValue = 0;

	MPU6050Properties.xRawGyroValue = 0;
	MPU6050Properties.yRawGyroValue = 0;
	MPU6050Properties.zRawGyroValue = 0;

	MPU6050Properties.xAccValue = 0.0;
	MPU6050Properties.yAccValue = 0.0;
	MPU6050Properties.zAccValue = 0.0;

	MPU6050Properties.xGyroValue = 0.0;
	MPU6050Properties.yGyroValue = 0.0;
	MPU6050Properties.zGyroValue = 0.0;

	if (MPU6050FindDevice(I2Cx, 0, 255) == false)
		return false;

	if (MPU6050WakeUpSensor(I2Cx) == false)
		return false;

	if (MPU6050SetAccelerometerRange(I2Cx, accRange) == false)
		return false;

	if (MPU6050SetGyroRange(I2Cx, gyroRange) == false)
		return false;

	if (MPU6050SetSamplingRate(I2Cx, sr) == false)
		return false;

	if (MPU6050EnableInterrupts(I2Cx) == false)
		return false;

	return true;
}

bool MPU6050ReadAccelValues(I2C_HandleTypeDef *I2Cx) {
	if (MPU6050CheckProperties() == false)
		return false;

	HAL_StatusTypeDef halI2CResult;
	uint8_t i2c_buffer[15];

	// Read Accelerometer Data
	// Request to read
	i2c_buffer[0] = MPU6050_REGISTER_ADDR_ACCEL_XOUT_H;
	halI2CResult = HAL_I2C_Master_Transmit(I2Cx, MPU6050Properties.I2CAddress,
			i2c_buffer, 1, MPU6050_DEFAULT_TIMEOUT_VALUE);
	if (halI2CResult == HAL_TIMEOUT) {
		MPU6050ErrorOccured(TimeoutI2CInterface);
		return false;
	} else if (halI2CResult == HAL_ERROR) {
		MPU6050ErrorOccured(ErrorI2CInterface);
		return false;
	} else if (halI2CResult == HAL_BUSY) {
		MPU6050ErrorOccured(BusyI2CInterface);
		return false;
	}

	// Read Data
	i2c_buffer[1] = 0x00;
	halI2CResult = HAL_I2C_Master_Receive(I2Cx, MPU6050Properties.I2CAddress,
			&i2c_buffer[1], 14, MPU6050_DEFAULT_TIMEOUT_VALUE);
	if (halI2CResult == HAL_TIMEOUT) {
		MPU6050ErrorOccured(TimeoutI2CInterface);
		return false;
	} else if (halI2CResult == HAL_ERROR) {
		MPU6050ErrorOccured(ErrorI2CInterface);
		return false;
	} else if (halI2CResult == HAL_BUSY) {
		MPU6050ErrorOccured(BusyI2CInterface);
		return false;
	}

	MPU6050Properties.xRawAccValue = (i2c_buffer[1] << 8 | i2c_buffer[2]);
	MPU6050Properties.yRawAccValue = (i2c_buffer[3] << 8 | i2c_buffer[4]);
	MPU6050Properties.zRawAccValue = (i2c_buffer[5] << 8 | i2c_buffer[6]);

	MPU6050Properties.xRawGyroValue = (i2c_buffer[9] << 8 | i2c_buffer[10]);
	MPU6050Properties.yRawGyroValue = (i2c_buffer[11] << 8 | i2c_buffer[12]);
	MPU6050Properties.zRawGyroValue = (i2c_buffer[13] << 8 | i2c_buffer[14]);

	MPU6050Properties.rawTempValue = (i2c_buffer[7] << 8 | i2c_buffer[8]);

	MPU6050CalibrateData();

	return true;
}

//**************** MPU6050 CODES ****************//

//**************** UTILS CODES ****************//

bool MPU6050FindDevice(I2C_HandleTypeDef *I2Cx, uint8_t startAddr,
		uint8_t endAddr) {
	// Detect Address MPU6050
	for (uint8_t i = 0; i < 255; ++i) {
		if (HAL_I2C_IsDeviceReady(I2Cx, i, 1, MPU6050_DEFAULT_TIMEOUT_VALUE)
				== HAL_OK) {
			MPU6050Properties.I2CAddress = i;
			return true;
		}
	}

	MPU6050ErrorOccured(UninitAddr);
	return false;
}

bool MPU6050CheckProperties() {
	if (MPU6050Properties.I2CAddress == 0) {
		MPU6050ErrorOccured(UninitAddr);
		return false;
	}

	return true;
}

void MPU6050CalibrateData() {
	float accCalibValue = 0.0f, gyroCalibValue = 0.0f;

	switch (MPU6050Properties.accRange) {
	case AR2:
		accCalibValue = 16384.0f;
		break;
	case AR4:
		accCalibValue = 8192.0f;
		break;
	case AR8:
		accCalibValue = 4096.0f;
		break;
	case AR16:
		accCalibValue = 2048.0f;
		break;
	}

	switch (MPU6050Properties.gyroRange) {
	case GR250:
		gyroCalibValue = 131.0f;
		break;
	case GR500:
		gyroCalibValue = 65.5f;
		break;
	case GR1000:
		gyroCalibValue = 32.8f;
		break;
	case GR2000:
		gyroCalibValue = 16.4f;
		break;
	}

	MPU6050Properties.xAccValue = MPU6050Properties.xRawAccValue
			/ accCalibValue;
	MPU6050Properties.yAccValue = MPU6050Properties.yRawAccValue
			/ accCalibValue;
	MPU6050Properties.zAccValue = MPU6050Properties.zRawAccValue
			/ accCalibValue;

	MPU6050Properties.xGyroValue = MPU6050Properties.xRawGyroValue
			/ gyroCalibValue;
	MPU6050Properties.yGyroValue = MPU6050Properties.yRawGyroValue
			/ gyroCalibValue;
	MPU6050Properties.zGyroValue = MPU6050Properties.zRawGyroValue
			/ gyroCalibValue;

	MPU6050Properties.tempValue = -(MPU6050Properties.rawTempValue / 340.0f)
			+ 36.53f;
}

//**************** UTILS CODES ****************//

//**************** ERROR CODES ****************//

void MPU6050ErrorOccured(MPU6050_ErrorCode errorCode) {
	MPU6050ErrorProperties.errorCode = errorCode;
	MPU6050ErrorProperties.errorString = MPU6050_ERROR_STRING[errorCode];
}

MPU6050_ErrorCode MPU6050LastErrorCode() {
	return MPU6050ErrorProperties.errorCode;
}

const char* MPU6050LastErrorString() {
	return MPU6050ErrorProperties.errorString;
}

//**************** ERROR CODES ****************//
