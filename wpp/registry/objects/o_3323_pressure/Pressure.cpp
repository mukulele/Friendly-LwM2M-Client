/*
 * Pressure
 * Generated on: 2025-11-12 12:55:44
 * Created by: Sinai RnD
 */

#include "o_3323_pressure/Pressure.h"

#include "Resource.h"
#include "ItemOp.h"
#include "WppTypes.h"
#include "WppLogs.h"
#include "WppClient.h"

/* --------------- Code_cpp block 0 start --------------- */
/* --------------- Code_cpp block 0 end --------------- */

#define TAG "Pressure"

namespace wpp {

Pressure::Pressure(lwm2m_context_t &context, const OBJ_LINK_T &id): Instance(context, id) {

	/* --------------- Code_cpp block 1 start --------------- */
	/* --------------- Code_cpp block 1 end --------------- */

	resourcesCreate();
	resourcesInit();

	/* --------------- Code_cpp block 2 start --------------- */
	/* --------------- Code_cpp block 2 end --------------- */
}

Pressure::~Pressure() {
	/* --------------- Code_cpp block 3 start --------------- */
	/* --------------- Code_cpp block 3 end --------------- */
}

Object & Pressure::object(WppClient &ctx) {
	return ctx.registry().pressure();
}

Pressure * Pressure::instance(WppClient &ctx, ID_T instId) {
	Instance *inst = ctx.registry().pressure().instance(instId);
	if (!inst) return NULL;
	return static_cast<Pressure*>(inst);
}

Pressure * Pressure::createInst(WppClient &ctx, ID_T instId) {
	Instance *inst = ctx.registry().pressure().createInstance(instId);
	if (!inst) return NULL;
	return static_cast<Pressure*>(inst);
}

bool Pressure::removeInst(WppClient &ctx, ID_T instId) {
	return ctx.registry().pressure().remove(instId);
}

void Pressure::serverOperationNotifier(Instance *securityInst, ItemOp::TYPE type, const ResLink &resLink) {
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

void Pressure::userOperationNotifier(ItemOp::TYPE type, const ResLink &resLink) {
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

void Pressure::resourcesCreate() {
	std::vector<Resource> resources = {
		{SENSOR_VALUE_5700,                      ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::FLOAT },   
		#if RES_3323_5701                                                                                                                                                               
		{SENSOR_UNITS_5701,                      ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING },  
		#endif                                                                                                                                                                          
		#if RES_3323_5601                                                                                                                                                               
		{MIN_MEASURED_VALUE_5601,                ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::FLOAT },   
		#endif                                                                                                                                                                          
		#if RES_3323_5602                                                                                                                                                               
		{MAX_MEASURED_VALUE_5602,                ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::FLOAT },   
		#endif                                                                                                                                                                          
		#if RES_3323_5603                                                                                                                                                               
		{MIN_RANGE_VALUE_5603,                   ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::FLOAT },   
		#endif                                                                                                                                                                          
		#if RES_3323_5604                                                                                                                                                               
		{MAX_RANGE_VALUE_5604,                   ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::FLOAT },   
		#endif                                                                                                                                                                          
		#if RES_3323_5605                                                                                                                                                               
		{RESET_MIN_AND_MAX_MEASURED_VALUES_5605, ItemOp(ItemOp::EXECUTE),            IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::EXECUTE }, 
		#endif                                                                                                                                                                          
		#if RES_3323_5821                                                                                                                                                               
		{CURRENT_CALIBRATION_5821,               ItemOp(ItemOp::READ|ItemOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::FLOAT },   
		#endif                                                                                                                                                                          
		#if RES_3323_5750                                                                                                                                                               
		{APPLICATION_TYPE_5750,                  ItemOp(ItemOp::READ|ItemOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING },  
		#endif                                                                                                                                                                          
		#if RES_3323_5518                                                                                                                                                               
		{TIMESTAMP_5518,                         ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::TIME },    
		#endif                                                                                                                                                                          
		#if RES_3323_6050                                                                                                                                                               
		{FRACTIONAL_TIMESTAMP_6050,              ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::FLOAT },   
		#endif                                                                                                                                                                          
		#if RES_3323_6042                                                                                                                                                               
		{MEASUREMENT_QUALITY_INDICATOR_6042,     ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },     
		#endif                                                                                                                                                                          
		#if RES_3323_6049                                                                                                                                                               
		{MEASUREMENT_QUALITY_LEVEL_6049,         ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },     
		#endif                                                                                                                                                                          
	};
	setupResources(std::move(resources));
}

void Pressure::resourcesInit() {
	/* --------------- Code_cpp block 7 start --------------- */
	resource(SENSOR_VALUE_5700)->set<FLOAT_T>(0.0f); // TODO: Set appropriate value
	// resource(SENSOR_VALUE_5700)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#if RES_3323_5701
	resource(SENSOR_UNITS_5701)->set<STRING_T>(""); // TODO: Set appropriate value
	// resource(SENSOR_UNITS_5701)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3323_5601
	resource(MIN_MEASURED_VALUE_5601)->set<FLOAT_T>(0.0f); // TODO: Set appropriate value
	// resource(MIN_MEASURED_VALUE_5601)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3323_5602
	resource(MAX_MEASURED_VALUE_5602)->set<FLOAT_T>(0.0f); // TODO: Set appropriate value
	// resource(MAX_MEASURED_VALUE_5602)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3323_5603
	resource(MIN_RANGE_VALUE_5603)->set<FLOAT_T>(0.0f); // TODO: Set appropriate value
	// resource(MIN_RANGE_VALUE_5603)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3323_5604
	resource(MAX_RANGE_VALUE_5604)->set<FLOAT_T>(0.0f); // TODO: Set appropriate value
	// resource(MAX_RANGE_VALUE_5604)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3323_5605
	// resource(RESET_MIN_AND_MAX_MEASURED_VALUES_5605)->set<EXECUTE_T>( /* TODO: Implement execute handler */ );
	// resource(RESET_MIN_AND_MAX_MEASURED_VALUES_5605)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3323_5821
	resource(CURRENT_CALIBRATION_5821)->set<FLOAT_T>(0.0f); // TODO: Set appropriate value
	// resource(CURRENT_CALIBRATION_5821)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3323_5750
	resource(APPLICATION_TYPE_5750)->set<STRING_T>(""); // TODO: Set appropriate value
	// resource(APPLICATION_TYPE_5750)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3323_5518
	resource(TIMESTAMP_5518)->set<TIME_T>(0); // TODO: Set appropriate value
	// resource(TIMESTAMP_5518)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3323_6050
	resource(FRACTIONAL_TIMESTAMP_6050)->set<FLOAT_T>(0.0f); // TODO: Set appropriate value
	// resource(FRACTIONAL_TIMESTAMP_6050)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3323_6042
	resource(MEASUREMENT_QUALITY_INDICATOR_6042)->set<INT_T>(0); // TODO: Set appropriate value
	// resource(MEASUREMENT_QUALITY_INDICATOR_6042)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3323_6049
	resource(MEASUREMENT_QUALITY_LEVEL_6049)->set<INT_T>(0); // TODO: Set appropriate value
	// resource(MEASUREMENT_QUALITY_LEVEL_6049)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	/* --------------- Code_cpp block 7 end --------------- */
}

/* --------------- Code_cpp block 8 start --------------- */
/* --------------- Code_cpp block 8 end --------------- */

} /* namespace wpp */
