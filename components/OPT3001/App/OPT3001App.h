/*
 * OPT3001App.h
 *
 *  Created on: Dec 11, 2019
 *      Author: amit
 */

#ifndef COMPONENTS_OPT3001_APP_OPT3001APP_H_
#define COMPONENTS_OPT3001_APP_OPT3001APP_H_
#include "../../../components/OPT3001/ClosedCube_OPT3001.h"

using namespace std;

typedef struct{
	float LUX;
}OPTMeasurement_t;

class OPT3001App{
private:
	ClosedCube_OPT3001* sensor;

public:
	/*
	 * Constructor
	 * @param
	 */
	OPT3001App();


	virtual ~OPT3001App();

	/*
	 * Take measurement from BME 280 (sensor measurement is taken here)
	 * @param measurement: BMEmeasurement contains
	 * 						> temperature,
	 * 						> pressure,
	 * 						> altitude,
	 * 						> humidity
	 */
	bool measure(OPTMeasurement_t* measurement);

	/*
	 * Display measurement stored measurement (measurement is not taken here)
	 * @param measurement: BMEmeasurement contains
	 * 						> temperature,
	 * 						> pressure,
	 * 						> altitude,
	 * 						> humidity
	 */
	void displayMeasurement(OPTMeasurement_t measurement);
};



#endif /* COMPONENTS_OPT3001_APP_OPT3001APP_H_ */
