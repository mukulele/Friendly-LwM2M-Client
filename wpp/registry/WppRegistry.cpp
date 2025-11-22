#include "WppRegistry.h"
#include "WppLogs.h"
#include "liblwm2m.h"

namespace wpp {

WppRegistry::WppRegistry(lwm2m_context_t &context): _context(context) {
    WPP_LOGD(TAG_WPP_REG, "Creating registry instance");
	
	/* ---------- Mandatory objects init block begin ---------- */
	#ifdef OBJ_M_3_DEVICE
	_objects.push_back(new ObjectImpl<Device>(_context, DEVICE_OBJ_INFO));
	#endif
	#ifdef OBJ_M_1_LWM2M_SERVER
	_objects.push_back(new ObjectImpl<Lwm2mServer>(_context, LWM2M_SERVER_OBJ_INFO));
	#endif
	#ifdef OBJ_M_0_LWM2M_SECURITY
	_objects.push_back(new ObjectImpl<Lwm2mSecurity>(_context, LWM2M_SECURITY_OBJ_INFO));
	#endif
	/* ---------- Mandatory objects init block end ---------- */

	/* ---------- Optional objects init block begin ---------- */
	#ifdef OBJ_O_4_CONNECTIVITY_MONITORING
	_objects.push_back(new ObjectImpl<ConnectivityMonitoring>(_context, CONNECTIVITY_MONITORING_OBJ_INFO));
	#endif
	#ifdef OBJ_O_2_LWM2M_ACCESS_CONTROL
	_objects.push_back(new ObjectImpl<Lwm2mAccessControl>(_context, LWM2M_ACCESS_CONTROL_OBJ_INFO));
	#endif
	#if OBJ_O_3339_AUDIO_CLIP
	_objects.push_back(new ObjectImpl<AudioClip>(_context, AUDIO_CLIP_OBJ_INFO));
	#endif
	#if OBJ_O_5_FIRMWARE_UPDATE
	_objects.push_back(new ObjectImpl<FirmwareUpdate>(_context, FIRMWARE_UPDATE_OBJ_INFO));
	#endif
	#if OBJ_O_3201_DIGITAL_OUTPUT
	_objects.push_back(new ObjectImpl<DigitalOutput>(_context, DIGITAL_OUTPUT_OBJ_INFO));
	#endif
	#if OBJ_O_3347_PUSH_BUTTON
	_objects.push_back(new ObjectImpl<PushButton>(_context, PUSH_BUTTON_OBJ_INFO));
	#endif
	#if OBJ_O_3303_TEMPERATURE
	_objects.push_back(new ObjectImpl<Temperature>(_context, TEMPERATURE_OBJ_INFO));
	#endif
	#if OBJ_O_3313_ACCELEROMETER
	_objects.push_back(new ObjectImpl<Accelerometer>(_context, ACCELEROMETER_OBJ_INFO));
	#endif
	#if OBJ_O_6_LOCATION
	_objects.push_back(new ObjectImpl<Location>(_context, LOCATION_OBJ_INFO));
	#endif
	#if OBJ_O_3202_ANALOG_INPUT
	_objects.push_back(new ObjectImpl<AnalogInput>(_context, ANALOG_INPUT_OBJ_INFO));
	#endif
	#if OBJ_O_3203_ANALOG_OUTPUT
	_objects.push_back(new ObjectImpl<AnalogOutput>(_context, ANALOG_OUTPUT_OBJ_INFO));
	#endif
	#if OBJ_O_3300_GENERIC_SENSOR
	_objects.push_back(new ObjectImpl<GenericSensor>(_context, GENERIC_SENSOR_OBJ_INFO));
	#endif
	#if OBJ_O_3311_LIGHT_CONTROL
	_objects.push_back(new ObjectImpl<LightControl>(_context, LIGHT_CONTROL_OBJ_INFO));
	#endif
	#if OBJ_O_3316_VOLTAGE
	_objects.push_back(new ObjectImpl<Voltage>(_context, VOLTAGE_OBJ_INFO));
	#endif
	#if OBJ_O_3317_CURRENT
	_objects.push_back(new ObjectImpl<Current>(_context, CURRENT_OBJ_INFO));
	#endif
	#if OBJ_O_3322_LOAD
	_objects.push_back(new ObjectImpl<Load>(_context, LOAD_OBJ_INFO));
	#endif
	#if OBJ_O_3323_PRESSURE
	_objects.push_back(new ObjectImpl<Pressure>(_context, PRESSURE_OBJ_INFO));
	#endif
	#if OBJ_O_3328_POWER
	_objects.push_back(new ObjectImpl<Power>(_context, POWER_OBJ_INFO));
	#endif
	#if OBJ_O_3336_LOCATION
	_objects.push_back(new ObjectImpl<Location1>(_context, LOCATION_OBJ_INFO));
	#endif
	#if OBJ_O_3342_ON_OFF_SWITCH
	_objects.push_back(new ObjectImpl<On_offSwitch>(_context, ON_OFF_SWITCH_OBJ_INFO));
	#endif
	/* ---------- Optional objects init block end ---------- */
}

WppRegistry::~WppRegistry() {
	for (auto obj : _objects) {
		delete obj;
	}
	_objects.clear();
}

bool WppRegistry::registerObj(Object &object) {
	WPP_LOGD(TAG_WPP_CLIENT, "Register object with id: %d", object.getObjectID());
	return !lwm2m_add_object(&_context, &object.getLwm2mObject());
}

bool WppRegistry::deregisterObj(Object &object) {
	WPP_LOGD(TAG_WPP_CLIENT, "Deregister object with id: %d", object.getObjectID());
	return !lwm2m_remove_object(&_context, object.getLwm2mObject().objID);
}

bool WppRegistry::isObjRegistered(Object &object) {
	lwm2m_object_t * lwm2m_object = (lwm2m_object_t *)LWM2M_LIST_FIND(_context.objectList, object.getLwm2mObject().objID);
	return lwm2m_object != NULL;
}

bool WppRegistry::isExist(OBJ_ID objId) {
	return object(objId) != NULL;
}

Object * WppRegistry::object(OBJ_ID objId) {
	auto finder = [objId](const Object *obj) -> bool { return obj->getObjectID() == objId; };
	auto objIter = std::find_if(_objects.begin(), _objects.end(), finder);
	return objIter != _objects.end()? *objIter : NULL;
}

std::vector<Object *> & WppRegistry::objects() {
	return _objects;
}

/* ---------- Mandatory objects method block begin ---------- */
#ifdef OBJ_M_3_DEVICE
Object & WppRegistry::device() {
	return *object(OBJ_ID::DEVICE);
}
#endif
#ifdef OBJ_M_1_LWM2M_SERVER
Object & WppRegistry::lwm2mServer() {
	return *object(OBJ_ID::LWM2M_SERVER);
}
#endif
#ifdef OBJ_M_0_LWM2M_SECURITY
Object & WppRegistry::lwm2mSecurity() {
	return *object(OBJ_ID::LWM2M_SECURITY);
}
#endif
/* ---------- Mandatory objects method block end ---------- */

/* ---------- Optional objects method block begin ---------- */
#ifdef OBJ_O_4_CONNECTIVITY_MONITORING
Object & WppRegistry::connectivityMonitoring() {
	return *object(OBJ_ID::CONNECTIVITY_MONITORING);
}
#endif
#ifdef OBJ_O_2_LWM2M_ACCESS_CONTROL
Object & WppRegistry::lwm2mAccessControl() {
	return *object(OBJ_ID::LWM2M_ACCESS_CONTROL);
}
#endif
#if OBJ_O_3339_AUDIO_CLIP
Object & WppRegistry::audioClip() {
	return *object(OBJ_ID::AUDIO_CLIP);
}
#endif
#if OBJ_O_5_FIRMWARE_UPDATE
Object & WppRegistry::firmwareUpdate() {
	return *object(OBJ_ID::FIRMWARE_UPDATE);
}
#endif
#if OBJ_O_3201_DIGITAL_OUTPUT
Object & WppRegistry::digitalOutput() {
	return *object(OBJ_ID::DIGITAL_OUTPUT);
}
#endif
#if OBJ_O_3347_PUSH_BUTTON
Object & WppRegistry::pushButton() {
	return *object(OBJ_ID::PUSH_BUTTON);
}
#endif
#if OBJ_O_3303_TEMPERATURE
Object & WppRegistry::temperature() {
	return *object(OBJ_ID::TEMPERATURE);
}
#endif
#if OBJ_O_3313_ACCELEROMETER
Object & WppRegistry::accelerometer() {
	return *object(OBJ_ID::ACCELEROMETER);
}
#endif
#if OBJ_O_6_LOCATION
Object & WppRegistry::location() {
	return *object(OBJ_ID::LOCATION);
}
#endif
#if OBJ_O_3202_ANALOG_INPUT
Object & WppRegistry::analogInput() {
	return *object(OBJ_ID::ANALOG_INPUT);
}
#endif
#if OBJ_O_3203_ANALOG_OUTPUT
Object & WppRegistry::analogOutput() {
	return *object(OBJ_ID::ANALOG_OUTPUT);
}
#endif
#if OBJ_O_3300_GENERIC_SENSOR
Object & WppRegistry::genericSensor() {
	return *object(OBJ_ID::GENERIC_SENSOR);
}
#endif
#if OBJ_O_3311_LIGHT_CONTROL
Object & WppRegistry::lightControl() {
	return *object(OBJ_ID::LIGHT_CONTROL);
}
#endif
#if OBJ_O_3316_VOLTAGE
Object & WppRegistry::voltage() {
	return *object(OBJ_ID::VOLTAGE);
}
#endif
#if OBJ_O_3317_CURRENT
Object & WppRegistry::current() {
	return *object(OBJ_ID::CURRENT);
}
#endif
#if OBJ_O_3322_LOAD
Object & WppRegistry::load() {
	return *object(OBJ_ID::LOAD);
}
#endif
#if OBJ_O_3323_PRESSURE
Object & WppRegistry::pressure() {
	return *object(OBJ_ID::PRESSURE);
}
#endif
#if OBJ_O_3328_POWER
Object & WppRegistry::power() {
	return *object(OBJ_ID::POWER);
}
#endif
#if OBJ_O_3336_LOCATION
Object & WppRegistry::location1() {
	return *object(OBJ_ID::LOCATION1);
}
#endif
#if OBJ_O_3342_ON_OFF_SWITCH
Object & WppRegistry::on_offSwitch() {
	return *object(OBJ_ID::ON_OFF_SWITCH);
}
#endif
/* ---------- Optional objects method block end ---------- */

} //wpp
