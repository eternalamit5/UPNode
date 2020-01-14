/*
 * OPT3001Test.cpp
 *
 *  Created on: Dec 11, 2019
 *      Author: AMIT
 */

#include "stdlib.h"
#include "../ClosedCube_OPT3001.h"
#include "../../../components/arduino-esp32/libraries/Wire/src/Wire.h"
#include "../../../components/Adafruit_Common_Sensor/Adafruit_Sensor.h"
#include "esp_log.h"

using namespace std;

#define OPT3001_ADDRESS 0x45  //48
#define HEX 16
ClosedCube_OPT3001 opt3001;

static char OPT3001_TAG[] = "OPT3001: ";

void takeMeasurement(ClosedCube_OPT3001 &opt) {

	ESP_LOGI(OPT3001_TAG, "OPT3001 Manufacturer ID = %d ",
			opt.readManufacturerID());
	ESP_LOGI(OPT3001_TAG, "OPT3001 Device ID=%d", opt.readDeviceID());

	// configure sensor
	void configureSensor();

	ESP_LOGI(OPT3001_TAG, "High-Limit %f", opt.readHighLimit().lux);
	ESP_LOGI(OPT3001_TAG, "Low-Limit %f", opt.readLowLimit().lux);

	ESP_LOGI(OPT3001_TAG, "---------------------------------");

	ESP_LOGI(OPT3001_TAG, "OPT3001 result %f = ", opt.readResult().lux);
	vTaskDelay(500);

}

void OPT3001Task(void *arg) {

	try {
		//Create OPT instance
		ClosedCube_OPT3001 *optInstance = new ClosedCube_OPT3001;
		ClosedCube_OPT3001 &opt = *optInstance;

		//Init opt in I2C mode
		if (opt.begin(OPT3001_ADDRESS)!=NO_ERROR) {
			ESP_LOGW(OPT3001_TAG, "{OPT3001_I2C_setup}, begin failed, FAIL");
			vTaskDelete(NULL);
		}
		vTaskDelay(1000);

		while (1) {
			takeMeasurement(opt);
			vTaskDelay(100);
		}
	} catch (exception &err) {
		ESP_LOGW(OPT3001_TAG, "{addTopic}, Exception: %s", err.what());
		//ret=EMU_FAILURE;
	}

	vTaskDelete(NULL);

}

void configureSensor(ClosedCube_OPT3001 &opt) {
	OPT3001_Config newConfig;

	newConfig.RangeNumber = 4;
	newConfig.ConvertionTime = 1;
	newConfig.Latch = 1;
	newConfig.ModeOfConversionOperation = 3;

	OPT3001_ErrorCode errorConfig = opt.writeConfig(newConfig);
	if (errorConfig != NO_ERROR) {
		ESP_LOGI(OPT3001_TAG, "OPT3001 configuration %d", errorConfig);
	} else {

		OPT3001_Config sensorConfig = opt.readConfig();
		ESP_LOGI(OPT3001_TAG, "OPT3001 Current Config:");
		ESP_LOGI(OPT3001_TAG, "------------------------------");


		//ESP_LOGI(OPT3001_TAG,"Conversion ready (R): %x"sensorConfig.ConversionReady);
		ESP_LOGI(OPT3001_TAG, "Conversion ready (R): %d",sensorConfig.ConversionReady);

		ESP_LOGI(OPT3001_TAG, "Conversion time (R/W): %d",
				sensorConfig.ConvertionTime);

		ESP_LOGI(OPT3001_TAG, "Fault count field (R/W): %d",
				sensorConfig.FaultCount);

		ESP_LOGI(OPT3001_TAG, "Flag high field (R-only): %d",
				sensorConfig.FlagHigh);

		ESP_LOGI(OPT3001_TAG, "Flag low field (R-only): %d",
				sensorConfig.FlagLow);

		ESP_LOGI(OPT3001_TAG, "Latch field (R/W): %d", sensorConfig.Latch);

		ESP_LOGI(OPT3001_TAG, "Mask exponent field (R/W): %d",
				sensorConfig.MaskExponent);

		ESP_LOGI(OPT3001_TAG, "Mode of conversion operation (R/W): %d",
				sensorConfig.ModeOfConversionOperation);

		ESP_LOGI(OPT3001_TAG, "Polarity field (R/W): %d",
				sensorConfig.Polarity);

		ESP_LOGI(OPT3001_TAG, "Overflow flag (R-only): %d",
				sensorConfig.OverflowFlag);

		ESP_LOGI(OPT3001_TAG, "Range number (R/W): %d",
				sensorConfig.RangeNumber);

		ESP_LOGI(OPT3001_TAG, "------------------------------");
	}

}

