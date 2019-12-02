/*
 * GPS_NMEARead.cpp
 *
 *  Created on: Nov 4, 2019
 *      Author: karsh
 */

#include "GPS_NMEARead.h"
#include "Arduino.h"
#include "Wire.h"
#include "MicroNMEA.h" //http://librarymanager/All#MicroNMEA

static const char *GPS_TAG = "GPS ";

SFE_UBLOX_GPS myGPS;
char nmeaBuffer[100];
MicroNMEA nmea(nmeaBuffer, sizeof(nmeaBuffer));


void gpsSetup()
{
		ESP_LOGW(GPS_TAG," Starting gpsSetup");
		Wire.begin(33,32,400000);

	  if (myGPS.begin() == false)
	  {
		  ESP_LOGW(GPS_TAG,"Ublox GPS not detected at default I2C address. Please check wiring. Freezing.");
		  while (1);
	  }

	  	  //This will pipe all NMEA sentences to the serial port so we can see them
	  	  myGPS.setNMEAOutputPort(Serial);
	  	  ESP_LOGW(GPS_TAG," gpsSetup completed");
}

void gpsTask(void* arg)
{
	gpsSetup();
	while(1){
		myGPS.checkUblox(); //See if new data is available. Process bytes as they come in.
		  if(nmea.isValid() == true)
		  {
		    long latitude_mdeg = nmea.getLatitude();
		    long longitude_mdeg = nmea.getLongitude();

		    ESP_LOGI(GPS_TAG,"Latitude (deg): %f",latitude_mdeg / 1000000.0);
		    ESP_LOGI(GPS_TAG,"Longitude (deg): %f",longitude_mdeg / 1000000.0);
		  }
		  else
		  {
			  ESP_LOGI(GPS_TAG,"No Fix - Num. satellites: %d",nmea.getNumSatellites());
		  }
		delay(250); //Don't pound too hard on the I2C bus
	}
}
