/*
 * GenericSensor
 * Generated on: 2025-11-12 12:56:13
 * Created by: Sinai RnD
 */

#include "o_3300_generic_sensor/GenericSensor.h"

#include "Resource.h"
#include "ItemOp.h"
#include "WppTypes.h"
#include "WppLogs.h"
#include "WppClient.h"

/* --------------- Code_cpp block 0 start --------------- */
/* --------------- Code_cpp block 0 end --------------- */

#define TAG "GenericSensor"

namespace wpp {

GenericSensor::GenericSensor(lwm2m_context_t &context, const OBJ_LINK_T &id): Instance(context, id) {

	/* --------------- Code_cpp block 1 start --------------- */
	/* --------------- Code_cpp block 1 end --------------- */

	resourcesCreate();
	resourcesInit();

	/* --------------- Code_cpp block 2 start --------------- */
	/* --------------- Code_cpp block 2 end --------------- */
}

GenericSensor::~GenericSensor() {
	/* --------------- Code_cpp block 3 start --------------- */
	/* --------------- Code_cpp block 3 end --------------- */
}

Object & GenericSensor::object(WppClient &ctx) {
	return ctx.registry().genericSensor();
}

GenericSensor * GenericSensor::instance(WppClient &ctx, ID_T instId) {
	Instance *inst = ctx.registry().genericSensor().instance(instId);
	if (!inst) return NULL;
	return static_cast<GenericSensor*>(inst);
}

GenericSensor * GenericSensor::createInst(WppClient &ctx, ID_T instId) {
	Instance *inst = ctx.registry().genericSensor().createInstance(instId);
	if (!inst) return NULL;
	return static_cast<GenericSensor*>(inst);
}

bool GenericSensor::removeInst(WppClient &ctx, ID_T instId) {
	return ctx.registry().genericSensor().remove(instId);
}

void GenericSensor::serverOperationNotifier(Instance *securityInst, ItemOp::TYPE type, const ResLink &resLink) {
	/* --------------- Code_cpp block 4 start --------------- */
	/* --------------- Code_cpp block 4 end --------------- */

	operationNotify(*this, resLink, type);

	/* --------------- Code_cpp block 5 start --------------- */
	switch (type) {
	case ItemOp::WRITE:
		WPP_LOGD(TAG, "Server WRITE -> resId: %d, resInstId: %d", resLink.resId, resLink.resInstId);
		break;
	case ItemOp::EXECUTE:
		WPP_LOGD(TAG, "Server EXECUTE -> resId: %d, resInstId: %d", resLink.resId, resLink.resInstId);
		break;
	default: break;
	}
	/* --------------- Code_cpp block 5 end --------------- */
}

void GenericSensor::userOperationNotifier(ItemOp::TYPE type, const ResLink &resLink) {
	if (type == ItemOp::WRITE || type == ItemOp::DELETE) notifyResChanged(resLink.resId, resLink.resInstId);

	/* --------------- Code_cpp block 6 start --------------- */
	switch (type) {
	case ItemOp::WRITE:
		WPP_LOGD(TAG, "User WRITE -> resId: %d, resInstId: %d", resLink.resId, resLink.resInstId);
		break;
	case ItemOp::DELETE:
		WPP_LOGD(TAG, "User DELETE -> resId: %d, resInstId: %d", resLink.resId, resLink.resInstId);
		break;
	default: break;
	}
	/* --------------- Code_cpp block 6 end --------------- */
}

void GenericSensor::resourcesCreate() {
	std::vector<Resource> resources = {
		{SENSOR_VALUE_5700,                      ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::FLOAT },   
		#if RES_3300_5701                                                                                                                                                               
		{SENSOR_UNITS_5701,                      ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING },  
		#endif                                                                                                                                                                          
		#if RES_3300_5601                                                                                                                                                               
		{MIN_MEASURED_VALUE_5601,                ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::FLOAT },   
		#endif                                                                                                                                                                          
		#if RES_3300_5602                                                                                                                                                               
		{MAX_MEASURED_VALUE_5602,                ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::FLOAT },   
		#endif                                                                                                                                                                          
		#if RES_3300_5603                                                                                                                                                               
		{MIN_RANGE_VALUE_5603,                   ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::FLOAT },   
		#endif                                                                                                                                                                          
		#if RES_3300_5604                                                                                                                                                               
		{MAX_RANGE_VALUE_5604,                   ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::FLOAT },   
		#endif                                                                                                                                                                          
		#if RES_3300_5750                                                                                                                                                               
		{APPLICATION_TYPE_5750,                  ItemOp(ItemOp::READ|ItemOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING },  
		#endif                                                                                                                                                                          
		#if RES_3300_5751                                                                                                                                                               
		{SENSOR_TYPE_5751,                       ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING },  
		#endif                                                                                                                                                                          
		#if RES_3300_5605                                                                                                                                                               
		{RESET_MIN_AND_MAX_MEASURED_VALUES_5605, ItemOp(ItemOp::EXECUTE),            IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::EXECUTE }, 
		#endif                                                                                                                                                                          
		#if RES_3300_5518                                                                                                                                                               
		{TIMESTAMP_5518,                         ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::TIME },    
		#endif                                                                                                                                                                          
		#if RES_3300_6050                                                                                                                                                               
		{FRACTIONAL_TIMESTAMP_6050,              ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::FLOAT },   
		#endif                                                                                                                                                                          
		#if RES_3300_6042                                                                                                                                                               
		{MEASUREMENT_QUALITY_INDICATOR_6042,     ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },     
		#endif                                                                                                                                                                          
		#if RES_3300_6049                                                                                                                                                               
		{MEASUREMENT_QUALITY_LEVEL_6049,         ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },     
		#endif                                                                                                                                                                          
	};
	setupResources(std::move(resources));
}

void GenericSensor::resourcesInit() {
	/* --------------- Code_cpp block 7 start --------------- */
	resource(SENSOR_VALUE_5700)->set<FLOAT_T>(0.0f); // TODO: Set appropriate value
	// resource(SENSOR_VALUE_5700)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#if RES_3300_5701
	resource(SENSOR_UNITS_5701)->set<STRING_T>(""); // TODO: Set appropriate value
	// resource(SENSOR_UNITS_5701)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3300_5601
	resource(MIN_MEASURED_VALUE_5601)->set<FLOAT_T>(0.0f); // TODO: Set appropriate value
	// resource(MIN_MEASURED_VALUE_5601)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3300_5602
	resource(MAX_MEASURED_VALUE_5602)->set<FLOAT_T>(0.0f); // TODO: Set appropriate value
	// resource(MAX_MEASURED_VALUE_5602)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3300_5603
	resource(MIN_RANGE_VALUE_5603)->set<FLOAT_T>(0.0f); // TODO: Set appropriate value
	// resource(MIN_RANGE_VALUE_5603)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3300_5604
	resource(MAX_RANGE_VALUE_5604)->set<FLOAT_T>(0.0f); // TODO: Set appropriate value
	// resource(MAX_RANGE_VALUE_5604)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3300_5750
	resource(APPLICATION_TYPE_5750)->set<STRING_T>(""); // TODO: Set appropriate value
	// resource(APPLICATION_TYPE_5750)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3300_5751
	resource(SENSOR_TYPE_5751)->set<STRING_T>(""); // TODO: Set appropriate value
	// resource(SENSOR_TYPE_5751)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3300_5605
	// resource(RESET_MIN_AND_MAX_MEASURED_VALUES_5605)->set<EXECUTE_T>( /* TODO: Implement execute handler */ );
	// resource(RESET_MIN_AND_MAX_MEASURED_VALUES_5605)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3300_5518
	resource(TIMESTAMP_5518)->set<TIME_T>(0); // TODO: Set appropriate value
	// resource(TIMESTAMP_5518)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3300_6050
	resource(FRACTIONAL_TIMESTAMP_6050)->set<FLOAT_T>(0.0f); // TODO: Set appropriate value
	// resource(FRACTIONAL_TIMESTAMP_6050)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3300_6042
	resource(MEASUREMENT_QUALITY_INDICATOR_6042)->set<INT_T>(0); // TODO: Set appropriate value
	// resource(MEASUREMENT_QUALITY_INDICATOR_6042)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3300_6049
	resource(MEASUREMENT_QUALITY_LEVEL_6049)->set<INT_T>(0); // TODO: Set appropriate value
	// resource(MEASUREMENT_QUALITY_LEVEL_6049)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	/* --------------- Code_cpp block 7 end --------------- */
}

/* --------------- Code_cpp block 8 start --------------- */
/* --------------- Code_cpp block 8 end --------------- */

} /* namespace wpp */
