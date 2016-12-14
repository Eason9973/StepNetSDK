/*
   Copyright 2015 HMD VR

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 */

#ifndef _HMD_H
#define _HMD_H

#include <stdint.h>

#ifdef HMD_EXPORTS
#define HMD_API __declspec(dllexport)
#else
#define HMD_API __declspec(dllimport)
#endif

/*! Three element vector, can either represent a rotation, translation or acceleration. */
typedef struct {
	float x, y, z;
} HMD_VECTOR;

typedef struct {
	uint16_t Timestamp;
	HMD_VECTOR Acceleration;
	HMD_VECTOR Gyroscope;
	HMD_VECTOR Magnetic;
	unsigned int PacketNumber;
} HMD_DATA;

//-- going to redefine -- 

/**
* \defgroup Glove HMD Glove
* @{
*/

#define HMD_ERROR -1
#define HMD_SUCCESS 0
#define HMD_INVALID_ARGUMENT 1
#define HMD_DISCONNECTED 2

#ifdef __cplusplus
extern "C" {
#endif
	/*! \brief Initialize the HMD SDK.
	*
	*  Must be called before any other function in the SDK.
	*  This function should only be called once.
	*/
	HMD_API int HmdInit();

	/*! \brief Shutdown the HMD SDK.
	*
	*  Must be called when the SDK is no longer
	*  needed.
	*/
	HMD_API int HmdExit();
	HMD_API int HmdGetData(HMD_DATA* data, unsigned int timeout = 0);
	HMD_API bool HmdConnected();

	HMD_API int HmdGetTemperature(int16_t *temp, unsigned int timeout);
	HMD_API int HmdGetProximity(int16_t *prox, unsigned int timeout);
	HMD_API int HmdGetButton(uint8_t *button, unsigned int timeout);
	HMD_API int HmdGetLed(uint8_t *led, unsigned int timeout);
	/*HMD_API int HMDGetBatteryVoltage(uint16_t* battery, unsigned int timeout);
	HMD_API int HMDGetBatteryPercentage(uint8_t* battery, unsigned int timeout);

	HMD_API int HMDCalibrate(bool gyro, bool accel, bool fingers);
	HMD_API int HMDPowerOff();*/



#ifdef __cplusplus
}
#endif

/**@}*/

#endif
