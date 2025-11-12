/*
 * Ipso3336
 * Generated on: 2025-11-12 12:46:20
 * Created by: Sinai RnD
 */

#include "o_3336_ipso3336/Ipso3336.h"

#include "Resource.h"
#include "ItemOp.h"
#include "WppTypes.h"
#include "WppLogs.h"
#include "WppClient.h"

/* --------------- Code_cpp block 0 start --------------- */
/* --------------- Code_cpp block 0 end --------------- */

#define TAG "Ipso3336"

namespace wpp {

Ipso3336::Ipso3336(lwm2m_context_t &context, const OBJ_LINK_T &id): Instance(context, id) {

	/* --------------- Code_cpp block 1 start --------------- */
	/* --------------- Code_cpp block 1 end --------------- */

	resourcesCreate();
	resourcesInit();

	/* --------------- Code_cpp block 2 start --------------- */
	/* --------------- Code_cpp block 2 end --------------- */
}

Ipso3336::~Ipso3336() {
	/* --------------- Code_cpp block 3 start --------------- */
	/* --------------- Code_cpp block 3 end --------------- */
}

Object & Ipso3336::object(WppClient &ctx) {
	return ctx.registry().ipso3336();
}

Ipso3336 * Ipso3336::instance(WppClient &ctx, ID_T instId) {
	Instance *inst = ctx.registry().ipso3336().instance(instId);
	if (!inst) return NULL;
	return static_cast<Ipso3336*>(inst);
}

Ipso3336 * Ipso3336::createInst(WppClient &ctx, ID_T instId) {
	Instance *inst = ctx.registry().ipso3336().createInstance(instId);
	if (!inst) return NULL;
	return static_cast<Ipso3336*>(inst);
}

bool Ipso3336::removeInst(WppClient &ctx, ID_T instId) {
	return ctx.registry().ipso3336().remove(instId);
}

void Ipso3336::serverOperationNotifier(Instance *securityInst, ItemOp::TYPE type, const ResLink &resLink) {
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

void Ipso3336::userOperationNotifier(ItemOp::TYPE type, const ResLink &resLink) {
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

void Ipso3336::resourcesCreate() {
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

void Ipso3336::resourcesInit() {
	/* --------------- Code_cpp block 7 start --------------- */
	resource(NUMERIC_LATITUDE_6051)->set<FLOAT_T>(0.0f); // TODO: Set appropriate value
	// resource(NUMERIC_LATITUDE_6051)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	resource(NUMERIC_LONGITUDE_6052)->set<FLOAT_T>(0.0f); // TODO: Set appropriate value
	// resource(NUMERIC_LONGITUDE_6052)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#if RES_3336_6053
	resource(NUMERIC_UNCERTAINTY_6053)->set<FLOAT_T>(0.0f); // TODO: Set appropriate value
	// resource(NUMERIC_UNCERTAINTY_6053)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3336_5705
	resource(COMPASS_DIRECTION_5705)->set<FLOAT_T>(0.0f); // TODO: Set appropriate value
	// resource(COMPASS_DIRECTION_5705)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3336_5517
	resource(VELOCITY_5517)->set<OPAQUE_T>(0); // TODO: Set appropriate value
	// resource(VELOCITY_5517)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3336_5518
	resource(TIMESTAMP_5518)->set<TIME_T>(0); // TODO: Set appropriate value
	// resource(TIMESTAMP_5518)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3336_5750
	resource(APPLICATION_TYPE_5750)->set<STRING_T>(""); // TODO: Set appropriate value
	// resource(APPLICATION_TYPE_5750)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3336_6050
	resource(FRACTIONAL_TIMESTAMP_6050)->set<FLOAT_T>(0.0f); // TODO: Set appropriate value
	// resource(FRACTIONAL_TIMESTAMP_6050)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3336_6042
	resource(MEASUREMENT_QUALITY_INDICATOR_6042)->set<INT_T>(0); // TODO: Set appropriate value
	// resource(MEASUREMENT_QUALITY_INDICATOR_6042)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3336_6049
	resource(MEASUREMENT_QUALITY_LEVEL_6049)->set<INT_T>(0); // TODO: Set appropriate value
	// resource(MEASUREMENT_QUALITY_LEVEL_6049)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	/* --------------- Code_cpp block 7 end --------------- */
}

/* --------------- Code_cpp block 8 start --------------- */
/* --------------- Code_cpp block 8 end --------------- */

} /* namespace wpp */
