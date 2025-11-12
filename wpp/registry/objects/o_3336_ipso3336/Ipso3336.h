/*
 * Ipso3336
 * Generated on: 2025-11-12 12:46:20
 * Created by: Sinai RnD
 */

#ifndef WPP_O_3336_IPSO3336_H
#define WPP_O_3336_IPSO3336_H

#include "Ipso3336Config.h"
#include "Ipso3336Info.h"
#include "Instance.h"
#include "InstSubject.h"

/* --------------- Сode_h block 0 start --------------- */
/* --------------- Сode_h block 0 end --------------- */

namespace wpp {

class WppClient;
class Object;

/* --------------- Сode_h block 1 start --------------- */
/* --------------- Сode_h block 1 end --------------- */

class Ipso3336 : public Instance {
public:
	enum ID: ID_T {
		NUMERIC_LATITUDE_6051 = 6051,
		NUMERIC_LONGITUDE_6052 = 6052,
		#if RES_3336_6053
		NUMERIC_UNCERTAINTY_6053 = 6053,
		#endif
		#if RES_3336_5705
		COMPASS_DIRECTION_5705 = 5705,
		#endif
		#if RES_3336_5517
		VELOCITY_5517 = 5517,
		#endif
		#if RES_3336_5518
		TIMESTAMP_5518 = 5518,
		#endif
		#if RES_3336_5750
		APPLICATION_TYPE_5750 = 5750,
		#endif
		#if RES_3336_6050
		FRACTIONAL_TIMESTAMP_6050 = 6050,
		#endif
		#if RES_3336_6042
		MEASUREMENT_QUALITY_INDICATOR_6042 = 6042,
		#endif
		#if RES_3336_6049
		MEASUREMENT_QUALITY_LEVEL_6049 = 6049,
		#endif
	};

	/* --------------- Code_h block 2 start --------------- */
	/* --------------- Code_h block 2 end --------------- */

public:
	Ipso3336(lwm2m_context_t &context, const OBJ_LINK_T &id);
	~Ipso3336();

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
	static Ipso3336 * instance(WppClient &ctx, ID_T instId = ID_T_MAX_VAL);

	/**
	 * @brief Creates an instance of the object.
	 * @param ctx - WppClient context
	 * @param instanceID The instance ID.
	 * @return A pointer to the created Instance object or NULL.
	 */
	static Ipso3336 * createInst(WppClient &ctx, ID_T instId = ID_T_MAX_VAL);

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

#endif /* WPP_O_3336_IPSO3336_H */
