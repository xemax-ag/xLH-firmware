#include <stdint.h>
#include <Arduino.h>
#include <M5AtomS3.h>
#include <ddsm_ctrl.h>
#include <Adafruit_BNO08x.h>
#include "TOOLBOX.h"
#include "CAN_OPEN.h"
#include "CONFIG.h"
#include "IMU.h"
#include "MUX.h"

#define REPORT_INTVERVALL_US 5000
Adafruit_BNO08x bno08x(-1);
sh2_SensorValue_t sensorValue;
//struct_message_txd outgoingWritings;
euler_t ypr;
uint8_t read_sensor;

IMU::IMU(void) { ; }

void IMU::setup(void)
{
    Wire1.begin(G38, G39, 400000L);
    Wire1.setPins(G38, G39);
    //Wire1.setTimeout(50);
    if (!bno08x.begin_I2C(BNO08x_I2CADDR_DEFAULT, &Wire1, 0))
    {
        /*
        while (1)
        {
            Serial.println("not found");
            delay(10);
        }
        */
    }
    //bno08x.enableReport(SH2_GYRO_INTEGRATED_RV, REPORT_INTVERVALL_US);
    bno08x.enableReport(SH2_ARVR_STABILIZED_RV, REPORT_INTVERVALL_US);
}

void IMU::read_bn085(void)
{
    //static uint16_t micros_start;
    //micros_start = micros();
    // https://simple.wikipedia.org/wiki/Pitch,_yaw,_and_roll
    // https://simple.wikipedia.org/wiki/Pitch,_yaw,_and_roll#/media/File:Yaw_Axis_Corrected.svg
    
    if (bno08x.wasReset())
    {
        //bno08x.enableReport(SH2_GYRO_INTEGRATED_RV, REPORT_INTVERVALL_US);
        bno08x.enableReport(SH2_ARVR_STABILIZED_RV, REPORT_INTVERVALL_US);
    }
    if (bno08x.getSensorEvent(&sensorValue))
    {
        /*
        quaternionToEuler(sensorValue.un.gyroIntegratedRV.real,
            sensorValue.un.gyroIntegratedRV.i,
            sensorValue.un.gyroIntegratedRV.j,
            sensorValue.un.gyroIntegratedRV.k,
            &ypr, true);
        */
       quaternionToEuler(sensorValue.un.arvrStabilizedRV.real,
        sensorValue.un.arvrStabilizedRV.i,
        sensorValue.un.arvrStabilizedRV.j,
        sensorValue.un.arvrStabilizedRV.k,
        &ypr);
    }

    //outgoingWritings.yaw = (int32_t)(ypr.yaw * 1000000.0);
    //outgoingWritings.pitch = (int32_t)(ypr.pitch * 1000000.0);
    //outgoingWritings.roll = (int32_t)(ypr.roll * 1000000.0);
    //outgoingWritings.cycleTime = (int32_t)(micros() - micros_start);

    //Serial.printf("%d #y %0.2f # p %0.2f # r %0.2f # dt %d\r\n", millis(), ypr.yaw, ypr.pitch, ypr.roll, outgoingWritings.cycleTime);
}

void IMU::loop(void)
{
	if (this->init_done == 0)
	{
		;
	}
    
    delay(5);
	this->read_bn085();
    
    this->outImu.fYaw = ypr.yaw;
    this->outImu.fPitch = ypr.pitch;
    this->outImu.fRoll = ypr.roll;
    this->outImu.iYaw = (int16_t)(ypr.yaw * 100.0);// Optimierung 
    this->outImu.iPitch = (int16_t)(ypr.pitch * 100.0);
    this->outImu.iRoll = (int16_t)(ypr.roll * 100.0);

    portENTER_CRITICAL_ISR(&imuMux);
		memcpy(&can_open.outImu, &this->outImu, sizeof(this->outImu));
	portEXIT_CRITICAL_ISR(&imuMux);
}

IMU imu;