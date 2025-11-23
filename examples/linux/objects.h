#ifndef USER_OBJECTS_H
#define USER_OBJECTS_H

#include "WppClient.h"

using namespace wpp;

#define TEST_SERVER_SHORT_ID 123

/* ------------- Methods to init objects ------------- */
void serverInit(WppClient &client);

void securityInit(WppClient &client);

void deviceInit(WppClient &client);

#ifdef OBJ_O_2_LWM2M_ACCESS_CONTROL
void acInit(WppClient &client);
#endif

#ifdef OBJ_O_4_CONNECTIVITY_MONITORING
void connMonitoringInit(WppClient &client);
#endif

#ifdef OBJ_O_3339_AUDIO_CLIP
void audioClipInit(WppClient &client);
#endif

#ifdef OBJ_O_6_location
void 6_locationInit(WppClient &client);
#endif

	#ifdef OBJ_o_3201_digital_output
	void 3201_digital_outputInit(WppClient &client);
	#endif

	#ifdef OBJ_o_3202_analog_input
	void 3202_analog_inpInit(WppClient &client);
	#endif

	#ifdef OBJ_o_3203_analog_output
	void 3203_analog_outputInit(WppClient &client);
	#endif

	#ifdef OBJ_o_3300_generic_sensor
	void 3300_generic_sensorInit(WppClient &client);
	#endif

	#ifdef OBJ_o_3301_temperature
	void 3301_temperatureInit(WppClient &client);
	#endif

	#ifdef OBJ_o_3303_temperature
	void 3303_temperatureInit(WppClient &client);
	#endif

	#ifdef OBJ_o_3311_light_control
	void 3311_light_controlInit(WppClient &client);
	#endif

	#ifdef OBJ_o_3313_accelerometer
	void 3313_accelerometerInit(WppClient &client);
	#endif

	#ifdef OBJ_o_3316_voltage
	void 3316_voltageInit(WppClient &client);
	#endif

	#ifdef OBJ_o_3317_current
	void 3317_currentInit(WppClient &client);
	#endif

	#ifdef OBJ_o_3322_load
	void 3322_loadInit(WppClient &client);
	#endif

	#ifdef OBJ_o_3323_pressure
	void 3323_pressureInit(WppClient &client);
	#endif

	#ifdef OBJ_o_3328_power
	void 3328_powerInit(WppClient &client);
	#endif

	#ifdef OBJ_o_3336_location
	void 3336_locationInit(WppClient &client);
	#endif

	#ifdef OBJ_o_3342_on_off_switch
	void 3342_on_off_switchInit(WppClient &client);
	#endif

	#ifdef OBJ_o_3347_push_button
	void 3347_push_buttonInit(WppClient &client);
	#endif

	#ifdef OBJ_o_3311_light_control
	void 3311_light_controlInit(WppClient &client);
	#endif

	#ifdef OBJ_o_3313_accelerometer
	void 3313_accelerometerInit(WppClient &client);
	#endif

	#ifdef OBJ_o_3316_voltage
	void 3316_voltageInit(WppClient &client);
	#endif

	#ifdef OBJ_o_3317_current
	void 3317_currentInit(WppClient &client);
	#endif

	#ifdef OBJ_o_3322_load
	void locationInit(WppClient &client);
	#endif

	#ifdef OBJ_o_3323_pressure
	void 3323_pressureInit(WppClient &client);
	#endif

	#ifdef OBJ_o_3328_power
	void 3328_powerInit(WppClient &client);
	#endif

	#ifdef OBJ_o_3336_location
	void 3336_locationInit(WppClient &client);
	#endif

	#ifdef OBJ_o_3342_on_off_switch
	void 3342_on_off_switchInit(WppClient &client);
	#endif

	#ifdef OBJ_o_3347_push_button	
	void 3347_push_buttonInit(WppClient &client);
	#endif

/* ------------- Helpful methods ------------- */
bool isDeviceShouldBeRebooted();

#endif //USER_OBJECTS_H