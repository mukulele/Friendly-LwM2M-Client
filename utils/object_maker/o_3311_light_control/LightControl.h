/*
 * LightControl
 * Generated on: 2025-11-12 12:56:15
 * Created by: Sinai RnD
 */

#ifndef WPP_O_3311_LIGHT_CONTROL_H
#define WPP_O_3311_LIGHT_CONTROL_H

#include "LightControlConfig.h"
#include "LightControlInfo.h"
#include "Instance.h"
#include "InstSubject.h"

/* --------------- Сode_h block 0 start --------------- */
/* --------------- Сode_h block 0 end --------------- */

namespace wpp {

class WppClient;
class Object;

/* --------------- Сode_h block 1 start --------------- */
/* --------------- Сode_h block 1 end --------------- */

class LightControl : public Instance {
public:
	enum ID: ID_T {
		ON_OFF_5850 = 5850,
		#if RES_3311_5851
		DIMMER_5851 = 5851,
		#endif
		#if RES_3311_5852
		ON_TIME_5852 = 5852,
		#endif
		#if RES_3311_5805
		CUMULATIVE_ACTIVE_POWER_5805 = 5805,
		#endif
		#if RES_3311_5820
		POWER_FACTOR_5820 = 5820,
		#endif
		#if RES_3311_5706
		COLOUR_5706 = 5706,
		#endif
		#if RES_3311_5701
		SENSOR_UNITS_5701 = 5701,
		#endif
		#if RES_3311_5750
		APPLICATION_TYPE_5750 = 5750,
		#endif
	};

	/* --------------- Code_h block 2 start --------------- */
	/* --------------- Code_h block 2 end --------------- */

public:
	LightControl(lwm2m_context_t &context, const OBJ_LINK_T &id);
	~LightControl();

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
	static LightControl * instance(WppClient &ctx, ID_T instId = ID_T_MAX_VAL);

	/**
	 * @brief Creates an instance of the object.
	 * @param ctx - WppClient context
	 * @param instanceID The instance ID.
	 * @return A pointer to the created Instance object or NULL.
	 */
	static LightControl * createInst(WppClient &ctx, ID_T instId = ID_T_MAX_VAL);

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

#endif /* WPP_O_3311_LIGHT_CONTROL_H */
