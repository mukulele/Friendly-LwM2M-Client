/*
 * Location
 * Generated on: 2025-11-12 12:55:51
 * Created by: Sinai RnD
 */

#include "o_6_location/Location.h"

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
		{LATITUDE_0,  ItemOp(ItemOp::READ), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::FLOAT },  
		{LONGITUDE_1, ItemOp(ItemOp::READ), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::FLOAT },  
		#if RES_6_2                                                                                                                           
		{ALTITUDE_2,  ItemOp(ItemOp::READ), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::FLOAT },  
		#endif                                                                                                                                
		#if RES_6_3                                                                                                                           
		{RADIUS_3,    ItemOp(ItemOp::READ), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::FLOAT },  
		#endif                                                                                                                                
		#if RES_6_4                                                                                                                           
		{VELOCITY_4,  ItemOp(ItemOp::READ), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::OPAQUE }, 
		#endif                                                                                                                                
		{TIMESTAMP_5, ItemOp(ItemOp::READ), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::TIME },   
		#if RES_6_6                                                                                                                           
		{SPEED_6,     ItemOp(ItemOp::READ), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::FLOAT },  
		#endif                                                                                                                                
	};
	setupResources(std::move(resources));
}

void Location::resourcesInit() {
	/* --------------- Code_cpp block 7 start --------------- */
	resource(LATITUDE_0)->set<FLOAT_T>(0.0f); // TODO: Set appropriate value
	// resource(LATITUDE_0)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	resource(LONGITUDE_1)->set<FLOAT_T>(0.0f); // TODO: Set appropriate value
	// resource(LONGITUDE_1)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#if RES_6_2
	resource(ALTITUDE_2)->set<FLOAT_T>(0.0f); // TODO: Set appropriate value
	// resource(ALTITUDE_2)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_6_3
	resource(RADIUS_3)->set<FLOAT_T>(0.0f); // TODO: Set appropriate value
	// resource(RADIUS_3)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_6_4
	resource(VELOCITY_4)->set<OPAQUE_T>(0); // TODO: Set appropriate value
	// resource(VELOCITY_4)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	resource(TIMESTAMP_5)->set<TIME_T>(0); // TODO: Set appropriate value
	// resource(TIMESTAMP_5)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#if RES_6_6
	resource(SPEED_6)->set<FLOAT_T>(0.0f); // TODO: Set appropriate value
	// resource(SPEED_6)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	/* --------------- Code_cpp block 7 end --------------- */
}

/* --------------- Code_cpp block 8 start --------------- */
/* --------------- Code_cpp block 8 end --------------- */

} /* namespace wpp */
