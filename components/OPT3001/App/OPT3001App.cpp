/*
 * OPT3001App.cpp
 *
 *  Created on: Dec 11, 2019
 *      Author: amit
 */
#include "OPT3001App.h"

#include "../../../components/ErrorHandler/errorHandler.h"

static char OPT3001_APP_TAG[]="OPT3001 APP: ";
#define OPT3001_ADDRESS 0x45  //48

OPT3001App::OPT3001App() {
	try{
		this->sensor=new ClosedCube_OPT3001;
		this->sensor->begin(OPT3001_ADDRESS);

	}
	catch(exception& err){
		ESP_LOGW(OPT3001_APP_TAG,"{destructor}, Exception: %s",err.what());
	}
	catch(emu_err_t& err){
		ESP_LOGW(OPT3001_APP_TAG,"{destructor}, Exception: %s",errorHandler::printError(err));
	}
}

OPT3001App::~OPT3001App() {
	try{
		if(this->sensor!=NULL){
			delete this->sensor;
		}
	}
	catch(exception& err){
		ESP_LOGW(OPT3001_APP_TAG,"{destructor}, Exception: %s",err.what());
	}
	catch(emu_err_t& err){
		ESP_LOGW(OPT3001_APP_TAG,"{destructor}, Exception: %s",errorHandler::printError(err));
	}
}

void OPT3001App::displayMeasurement(OPTMeasurement_t measurement){

	try{
		ESP_LOGI(OPT3001_APP_TAG,"Temperature = %f (C)",measurement.LUX);

	}
	catch(exception& err){
		ESP_LOGW(OPT3001_APP_TAG,"{destructor}, Exception: %s",err.what());
	}
	catch(emu_err_t& err){
		ESP_LOGW(OPT3001_APP_TAG,"{destructor}, Exception: %s",errorHandler::printError(err));
	}
}

bool OPT3001App::measure(OPTMeasurement_t *measurement) {

	try{
		measurement->LUX=this->sensor->readResult().lux;

		return true;
	}
	catch(exception& err){
		ESP_LOGW(OPT3001_APP_TAG,"{destructor}, Exception: %s",err.what());
		return false;
	}
	catch(emu_err_t& err){
		ESP_LOGW(OPT3001_APP_TAG,"{destructor}, Exception: %s",errorHandler::printError(err));
		return false;
	}

}



