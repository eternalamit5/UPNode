#include "../components/BMI160/App/BMI160App.h"
#include "../components/BME280/App/BME280App.h"
#include "../components/LPG_MQTT/lpg_mqtt.h"
#include "../components/LPG_WIFI/lpg_wifi.h"
#include "sdkconfig.h"
#include "esp_log.h"


/*
 * Note Matilda MQTT end point (Next it goes to Grafana):134.102.96.205:1883
 *
 */

static char MATILDA_TAG[]={"Matilda:"};

void matidaTask(void* arg){

	try{
		BMI160App* MotionSensor=new BMI160App(0x69,GYRO_250,ACC_2G);
		BME280App* envSensor=new BME280App(1024);

		if(MotionSensor==NULL || envSensor==NULL){
			//throw "Sensors failed to init";
			throw EMU_ERR_OUT_OF_MEMORY;
		}


		//Start Wifi
		ESP_LOGI(MATILDA_TAG,"{mqttTest} Start WIFI !!!");
		lpg_wifi::startService();
		lpg_wifi* wifi=lpg_wifi::getInstance();
		if(wifi==NULL){
			throw EMU_ERR_OUT_OF_MEMORY;
		}


		while(wifi->getConnectionStatus()==WIFI_DISCONNECTED){
			ESP_LOGI(MATILDA_TAG,"{mqttTest} Connecting to WIFI !!!");
			vTaskDelay(1000);
		}

		//Start Mqtt
		ESP_LOGI(MATILDA_TAG,"{mqttTest} Start MQTT !!!");
		lpg_mqtt_client::startService(CONFIG_BROKER_URL,1883,1000,NULL,1,true,true,"","");
		lpg_mqtt_client* mqttClient=lpg_mqtt_client::getInstance();
		if(mqttClient==NULL){
			throw EMU_ERR_OUT_OF_MEMORY;
		}

		while(mqttClient->getConnectionStatus()==MQTT_DISCONNECTED){
			ESP_LOGI(MATILDA_TAG,"{mqttTest} Connecting to MQTT !!!");
			vTaskDelay(1000);
		}

		ESP_LOGI(MATILDA_TAG,"{mqttTest} Start Sensor loop !!!");
		while(1){
			if(mqttClient->getConnectionStatus()==MQTT_CONNECTED){
				//send sensor value
				BMIMeasurement_t motionMeasurement;
				BMEMeasurement_t envMeasurement;
				char mqttMotionMsg[200]={0};
				char mqttEnvMsg[200]={0};

				MotionSensor->measure(&motionMeasurement);
				envSensor->measure(&envMeasurement);

				//display measurement
				MotionSensor->displayMeasurement(motionMeasurement);
				envSensor->displayMeasurement(envMeasurement);

				//Create MQTT message from measurement
				sprintf(mqttMotionMsg,"MotionSense,DeviceID=0001,SensorType=ACC ACCX=%f,ACCY=%f,ACCZ=%f",
						motionMeasurement.accelerometer_G[0],motionMeasurement.accelerometer_G[1],motionMeasurement.accelerometer_G[2]);

				sprintf(mqttEnvMsg,"EnvSense,DeviceID=0001,SensorType=ENV Pressure=%f,Humidity=%f,Temperature=%f",
						envMeasurement.pressure,envMeasurement.humidity,envMeasurement.temperature);


				ESP_LOGI(MATILDA_TAG,"{mqttTest} Mqtt Motion Msg:   %s",mqttMotionMsg);
				ESP_LOGI(MATILDA_TAG,"{mqttTest} Mqtt Env Msg:   %s",mqttEnvMsg);

				//Publish mqtt message
				mqttClient->publish("Uptime/Telemetry",mqttMotionMsg);
				mqttClient->publish("Uptime/Telemetry",mqttEnvMsg);


			}else{
				ESP_LOGI(MATILDA_TAG,"{mqttTest} Mqtt Connection lost, reconnecting !!!");
			}
			vTaskDelay(1000);
		}
	}
	catch(exception& err){
		ESP_LOGW(MATILDA_TAG,"{matidaTask}, Exception: %s",err.what());
	}
	catch(emu_err_t& err){
		ESP_LOGW(MATILDA_TAG,"{matidaTask}, Exception: %s",errorHandler::printError(err));
	}

	vTaskDelete(NULL);

}

