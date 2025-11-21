/*
 * AnalogOutput
 * Generated on: 2025-11-21 15:22:07
 * Created by: Sinai RnD
 */

#include "o_3203_analog_output/AnalogOutput.h"

#include "Resource.h"
#include "ItemOp.h"
#include "WppTypes.h"
#include "WppLogs.h"
#include "WppClient.h"

/* --------------- Code_cpp block 0 start --------------- */
/* --------------- Code_cpp block 0 end --------------- */

#define TAG "AnalogOutput"

namespace wpp {

AnalogOutput::AnalogOutput(lwm2m_context_t &context, const OBJ_LINK_T &id): Instance(context, id) {

	/* --------------- Code_cpp block 1 start --------------- */
	/* --------------- Code_cpp block 1 end --------------- */

	resourcesCreate();
	resourcesInit();

	/* --------------- Code_cpp block 2 start --------------- */
	/* --------------- Code_cpp block 2 end --------------- */
}

AnalogOutput::~AnalogOutput() {
	/* --------------- Code_cpp block 3 start --------------- */
	/* --------------- Code_cpp block 3 end --------------- */
}

Object & AnalogOutput::object(WppClient &ctx) {
	return ctx.registry().analogOutput();
}

AnalogOutput * AnalogOutput::instance(WppClient &ctx, ID_T instId) {
	Instance *inst = ctx.registry().analogOutput().instance(instId);
	if (!inst) return NULL;
	return static_cast<AnalogOutput*>(inst);
}

AnalogOutput * AnalogOutput::createInst(WppClient &ctx, ID_T instId) {
	Instance *inst = ctx.registry().analogOutput().createInstance(instId);
	if (!inst) return NULL;
	return static_cast<AnalogOutput*>(inst);
}

bool AnalogOutput::removeInst(WppClient &ctx, ID_T instId) {
	return ctx.registry().analogOutput().remove(instId);
}

void AnalogOutput::serverOperationNotifier(Instance *securityInst, ItemOp::TYPE type, const ResLink &resLink) {
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

void AnalogOutput::userOperationNotifier(ItemOp::TYPE type, const ResLink &resLink) {
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

void AnalogOutput::resourcesCreate() {
	std::vector<Resource> resources = {
		{ANALOG_OUTPUT_CURRENT_VALUE_5650, ItemOp(ItemOp::READ|ItemOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::MANDATORY, TYPE_ID::FLOAT },  
		#if RES_3203_5750                                                                                                                                                        
		{APPLICATION_TYPE_5750,            ItemOp(ItemOp::READ|ItemOp::WRITE), IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING }, 
		#endif                                                                                                                                                                   
		#if RES_3203_5603                                                                                                                                                        
		{MIN_RANGE_VALUE_5603,             ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::FLOAT },  
		#endif                                                                                                                                                                   
		#if RES_3203_5604                                                                                                                                                        
		{MAX_RANGE_VALUE_5604,             ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::FLOAT },  
		#endif                                                                                                                                                                   
		#if RES_3203_5518                                                                                                                                                        
		{TIMESTAMP_5518,                   ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::TIME },   
		#endif                                                                                                                                                                   
		#if RES_3203_6050                                                                                                                                                        
		{FRACTIONAL_TIMESTAMP_6050,        ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::FLOAT },  
		#endif                                                                                                                                                                   
	};
	setupResources(std::move(resources));
}

void AnalogOutput::resourcesInit() {
	/* --------------- Code_cpp block 7 start --------------- */
	resource(ANALOG_OUTPUT_CURRENT_VALUE_5650)->set<FLOAT_T>( /* TODO */ );
	resource(ANALOG_OUTPUT_CURRENT_VALUE_5650)->setDataVerifier( /* TODO */ );
	#if RES_3203_5750
	resource(APPLICATION_TYPE_5750)->set<STRING_T>( /* TODO */ );
	resource(APPLICATION_TYPE_5750)->setDataVerifier( /* TODO */ );
	#endif
	#if RES_3203_5603
	resource(MIN_RANGE_VALUE_5603)->set<FLOAT_T>( /* TODO */ );
	resource(MIN_RANGE_VALUE_5603)->setDataVerifier( /* TODO */ );
	#endif
	#if RES_3203_5604
	resource(MAX_RANGE_VALUE_5604)->set<FLOAT_T>( /* TODO */ );
	resource(MAX_RANGE_VALUE_5604)->setDataVerifier( /* TODO */ );
	#endif
	#if RES_3203_5518
	resource(TIMESTAMP_5518)->set<TIME_T>( /* TODO */ );
	resource(TIMESTAMP_5518)->setDataVerifier( /* TODO */ );
	#endif
	#if RES_3203_6050
	resource(FRACTIONAL_TIMESTAMP_6050)->set<FLOAT_T>( /* TODO */ );
	resource(FRACTIONAL_TIMESTAMP_6050)->setDataVerifier( /* TODO */ );
	#endif
	/* --------------- Code_cpp block 7 end --------------- */
}

/* --------------- Code_cpp block 8 start --------------- */
/* --------------- Code_cpp block 8 end --------------- */

} /* namespace wpp */
