/*
 * Load
 * Generated on: 2025-11-12 12:55:43
 * Created by: Sinai RnD
 */

#ifndef WPP_O_3322_LOAD_H
#define WPP_O_3322_LOAD_H

#include "LoadConfig.h"
#include "LoadInfo.h"
#include "Instance.h"
#include "InstSubject.h"

/* --------------- Сode_h block 0 start --------------- */
/* --------------- Сode_h block 0 end --------------- */

namespace wpp {

class WppClient;
class Object;

/* --------------- Сode_h block 1 start --------------- */
/* --------------- Сode_h block 1 end --------------- */

class Load : public Instance {
public:
	enum ID: ID_T {
		SENSOR_VALUE_5700 = 5700,
		#if RES_3322_5701
		SENSOR_UNITS_5701 = 5701,
		#endif
		#if RES_3322_5601
		MIN_MEASURED_VALUE_5601 = 5601,
		#endif
		#if RES_3322_5602
		MAX_MEASURED_VALUE_5602 = 5602,
		#endif
		#if RES_3322_5603
		MIN_RANGE_VALUE_5603 = 5603,
		#endif
		#if RES_3322_5604
		MAX_RANGE_VALUE_5604 = 5604,
		#endif
		#if RES_3322_5605
		RESET_MIN_AND_MAX_MEASURED_VALUES_5605 = 5605,
		#endif
		#if RES_3322_5821
		CURRENT_CALIBRATION_5821 = 5821,
		#endif
		#if RES_3322_5750
		APPLICATION_TYPE_5750 = 5750,
		#endif
		#if RES_3322_5518
		TIMESTAMP_5518 = 5518,
		#endif
		#if RES_3322_6050
		FRACTIONAL_TIMESTAMP_6050 = 6050,
		#endif
		#if RES_3322_6042
		MEASUREMENT_QUALITY_INDICATOR_6042 = 6042,
		#endif
		#if RES_3322_6049
		MEASUREMENT_QUALITY_LEVEL_6049 = 6049,
		#endif
	};

	/* --------------- Code_h block 2 start --------------- */
	/* --------------- Code_h block 2 end --------------- */

public:
	Load(lwm2m_context_t &context, const OBJ_LINK_T &id);
	~Load();

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
	static Load * instance(WppClient &ctx, ID_T instId = ID_T_MAX_VAL);

	/**
	 * @brief Creates an instance of the object.
	 * @param ctx - WppClient context
	 * @param instanceID The instance ID.
	 * @return A pointer to the created Instance object or NULL.
	 */
	static Load * createInst(WppClient &ctx, ID_T instId = ID_T_MAX_VAL);

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

#endif /* WPP_O_3322_LOAD_H */
