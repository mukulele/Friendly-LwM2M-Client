/*
 * On_offSwitch
 * Generated on: 2025-11-21 15:34:43
 * Created by: Sinai RnD
 */

#ifndef WPP_O_3342_ON_OFF_SWITCH_H
#define WPP_O_3342_ON_OFF_SWITCH_H

#include "On_offSwitchConfig.h"
#include "On_offSwitchInfo.h"
#include "Instance.h"
#include "InstSubject.h"

/* --------------- Сode_h block 0 start --------------- */
/* --------------- Сode_h block 0 end --------------- */

namespace wpp {

class WppClient;
class Object;

/* --------------- Сode_h block 1 start --------------- */
/* --------------- Сode_h block 1 end --------------- */

class On_offSwitch : public Instance {
public:
	enum ID: ID_T {
		DIGITAL_INPUT_STATE_5500 = 5500,
		#if RES_3342_5501
		DIGITAL_INPUT_COUNTER_5501 = 5501,
		#endif
		#if RES_3342_5852
		ON_TIME_5852 = 5852,
		#endif
		#if RES_3342_5854
		OFF_TIME_5854 = 5854,
		#endif
		#if RES_3342_5750
		APPLICATION_TYPE_5750 = 5750,
		#endif
		#if RES_3342_5518
		TIMESTAMP_5518 = 5518,
		#endif
		#if RES_3342_6050
		FRACTIONAL_TIMESTAMP_6050 = 6050,
		#endif
	};

	/* --------------- Code_h block 2 start --------------- */
	/* --------------- Code_h block 2 end --------------- */

public:
	On_offSwitch(lwm2m_context_t &context, const OBJ_LINK_T &id);
	~On_offSwitch();

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
	static On_offSwitch * instance(WppClient &ctx, ID_T instId = ID_T_MAX_VAL);

	/**
	 * @brief Creates an instance of the object.
	 * @param ctx - WppClient context
	 * @param instanceID The instance ID.
	 * @return A pointer to the created Instance object or NULL.
	 */
	static On_offSwitch * createInst(WppClient &ctx, ID_T instId = ID_T_MAX_VAL);

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

#endif /* WPP_O_3342_ON_OFF_SWITCH_H */
