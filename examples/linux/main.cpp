#include <iostream>
#include <thread>
#include <chrono>

#include "Connection.h"
#include "objects.h"

using namespace std;
using namespace wpp;

void socketPolling(Connection *connection) {
	while (!isDeviceShouldBeRebooted()) {
		connection->loop();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void wppErrorHandler(WppClient &client, int errCode) {
	cout << "wppErrorHandler(): Error: " << errCode << endl;
	if (client.getState() == STATE_BOOTSTRAPPING || client.getState() == STATE_BOOTSTRAP_REQUIRED) {
		cout << "Trying to restore security and server objects" << endl;
		Object &securityObj = Lwm2mSecurity::object(client);
		Object &serverObj = Lwm2mServer::object(client);

		#if OBJ_O_2_LWM2M_ACCESS_CONTROL
		for (auto *inst : securityObj.instances()) Lwm2mAccessControl::remove(*inst);
		for (auto *inst : serverObj.instances()) Lwm2mAccessControl::remove(*inst);
		#endif

		securityObj.clear();
		serverObj.clear();

		securityInit(client);
		serverInit(client);
	}
}

// So this is the entire core interface for wpp::WppRegistry, it allows to register objects and access them.
bool objects_registering(WppClient &client) {
    WppRegistry &registry = client.registry();
 
    if (registry.isExist(OBJ_ID::CONNECTIVITY_MONITORING) == false) return false;
    if (registry.isExist(OBJ_ID::LWM2M_ACCESS_CONTROL) == false) return false;
    if (registry.isExist(OBJ_ID::FIRMWARE_UPDATE) == false) return false;
	if (registry.isExist(OBJ_ID::AUDIO_CLIP) == false) return false;
	if (registry.isExist(OBJ_ID::DIGITAL_OUTPUT) == false) return false;
	if (registry.isExist(OBJ_ID::PUSH_BUTTON) == false) return false;
	if (registry.isExist(OBJ_ID::TEMPERATURE) == false) return false;
	if (registry.isExist(OBJ_ID::ACCELEROMETER) == false) return false;
	if (registry.isExist(OBJ_ID::LOCATION) == false) return false;
	if (registry.isExist(OBJ_ID::ANALOG_INPUT) == false) return false;
	if (registry.isExist(OBJ_ID::ANALOG_OUTPUT) == false) return false;
	if (registry.isExist(OBJ_ID::GENERIC_SENSOR) == false) return false;
	if (registry.isExist(OBJ_ID::LIGHT_CONTROL) == false) return false;
	if (registry.isExist(OBJ_ID::VOLTAGE) == false) return false;
	if (registry.isExist(OBJ_ID::CURRENT) == false) return false;
	if (registry.isExist(OBJ_ID::LOAD) == false) return false;
	if (registry.isExist(OBJ_ID::PRESSURE) == false) return false;
	if (registry.isExist(OBJ_ID::POWER) == false) return false;
	if (registry.isExist(OBJ_ID::LOCATION1) == false) return false;
	if (registry.isExist(OBJ_ID::ON_OFF_SWITCH) == false) return false;
	
 
    Object &connMon = registry.connectivityMonitoring();
    Object &acl = registry.lwm2mAccessControl();
    Object *fwUpd = registry.object(OBJ_ID::FIRMWARE_UPDATE);
	Object &audioClip = registry.object(OBJ_ID::AUDIO_CLIP);
	Object &digitalOutput = registry.object(OBJ_ID::DIGITAL_OUTPUT);
	Object &pushButton = registry.object(OBJ_ID::PUSH_BUTTON);
	Object &temperature = registry.object(OBJ_ID::TEMPERATURE);
	Object &accelerometer = registry.object(OBJ_ID::ACCELEROMETER);
	Object &location = registry.object(OBJ_ID::LOCATION);
	Object &analogInput = registry.object(OBJ_ID::ANALOG_INPUT);
	Object &analogOutput = registry.object(OBJ_ID::ANALOG_OUTPUT);
	Object &genericSensor = registry.object(OBJ_ID::GENERIC_SENSOR);
	Object &lightControl = registry.object(OBJ_ID::LIGHT_CONTROL);
	Object &voltage = registry.object(OBJ_ID::VOLTAGE);
	Object &current = registry.object(OBJ_ID::CURRENT);
	Object &load = registry.object(OBJ_ID::LOAD);
	Object &pressure = registry.object(OBJ_ID::PRESSURE);
	Object &power = registry.object(OBJ_ID::POWER);
	Object &location1 = registry.object(OBJ_ID::LOCATION1);
	Object &onOffSwitch = registry.object(OBJ_ID::ON_OFF_SWITCH);

    registry.registerObj(connMon);
    registry.registerObj(acl);
    registry.registerObj(*fwUpd);
    registry.registerObj(audioClip);
    registry.registerObj(digitalOutput);
    registry.registerObj(pushButton);
    registry.registerObj(temperature);
    registry.registerObj(accelerometer);
    registry.registerObj(location);
    registry.registerObj(analogInput);
    registry.registerObj(analogOutput);
    registry.registerObj(genericSensor);
    registry.registerObj(lightControl);
    registry.registerObj(voltage);
    registry.registerObj(current);
    registry.registerObj(load);
    registry.registerObj(pressure);
    registry.registerObj(power);
    registry.registerObj(location1);
    registry.registerObj(onOffSwitch);
 
	
    if (registry.isObjRegistered(connMon) == false) return false;
    if (registry.isObjRegistered(acl) == false) return false;
    if (registry.isObjRegistered(*fwUpd) == false) return false;
    if (registry.isObjRegistered(audioClip) == false) return false;
    if (registry.isObjRegistered(digitalOutput) == false) return false;
    if (registry.isObjRegistered(pushButton) == false) return false;
    if (registry.isObjRegistered(temperature) == false) return false;
    if (registry.isObjRegistered(accelerometer) == false) return false;
    if (registry.isObjRegistered(location) == false) return false;
    if (registry.isObjRegistered(analogInput) == false) return false;
    if (registry.isObjRegistered(analogOutput) == false) return false;
    if (registry.isObjRegistered(genericSensor) == false) return false;
    if (registry.isObjRegistered(lightControl) == false) return false;
    if (registry.isObjRegistered(voltage) == false) return false;
    if (registry.isObjRegistered(current) == false) return false;
    if (registry.isObjRegistered(load) == false) return false;
    if (registry.isObjRegistered(pressure) == false) return false;
    if (registry.isObjRegistered(power) == false) return false;
    if (registry.isObjRegistered(location1) == false) return false;
    if (registry.isObjRegistered(onOffSwitch) == false) return false;
	
    return true;
}

// Found Wakaama bugs:
// TODO: Device work with NON confirmation messages

int main() {
	cout << endl << "---- Creating required components ----" << endl;
	Connection connection("56830", AF_INET);

	// Client initialization
	cout << endl << "---- Creating WppClient ----" << endl;
	string clientName = "Lwm2mClient";
	#if DTLS_WITH_PSK
	clientName += "PSK";
	#elif DTLS_WITH_RPK
	clientName += "RPK";
	#endif
	cout << "WppClient name: " << clientName << endl;
	WppClient::create({clientName, "", ""}, connection, wppErrorHandler);
	WppClient *client = WppClient::takeOwnershipBlocking();

	// Initialize wpp objects
	#ifdef OBJ_O_2_LWM2M_ACCESS_CONTROL
	acInit(*client);
	#endif
	cout << endl << "---- Initialization wpp Server ----" << endl;
	serverInit(*client);
	cout << endl << "---- Initialization wpp Security ----" << endl;
	securityInit(*client);
	cout << endl << "---- Initialization wpp Device ----" << endl;
	deviceInit(*client);
	#ifdef OBJ_O_4_CONNECTIVITY_MONITORING
	cout << endl << "---- Initialization wpp ConnectivityMonitoring ----" << endl;
	connMonitoringInit(*client);
	#endif
	#ifdef OBJ_O_3339_AUDIO_CLIP
	cout << endl << "---- Initialization wpp AudioClip ----" << endl;
	audioClipInit(*client);
	#endif
	/*
	#ifdef OBJ_O_6_location
	cout << endl << "---- Initialization wpp 6_Location ----" << endl;
	6_locationInit(*client);
	#endif
	#ifdef OBJ_o_3201_digital_output
	cout << endl << "---- Initialization 3201_digital_output ----" << endl;
	3201_digital_outputInit(*client);
	#endif
	#ifdef OBJ_o_3202_analog_input
	cout << endl << "---- Initialization wpp 3202_analog_inp ----" << endl;
	3202_analog_inpInit(*client);
	#endif
	#ifdef OBJ_o_3203_analog_output
	cout << endl << "---- Initialization wpp 3203_analog_output ----" << endl;
	3203_analog_outputInit(*client);
	#endif
	#ifdef OBJ_o_3300_generic_sensor
	cout << endl << "---- Initialization wpp 3300_generic_sensor ----" << endl;
	3300_generic_sensorInit(*client);
	#endif
	#ifdef OBJ_o_3301_temperature
	cout << endl << "---- Initialization wpp 3301_temperature ----" << endl;
	3301_temperatureInit(*client);
	#endif
	#ifdef OBJ_o_3303_temperature
	cout << endl << "---- Initialization wpp 3303_temperature ----" << endl;
	3303_temperatureInit(*client);
	#endif
	#ifdef OBJ_o_3311_light_control
	cout << endl << "---- Initialization wpp 3311_light_control ----" << endl;
	3311_light_controlInit(*client);
	#endif
	#ifdef OBJ_o_3313_accelerometer
	cout << endl << "---- Initialization wpp 3313_accelerometer ----" << endl;
	3313_accelerometerInit(*client);
	#endif
	#ifdef OBJ_o_3316_voltage
	cout << endl << "---- Initialization wpp 3316_voltage ----" << endl;
	3316_voltageInit(*client);
	#endif
	#ifdef OBJ_o_3317_current
	cout << endl << "---- Initialization wpp 3317_current ----" << endl;
	3317_currentInit(*client);
	#endif
	#ifdef OBJ_o_3322_load
	cout << endl << "---- Initialization wpp 3322_load ----" << endl;
	3322_loadInit(*client);
	#endif
	#ifdef OBJ_o_3323_pressure
	cout << endl << "---- Initialization wpp 3323_pressure ----" << endl;
	3323_pressureInit(*client);
	#endif
	#ifdef OBJ_o_3328_power
	cout << endl << "---- Initialization wpp 3328_power ----" << endl;
	3328_powerInit(*client);
	#endif
	#ifdef OBJ_o_3336_location
	cout << endl << "---- Initialization wpp 3336_location ----" << endl;
	3336_locationInit(*client);
	#endif
	#ifdef OBJ_o_3342_on_off_switch
	cout << endl << "---- Initialization wpp 3342_on_off_switch ----" << endl;
	3342_on_off_switchInit(*client);
	#endif
	#ifdef OBJ_o_3347_push_button
	cout << endl << "---- Initialization wpp 3347_push_button ----" << endl;
	3347_push_buttonInit(*client);
	#endif
	#ifdef OBJ_o_3311_light_control
	cout << endl << "---- Initialization wpp 3311_light_control ----" << endl;
	3311_light_controlInit(*client);
	#endif
	#ifdef OBJ_o_3313_accelerometer
	cout << endl << "---- Initialization wpp 3313_accelerometer ----" << endl;
	3313_accelerometerInit(*client);
	#endif
	#ifdef OBJ_o_3316_voltage
	cout << endl << "---- Initialization wpp 3316_voltage ----" << endl;
	3316_voltageInit(*client);
	#endif
	#ifdef OBJ_o_3317_current
	cout << endl << "---- Initialization wpp 3317_current ----" << endl;
	3317_currentInit(*client);
	#endif
	#ifdef OBJ_o_3322_load
	cout << endl << "---- Initialization wpp Location ----" << endl;
	locationInit(*client);
	#endif
	#ifdef OBJ_o_3323_pressure
	cout << endl << "---- Initialization wpp 3323_pressure ----" << endl;
	3323_pressureInit(*client);
	#endif
	#ifdef OBJ_o_3328_power
	cout << endl << "---- Initialization wpp 3328_power ----" << endl;
	3328_powerInit(*client);
	#endif
	#ifdef OBJ_o_3336_location
	cout << endl << "---- Initialization wpp 3336_location ----" << endl;
	3336_locationInit(*client);
	#endif
	#ifdef OBJ_o_3342_on_off_switch
	cout << endl << "---- Initialization wpp 3342_on_off_switch ----" << endl;
	3342_on_off_switchInit(*client);
	#endif
	#ifdef OBJ_o_3347_push_button	
	cout << endl << "---- Initialization wpp 3347_push_button ----" << endl;
	3347_push_buttonInit(*client);
	#endif
*/

	// Giving ownership to registry
	client->giveOwnership();

	// Add tasks with send operation
	#if defined(LWM2M_SUPPORT_SENML_JSON) && RES_1_23 && RES_3_13
	WppTaskQueue::addTask(5, [](WppClient &client, void *ctx) {
		WPP_LOGD(TAG_WPP_TASK, "Task: Send operation, sending current time to the server");
		DataLink dataLink = {{OBJ_ID::DEVICE, 0}, {Device::CURRENT_TIME_13,}};
		client.send(dataLink);
		return false;
	});
	#endif

	cout << endl << "---- Starting Connection thread ----" << endl;
	thread my_thread(socketPolling, &connection);

	time_t callTime = 0;
	for (int iterationCnt = 0; !isDeviceShouldBeRebooted(); iterationCnt++) {
		time_t currTime = time(NULL);

		cout << endl << "---- iteration:" << iterationCnt << ", time: " << time(NULL) << " ----" << endl;
		if (currTime >= callTime || connection.getPacketQueueSize()) {
			// Handle client state and process packets from the server
			client = WppClient::takeOwnership();
			if (client) { 
				callTime = currTime + client->loop();
				client->giveOwnership();
				cout << "Sleep time: " << callTime - time(NULL) << endl;
			}
		}
		this_thread::sleep_for(chrono::seconds(1));
	}

	cout << endl << "---- Closing example ----" << endl;
	my_thread.join();
	WppClient::remove();
	
	return 0;
}

