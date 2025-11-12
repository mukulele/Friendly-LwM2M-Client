/*
 * Device
 * Generated on: 2025-11-12 12:56:25
 * Created by: Sinai RnD
 */

#ifndef WPP_M_3_DEVICE_H
#define WPP_M_3_DEVICE_H

#include "DeviceConfig.h"
#include "DeviceInfo.h"
#include "Instance.h"
#include "InstSubject.h"

/* --------------- Сode_h block 0 start --------------- */
/* --------------- Сode_h block 0 end --------------- */

namespace wpp {

class WppClient;
class Object;

/* --------------- Сode_h block 1 start --------------- */
/* --------------- Сode_h block 1 end --------------- */

class Device : public Instance {
public:
	enum ID: ID_T {
		#if RES_3_0
		MANUFACTURER_0 = 0,
		#endif
		#if RES_3_1
		MODEL_NUMBER_1 = 1,
		#endif
		#if RES_3_2
		SERIAL_NUMBER_2 = 2,
		#endif
		#if RES_3_3
		FIRMWARE_VERSION_3 = 3,
		#endif
		REBOOT_4 = 4,
		#if RES_3_5
		FACTORY_RESET_5 = 5,
		#endif
		#if RES_3_6
		AVAILABLE_POWER_SOURCES_6 = 6,
		#endif
		#if RES_3_7
		POWER_SOURCE_VOLTAGE_7 = 7,
		#endif
		#if RES_3_8
		POWER_SOURCE_CURRENT_8 = 8,
		#endif
		#if RES_3_9
		BATTERY_LEVEL_9 = 9,
		#endif
		#if RES_3_10
		MEMORY_FREE_10 = 10,
		#endif
		ERROR_CODE_11 = 11,
		#if RES_3_12
		RESET_ERROR_CODE_12 = 12,
		#endif
		#if RES_3_13
		CURRENT_TIME_13 = 13,
		#endif
		#if RES_3_14
		UTC_OFFSET_14 = 14,
		#endif
		#if RES_3_15
		TIMEZONE_15 = 15,
		#endif
		SUPPORTED_BINDING_AND_MODES_16 = 16,
		#if RES_3_17
		DEVICE_TYPE_17 = 17,
		#endif
		#if RES_3_18
		HARDWARE_VERSION_18 = 18,
		#endif
		#if RES_3_19
		SOFTWARE_VERSION_19 = 19,
		#endif
		#if RES_3_20
		BATTERY_STATUS_20 = 20,
		#endif
		#if RES_3_21
		MEMORY_TOTAL_21 = 21,
		#endif
		#if RES_3_22
		EXTDEVINFO_22 = 22,
		#endif
	};

	/* --------------- Code_h block 2 start --------------- */
	/* --------------- Code_h block 2 end --------------- */

public:
	Device(lwm2m_context_t &context, const OBJ_LINK_T &id);
	~Device();

	/* --------------- Helpful methods --------------- */
	/**
	 * @brief Gets the Object reference.
	 * @param ctx - WppClient context.
	 * @return A reference to the Object.
	 */
	static Object & object(WppClient &ctx);

	/**
	 * @brief Gets an instance of the object.
	 * @param ctx - WppClient context.
	 * @param instId The instance ID. If not provided, the first available instance is returned.
	 * @return A pointer to the Instance object or NULL.
	 */
	static Device * instance(WppClient &ctx, ID_T instId = ID_T_MAX_VAL);

	/**
	 * @brief Creates an instance of the object.
	 * @param ctx - WppClient context
	 * @param instanceID The instance ID.
	 * @return A pointer to the created Instance object or NULL.
	 */
	static Device * createInst(WppClient &ctx, ID_T instId = ID_T_MAX_VAL);

	/**
	 * @brief Removes an instance of the object.
	 * @param ctx - WppClient context.
	 * @param instId The instance ID.
	 * @return True if the instance was successfully removed, false otherwise.
	 */
	static bool removeInst(WppClient &ctx, ID_T instId);

	/* --------------- Code_h block 3 start --------------- */
	/* --------------- Code_h block 3 end --------------- */

protected:
	/* --------------- Instance implementation part --------------- */
	/*
	 * Handles information about resource operation that made server
	 */
	void serverOperationNotifier(Instance *securityInst, ItemOp::TYPE type, const ResLink &resLink) override;
	/*
	 * Handles information about resource operation that made user
	 */
	void userOperationNotifier(ItemOp::TYPE type, const ResLink &resLink) override;

	/* --------------- Code_h block 4 start --------------- */
	/* --------------- Code_h block 4 end --------------- */

private:
	/* --------------- Class private methods --------------- */
	/*
	 * Creates resources without initializing.
	 */
	void resourcesCreate();
	/*
	 * Initialize resources with default values
	 * SINGLE resource always must have at least one instance.
	 */	
	void resourcesInit();
	
	/* --------------- Code_h block 5 start --------------- */
	/* --------------- Code_h block 5 end --------------- */

private:
	/* --------------- Class private properties --------------- */
	/* --------------- Code_h block 6 start --------------- */
	/* --------------- Code_h block 6 end --------------- */
};

} /* namespace wpp */

#endif /* WPP_M_3_DEVICE_H */
