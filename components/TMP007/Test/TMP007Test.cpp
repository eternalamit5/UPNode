/*
 * TMP007Test.cpp
 *
 *  Created on: Dec 11, 2019
 *      Author: amit
 */

#include "stdlib.h"
#include "../components/TMP007/Adafruit_TMP007.h"
#include "../arduino-esp32/libraries/Wire/src/Wire.h"
#include "../components/Adafruit_Common_Sensor/Adafruit_Sensor.h"
#include "esp_log.h"

using namespace std;

static char TMP007_TAG[] = "TMP007: ";

//#define SEALEVELPRESSURE_HPA (1013.25)

void takeMeasurement(Adafruit_TMP007 &tmp) {
	ESP_LOGI(TMP007_TAG, "RAW Die Temperature = %d (C)",
			tmp.readRawDieTemperature());
	ESP_LOGI(TMP007_TAG, "Raw voltage Temperature = %d (C)",
			tmp.readRawVoltage());
	ESP_LOGI(TMP007_TAG, "object Temperature = %f (C)", tmp.readObjTempC());
	ESP_LOGI(TMP007_TAG, "Die Temperature = %f (C)", tmp.readDieTempC());

}

void TMP007Task(void *arg) {

	try {
		//Create BME instance
		Adafruit_TMP007 *tmpInstance = new Adafruit_TMP007;
		Adafruit_TMP007 &tmp = *tmpInstance;



		//Init tmp in I2C mode
		if (!tmp.begin()) {
			ESP_LOGW(TMP007_TAG, "{TMP007_I2C_setup}, begin failed, FAIL");
			vTaskDelete(NULL);
		}
		vTaskDelay(1000);

		while (1) {
			takeMeasurement(tmp);
			vTaskDelay(100);
		}
	} catch (exception &err) {
		ESP_LOGW(TMP007_TAG, "{addTopic}, Exception: %s", err.what());
		//ret=EMU_FAILURE;
	}

	vTaskDelete(NULL);

}

