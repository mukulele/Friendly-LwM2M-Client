/*
 * OnoffSwitch
 * Generated on: 2025-11-12 10:31:02
 * Created by: Sinai RnD
 */

#include "o_3342_onoff_switch/OnoffSwitch.h"

#include "Resource.h"
#include "ItemOp.h"
#include "WppTypes.h"
#include "WppLogs.h"
#include "WppClient.h"

/* --------------- Code_cpp block 0 start --------------- */
/* --------------- Code_cpp block 0 end --------------- */

#define TAG "OnoffSwitch"

namespace wpp {

OnoffSwitch::OnoffSwitch(lwm2m_context_t &context, const OBJ_LINK_T &id): Instance(context, id) {

	/* --------------- Code_cpp block 1 start --------------- */
	/* --------------- Code_cpp block 1 end --------------- */

	resourcesCreate();
	resourcesInit();

	/* --------------- Code_cpp block 2 start --------------- */
	/* --------------- Code_cpp block 2 end --------------- */
}

OnoffSwitch::~OnoffSwitch() {
	/* --------------- Code_cpp block 3 start --------------- */
	/* --------------- Code_cpp block 3 end --------------- */
}

Object & OnoffSwitch::object(WppClient &ctx) {
	return ctx.registry().onoffSwitch();
}

OnoffSwitch * OnoffSwitch::instance(WppClient &ctx, ID_T instId) {
	Instance *inst = ctx.registry().onoffSwitch().instance(instId);
	if (!inst) return NULL;
	return static_cast<OnoffSwitch*>(inst);
}

OnoffSwitch * OnoffSwitch::createInst(WppClient &ctx, ID_T instId) {
	Instance *inst = ctx.registry().onoffSwitch().createInstance(instId);
	if (!inst) return NULL;
	return static_cast<OnoffSwitch*>(inst);
}

bool OnoffSwitch::removeInst(WppClient &ctx, ID_T instId) {
	return ctx.registry().onoffSwitch().remove(instId);
}

void OnoffSwitch::serverOperationNotifier(Instance *securityInst, ItemOp::TYPE type, const ResLink &resLink) {
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

void OnoffSwitch::userOperationNotifier(ItemOp::TYPE type, const ResLink &resLink) {
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

void OnoffSwitch::resourcesCreate() {
	std::vector<Resource> resources = {
		{DIGITAL_INPUT_STATE_5500,   ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::BOOL },   
		#if RES_3342_5501                                                                                                                                                  
		{DIGITAL_INPUT_COUNTER_5501, ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },    
		#endif                                                                                                                                                             
		#if RES_3342_5852                                                                                                                                                  
		{ON_TIME_5852,               ItemOp(ItemOp::READ|ItemOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },    
		#endif                                                                                                                                                             
		#if RES_3342_5854                                                                                                                                                  
		{OFF_TIME_5854,              ItemOp(ItemOp::READ|ItemOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },    
		#endif                                                                                                                                                             
		#if RES_3342_5750                                                                                                                                                  
		{APPLICATION_TYPE_5750,      ItemOp(ItemOp::READ|ItemOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING }, 
		#endif                                                                                                                                                             
		#if RES_3342_5518                                                                                                                                                  
		{TIMESTAMP_5518,             ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::TIME },   
		#endif                                                                                                                                                             
		#if RES_3342_6050                                                                                                                                                  
		{FRACTIONAL_TIMESTAMP_6050,  ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::FLOAT },  
		#endif                                                                                                                                                             
	};
	setupResources(std::move(resources));
}

void OnoffSwitch::resourcesInit() {
	/* --------------- Code_cpp block 7 start --------------- */
	resource(DIGITAL_INPUT_STATE_5500)->set<BOOL_T>( /* TODO */ );
	resource(DIGITAL_INPUT_STATE_5500)->setDataVerifier( /* TODO */ );
	#if RES_3342_5501
	resource(DIGITAL_INPUT_COUNTER_5501)->set<INT_T>( /* TODO */ );
	resource(DIGITAL_INPUT_COUNTER_5501)->setDataVerifier( /* TODO */ );
	#endif
	#if RES_3342_5852
	resource(ON_TIME_5852)->set<INT_T>( /* TODO */ );
	resource(ON_TIME_5852)->setDataVerifier( /* TODO */ );
	#endif
	#if RES_3342_5854
	resource(OFF_TIME_5854)->set<INT_T>( /* TODO */ );
	resource(OFF_TIME_5854)->setDataVerifier( /* TODO */ );
	#endif
	#if RES_3342_5750
	resource(APPLICATION_TYPE_5750)->set<STRING_T>( /* TODO */ );
	resource(APPLICATION_TYPE_5750)->setDataVerifier( /* TODO */ );
	#endif
	#if RES_3342_5518
	resource(TIMESTAMP_5518)->set<TIME_T>( /* TODO */ );
	resource(TIMESTAMP_5518)->setDataVerifier( /* TODO */ );
	#endif
	#if RES_3342_6050
	resource(FRACTIONAL_TIMESTAMP_6050)->set<FLOAT_T>( /* TODO */ );
	resource(FRACTIONAL_TIMESTAMP_6050)->setDataVerifier( /* TODO */ );
	#endif
	/* --------------- Code_cpp block 7 end --------------- */
}

/* --------------- Code_cpp block 8 start --------------- */
/* --------------- Code_cpp block 8 end --------------- */

} /* namespace wpp */
