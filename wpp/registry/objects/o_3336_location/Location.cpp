/*
 * Location
 * Generated on: 2025-11-21 15:31:51
 * Created by: Sinai RnD
 */

#include "o_3336_location/Location.h"

#include "Resource.h"
#include "ItemOp.h"
#include "WppTypes.h"
#include "WppLogs.h"
#include "WppClient.h"

/* --------------- Code_cpp block 0 start --------------- */
/* --------------- Code_cpp block 0 end --------------- */

#define TAG "Location"

namespace wpp {

Location::Location(lwm2m_context_t &context, const OBJ_LINK_T &id): Instance(context, id) {

	/* --------------- Code_cpp block 1 start --------------- */
	/* --------------- Code_cpp block 1 end --------------- */

	resourcesCreate();
	resourcesInit();

	/* --------------- Code_cpp block 2 start --------------- */
	/* --------------- Code_cpp block 2 end --------------- */
}

Location::~Location() {
	/* --------------- Code_cpp block 3 start --------------- */
	/* --------------- Code_cpp block 3 end --------------- */
}

Object & Location::object(WppClient &ctx) {
	return ctx.registry().location();
}

Location * Location::instance(WppClient &ctx, ID_T instId) {
	Instance *inst = ctx.registry().location().instance(instId);
	if (!inst) return NULL;
	return static_cast<Location*>(inst);
}

Location * Location::createInst(WppClient &ctx, ID_T instId) {
	Instance *inst = ctx.registry().location().createInstance(instId);
	if (!inst) return NULL;
	return static_cast<Location*>(inst);
}

bool Location::removeInst(WppClient &ctx, ID_T instId) {
	return ctx.registry().location().remove(instId);
}

void Location::serverOperationNotifier(Instance *securityInst, ItemOp::TYPE type, const ResLink &resLink) {
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

void Location::userOperationNotifier(ItemOp::TYPE type, const ResLink &resLink) {
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

void Location::resourcesCreate() {
	std::vector<Resource> resources = {
		{NUMERIC_LATITUDE_6051,              ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::FLOAT },  
		{NUMERIC_LONGITUDE_6052,             ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::FLOAT },  
		#if RES_3336_6053                                                                                                                                                          
		{NUMERIC_UNCERTAINTY_6053,           ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::FLOAT },  
		#endif                                                                                                                                                                     
		#if RES_3336_5705                                                                                                                                                          
		{COMPASS_DIRECTION_5705,             ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::FLOAT },  
		#endif                                                                                                                                                                     
		#if RES_3336_5517                                                                                                                                                          
		{VELOCITY_5517,                      ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::OPAQUE }, 
		#endif                                                                                                                                                                     
		#if RES_3336_5518                                                                                                                                                          
		{TIMESTAMP_5518,                     ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::TIME },   
		#endif                                                                                                                                                                     
		#if RES_3336_5750                                                                                                                                                          
		{APPLICATION_TYPE_5750,              ItemOp(ItemOp::READ|ItemOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING }, 
		#endif                                                                                                                                                                     
		#if RES_3336_6050                                                                                                                                                          
		{FRACTIONAL_TIMESTAMP_6050,          ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::FLOAT },  
		#endif                                                                                                                                                                     
		#if RES_3336_6042                                                                                                                                                          
		{MEASUREMENT_QUALITY_INDICATOR_6042, ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },    
		#endif                                                                                                                                                                     
		#if RES_3336_6049                                                                                                                                                          
		{MEASUREMENT_QUALITY_LEVEL_6049,     ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },    
		#endif                                                                                                                                                                     
	};
	setupResources(std::move(resources));
}

void Location::resourcesInit() {
	/* --------------- Code_cpp block 7 start --------------- */
	resource(NUMERIC_LATITUDE_6051)->set<FLOAT_T>( /* TODO */ );
	resource(NUMERIC_LATITUDE_6051)->setDataVerifier( /* TODO */ );
	resource(NUMERIC_LONGITUDE_6052)->set<FLOAT_T>( /* TODO */ );
	resource(NUMERIC_LONGITUDE_6052)->setDataVerifier( /* TODO */ );
	#if RES_3336_6053
	resource(NUMERIC_UNCERTAINTY_6053)->set<FLOAT_T>( /* TODO */ );
	resource(NUMERIC_UNCERTAINTY_6053)->setDataVerifier( /* TODO */ );
	#endif
	#if RES_3336_5705
	resource(COMPASS_DIRECTION_5705)->set<FLOAT_T>( /* TODO */ );
	resource(COMPASS_DIRECTION_5705)->setDataVerifier( /* TODO */ );
	#endif
	#if RES_3336_5517
	resource(VELOCITY_5517)->set<OPAQUE_T>( /* TODO */ );
	resource(VELOCITY_5517)->setDataVerifier( /* TODO */ );
	#endif
	#if RES_3336_5518
	resource(TIMESTAMP_5518)->set<TIME_T>( /* TODO */ );
	resource(TIMESTAMP_5518)->setDataVerifier( /* TODO */ );
	#endif
	#if RES_3336_5750
	resource(APPLICATION_TYPE_5750)->set<STRING_T>( /* TODO */ );
	resource(APPLICATION_TYPE_5750)->setDataVerifier( /* TODO */ );
	#endif
	#if RES_3336_6050
	resource(FRACTIONAL_TIMESTAMP_6050)->set<FLOAT_T>( /* TODO */ );
	resource(FRACTIONAL_TIMESTAMP_6050)->setDataVerifier( /* TODO */ );
	#endif
	#if RES_3336_6042
	resource(MEASUREMENT_QUALITY_INDICATOR_6042)->set<INT_T>( /* TODO */ );
	resource(MEASUREMENT_QUALITY_INDICATOR_6042)->setDataVerifier( /* TODO */ );
	#endif
	#if RES_3336_6049
	resource(MEASUREMENT_QUALITY_LEVEL_6049)->set<INT_T>( /* TODO */ );
	resource(MEASUREMENT_QUALITY_LEVEL_6049)->setDataVerifier( /* TODO */ );
	#endif
	/* --------------- Code_cpp block 7 end --------------- */
}

/* --------------- Code_cpp block 8 start --------------- */
/* --------------- Code_cpp block 8 end --------------- */

} /* namespace wpp */
