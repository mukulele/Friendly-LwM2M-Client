/*
 * Device
 * Generated on: 2025-11-12 12:56:25
 * Created by: Sinai RnD
 */

#include "m_3_device/Device.h"

#include "Resource.h"
#include "ItemOp.h"
#include "WppTypes.h"
#include "WppLogs.h"
#include "WppClient.h"

/* --------------- Code_cpp block 0 start --------------- */
/* --------------- Code_cpp block 0 end --------------- */

#define TAG "Device"

namespace wpp {

Device::Device(lwm2m_context_t &context, const OBJ_LINK_T &id): Instance(context, id) {

	/* --------------- Code_cpp block 1 start --------------- */
	/* --------------- Code_cpp block 1 end --------------- */

	resourcesCreate();
	resourcesInit();

	/* --------------- Code_cpp block 2 start --------------- */
	/* --------------- Code_cpp block 2 end --------------- */
}

Device::~Device() {
	/* --------------- Code_cpp block 3 start --------------- */
	/* --------------- Code_cpp block 3 end --------------- */
}

Object & Device::object(WppClient &ctx) {
	return ctx.registry().device();
}

Device * Device::instance(WppClient &ctx, ID_T instId) {
	Instance *inst = ctx.registry().device().instance(instId);
	if (!inst) return NULL;
	return static_cast<Device*>(inst);
}

Device * Device::createInst(WppClient &ctx, ID_T instId) {
	Instance *inst = ctx.registry().device().createInstance(instId);
	if (!inst) return NULL;
	return static_cast<Device*>(inst);
}

bool Device::removeInst(WppClient &ctx, ID_T instId) {
	return ctx.registry().device().remove(instId);
}

void Device::serverOperationNotifier(Instance *securityInst, ItemOp::TYPE type, const ResLink &resLink) {
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

void Device::userOperationNotifier(ItemOp::TYPE type, const ResLink &resLink) {
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

void Device::resourcesCreate() {
	std::vector<Resource> resources = {
		#if RES_3_0                                                                                                                                                                
		{MANUFACTURER_0,                 ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING },   
		#endif                                                                                                                                                                     
		#if RES_3_1                                                                                                                                                                
		{MODEL_NUMBER_1,                 ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING },   
		#endif                                                                                                                                                                     
		#if RES_3_2                                                                                                                                                                
		{SERIAL_NUMBER_2,                ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING },   
		#endif                                                                                                                                                                     
		#if RES_3_3                                                                                                                                                                
		{FIRMWARE_VERSION_3,             ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING },   
		#endif                                                                                                                                                                     
		{REBOOT_4,                       ItemOp(ItemOp::EXECUTE),            IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::EXECUTE },  
		#if RES_3_5                                                                                                                                                                
		{FACTORY_RESET_5,                ItemOp(ItemOp::EXECUTE),            IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::EXECUTE },  
		#endif                                                                                                                                                                     
		#if RES_3_6                                                                                                                                                                
		{AVAILABLE_POWER_SOURCES_6,      ItemOp(ItemOp::READ),               IS_SINGLE::MULTIPLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },      
		#endif                                                                                                                                                                     
		#if RES_3_7                                                                                                                                                                
		{POWER_SOURCE_VOLTAGE_7,         ItemOp(ItemOp::READ),               IS_SINGLE::MULTIPLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },      
		#endif                                                                                                                                                                     
		#if RES_3_8                                                                                                                                                                
		{POWER_SOURCE_CURRENT_8,         ItemOp(ItemOp::READ),               IS_SINGLE::MULTIPLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },      
		#endif                                                                                                                                                                     
		#if RES_3_9                                                                                                                                                                
		{BATTERY_LEVEL_9,                ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },      
		#endif                                                                                                                                                                     
		#if RES_3_10                                                                                                                                                               
		{MEMORY_FREE_10,                 ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },      
		#endif                                                                                                                                                                     
		{ERROR_CODE_11,                  ItemOp(ItemOp::READ),               IS_SINGLE::MULTIPLE, IS_MANDATORY::MANDATORY, TYPE_ID::INT },      
		#if RES_3_12                                                                                                                                                               
		{RESET_ERROR_CODE_12,            ItemOp(ItemOp::EXECUTE),            IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::EXECUTE },  
		#endif                                                                                                                                                                     
		#if RES_3_13                                                                                                                                                               
		{CURRENT_TIME_13,                ItemOp(ItemOp::READ|ItemOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::TIME },     
		#endif                                                                                                                                                                     
		#if RES_3_14                                                                                                                                                               
		{UTC_OFFSET_14,                  ItemOp(ItemOp::READ|ItemOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING },   
		#endif                                                                                                                                                                     
		#if RES_3_15                                                                                                                                                               
		{TIMEZONE_15,                    ItemOp(ItemOp::READ|ItemOp::WRITE), IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING },   
		#endif                                                                                                                                                                     
		{SUPPORTED_BINDING_AND_MODES_16, ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE,   IS_MANDATORY::MANDATORY, TYPE_ID::STRING },   
		#if RES_3_17                                                                                                                                                               
		{DEVICE_TYPE_17,                 ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING },   
		#endif                                                                                                                                                                     
		#if RES_3_18                                                                                                                                                               
		{HARDWARE_VERSION_18,            ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING },   
		#endif                                                                                                                                                                     
		#if RES_3_19                                                                                                                                                               
		{SOFTWARE_VERSION_19,            ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::STRING },   
		#endif                                                                                                                                                                     
		#if RES_3_20                                                                                                                                                               
		{BATTERY_STATUS_20,              ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },      
		#endif                                                                                                                                                                     
		#if RES_3_21                                                                                                                                                               
		{MEMORY_TOTAL_21,                ItemOp(ItemOp::READ),               IS_SINGLE::SINGLE,   IS_MANDATORY::OPTIONAL,  TYPE_ID::INT },      
		#endif                                                                                                                                                                     
		#if RES_3_22                                                                                                                                                               
		{EXTDEVINFO_22,                  ItemOp(ItemOp::READ),               IS_SINGLE::MULTIPLE, IS_MANDATORY::OPTIONAL,  TYPE_ID::OBJ_LINK }, 
		#endif                                                                                                                                                                     
	};
	setupResources(std::move(resources));
}

void Device::resourcesInit() {
	/* --------------- Code_cpp block 7 start --------------- */
	#if RES_3_0
	resource(MANUFACTURER_0)->set<STRING_T>(""); // TODO: Set appropriate value
	// resource(MANUFACTURER_0)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3_1
	resource(MODEL_NUMBER_1)->set<STRING_T>(""); // TODO: Set appropriate value
	// resource(MODEL_NUMBER_1)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3_2
	resource(SERIAL_NUMBER_2)->set<STRING_T>(""); // TODO: Set appropriate value
	// resource(SERIAL_NUMBER_2)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3_3
	resource(FIRMWARE_VERSION_3)->set<STRING_T>(""); // TODO: Set appropriate value
	// resource(FIRMWARE_VERSION_3)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	// resource(REBOOT_4)->set<EXECUTE_T>( /* TODO: Implement execute handler */ );
	// resource(REBOOT_4)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#if RES_3_5
	// resource(FACTORY_RESET_5)->set<EXECUTE_T>( /* TODO: Implement execute handler */ );
	// resource(FACTORY_RESET_5)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3_6
	resource(AVAILABLE_POWER_SOURCES_6)->set<INT_T>(0); // TODO: Set appropriate value
	// resource(AVAILABLE_POWER_SOURCES_6)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3_7
	resource(POWER_SOURCE_VOLTAGE_7)->set<INT_T>(0); // TODO: Set appropriate value
	// resource(POWER_SOURCE_VOLTAGE_7)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3_8
	resource(POWER_SOURCE_CURRENT_8)->set<INT_T>(0); // TODO: Set appropriate value
	// resource(POWER_SOURCE_CURRENT_8)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3_9
	resource(BATTERY_LEVEL_9)->set<INT_T>(0); // TODO: Set appropriate value
	// resource(BATTERY_LEVEL_9)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3_10
	resource(MEMORY_FREE_10)->set<INT_T>(0); // TODO: Set appropriate value
	// resource(MEMORY_FREE_10)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	resource(ERROR_CODE_11)->set<INT_T>(0); // TODO: Set appropriate value
	// resource(ERROR_CODE_11)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#if RES_3_12
	// resource(RESET_ERROR_CODE_12)->set<EXECUTE_T>( /* TODO: Implement execute handler */ );
	// resource(RESET_ERROR_CODE_12)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3_13
	resource(CURRENT_TIME_13)->set<TIME_T>(0); // TODO: Set appropriate value
	// resource(CURRENT_TIME_13)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3_14
	resource(UTC_OFFSET_14)->set<STRING_T>(""); // TODO: Set appropriate value
	// resource(UTC_OFFSET_14)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3_15
	resource(TIMEZONE_15)->set<STRING_T>(""); // TODO: Set appropriate value
	// resource(TIMEZONE_15)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	resource(SUPPORTED_BINDING_AND_MODES_16)->set<STRING_T>(""); // TODO: Set appropriate value
	// resource(SUPPORTED_BINDING_AND_MODES_16)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#if RES_3_17
	resource(DEVICE_TYPE_17)->set<STRING_T>(""); // TODO: Set appropriate value
	// resource(DEVICE_TYPE_17)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3_18
	resource(HARDWARE_VERSION_18)->set<STRING_T>(""); // TODO: Set appropriate value
	// resource(HARDWARE_VERSION_18)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3_19
	resource(SOFTWARE_VERSION_19)->set<STRING_T>(""); // TODO: Set appropriate value
	// resource(SOFTWARE_VERSION_19)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3_20
	resource(BATTERY_STATUS_20)->set<INT_T>(0); // TODO: Set appropriate value
	// resource(BATTERY_STATUS_20)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3_21
	resource(MEMORY_TOTAL_21)->set<INT_T>(0); // TODO: Set appropriate value
	// resource(MEMORY_TOTAL_21)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	#if RES_3_22
	resource(EXTDEVINFO_22)->set<OBJ_LINK_T>(0); // TODO: Set appropriate value
	// resource(EXTDEVINFO_22)->setDataVerifier( /* TODO: Add data verifier if needed */ );
	#endif
	/* --------------- Code_cpp block 7 end --------------- */
}

/* --------------- Code_cpp block 8 start --------------- */
/* --------------- Code_cpp block 8 end --------------- */

} /* namespace wpp */
