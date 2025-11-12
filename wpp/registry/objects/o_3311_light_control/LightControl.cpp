/*
 * LightControl
 * Generated on: 2025-11-12 12:46:13
 * Created by: Sinai RnD
 */

#include "o_3311_light_control/LightControl.h"

#include "Resource.h"
#include "ItemOp.h"
#include "WppTypes.h"
#include "WppLogs.h"
#include "WppClient.h"

/* --------------- Code_cpp block 0 start --------------- */
/* --------------- Code_cpp block 0 end --------------- */

#define TAG "LightControl"

namespace wpp {

LightControl::LightControl(lwm2m_context_t &context, const OBJ_LINK_T &id): Instance(context, id) {

	/* --------------- Code_cpp block 1 start --------------- */
	/* --------------- Code_cpp block 1 end --------------- */

	resourcesCreate();
	resourcesInit();

	/* --------------- Code_cpp block 2 start --------------- */
	/* --------------- Code_cpp block 2 end --------------- */
}

LightControl::~LightControl() {
	/* --------------- Code_cpp block 3 start --------------- */
	/* --------------- Code_cpp block 3 end --------------- */
}

Object & LightControl::object(WppClient &ctx) {
	return ctx.registry().lightControl();
}

LightControl * LightControl::instance(WppClient &ctx, ID_T instId) {
	Instance *inst = ctx.registry().lightControl().instance(instId);
	if (!inst) return NULL;
	return static_cast<LightControl*>(inst);
}

LightControl * LightControl::createInst(WppClient &ctx, ID_T instId) {
	Instance *inst = ctx.registry().lightControl().createInstance(instId);
	if (!inst) return NULL;
	return static_cast<LightControl*>(inst);
}

bool LightControl::removeInst(WppClient &ctx, ID_T instId) {
	return ctx.registry().lightControl().remove(instId);
}

void LightControl::serverOperationNotifier(Instance *securityInst, ItemOp::TYPE type, const ResLink &resLink) {
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

void LightControl::userOperationNotifier(ItemOp::TYPE type, const ResLink &resLink) {
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

void LightControl::resourcesCreate() {
	std::vector<Resource> resources = {
		{ON_OFF_5850,                  ItemOp(ItemOp::READ|ItemOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::BOOL },   
		#if RES_3311_5851                                                                                                                                                    
		{DIMMER_5851,                  ItemOp(ItemOp::READ|ItemOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },    
		#endif                                                                                                                                                               
		#if RES_3311_5852                                                                                                                                                    
		{ON_TIME_5852,                 ItemOp(ItemOp::READ|ItemOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },    
		#endif                                                                                                                                                               
		#if RES_3311_5805                                                                                                                                                    
		{CUMULATIVE_ACTIVE_POWER_5805, ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::FLOAT },  
		#endif                                                                                                                                                               
		#if RES_3311_5820                                                                                                                                                    
		{POWER_FACTOR_5820,            ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::FLOAT },  
		#endif                                                                                                                                                               
		#if RES_3311_5706                                                                                                                                                    
		{COLOUR_5706,                  ItemOp(ItemOp::READ|ItemOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING }, 
		#endif                                                                                                                                                               
		#if RES_3311_5701                                                                                                                                                    
		{SENSOR_UNITS_5701,            ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING }, 
		#endif                                                                                                                                                               
		#if RES_3311_5750                                                                                                                                                    
		{APPLICATION_TYPE_5750,        ItemOp(ItemOp::READ|ItemOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING }, 
		#endif                                                                                                                                                               
	};
	setupResources(std::move(resources));
}

void LightControl::resourcesInit() {
	/* --------------- Code_cpp block 7 start --------------- */
	resource(ON_OFF_5850)->set<BOOL_T>(false); // TODO: Set appropriate value
	// resource(ON_OFF_5850)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#if RES_3311_5851
	resource(DIMMER_5851)->set<INT_T>(0); // TODO: Set appropriate value
	// resource(DIMMER_5851)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3311_5852
	resource(ON_TIME_5852)->set<INT_T>(0); // TODO: Set appropriate value
	// resource(ON_TIME_5852)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3311_5805
	resource(CUMULATIVE_ACTIVE_POWER_5805)->set<FLOAT_T>(0.0f); // TODO: Set appropriate value
	// resource(CUMULATIVE_ACTIVE_POWER_5805)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3311_5820
	resource(POWER_FACTOR_5820)->set<FLOAT_T>(0.0f); // TODO: Set appropriate value
	// resource(POWER_FACTOR_5820)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3311_5706
	resource(COLOUR_5706)->set<STRING_T>(""); // TODO: Set appropriate value
	// resource(COLOUR_5706)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3311_5701
	resource(SENSOR_UNITS_5701)->set<STRING_T>(""); // TODO: Set appropriate value
	// resource(SENSOR_UNITS_5701)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3311_5750
	resource(APPLICATION_TYPE_5750)->set<STRING_T>(""); // TODO: Set appropriate value
	// resource(APPLICATION_TYPE_5750)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	/* --------------- Code_cpp block 7 end --------------- */
}

/* --------------- Code_cpp block 8 start --------------- */
/* --------------- Code_cpp block 8 end --------------- */

} /* namespace wpp */
