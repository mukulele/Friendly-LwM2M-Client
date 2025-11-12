/*
 * Generated on: 2025-11-12 10:30:02
 * Created by: Sinai RnD
 */

#ifndef WPP_GENERIC_SENSOR_INFO_H
#define WPP_GENERIC_SENSOR_INFO_H

#include "ObjectInfo.h"

#ifdef OBJ_O_3300_GENERIC_SENSOR

namespace wpp {

inline const ObjectInfo GENERIC_SENSOR_OBJ_INFO = {
	/* Name */
	"Generic Sensor",
	/* Object ID */
	OBJ_ID::GENERIC_SENSOR,
	/* URN */
	"urn:oma:lwm2m:ext:3300:1.1",
	/* Object version */
	{1,1},
	/* Lwm2m version */
	{1,0},
	/* Is single */
	IS_SINGLE::MULTIPLE,
	/* Is Mandatory */
	IS_MANDATORY::OPTIONAL,
	/* Object supported operations */
	/* --------------- Info block 0 start --------------- */
	ItemOp(ItemOp::CREATE|
		   ItemOp::DELETE|
		   ItemOp::READ|
		   ItemOp::WRITE|
		   ItemOp::DISCOVER|
		   ItemOp::EXECUTE),
	/* --------------- Info block 0 end --------------- */
};

} /* namespace wpp */

#endif /* OBJ_O_3300_GENERIC_SENSOR */
#endif // WPP_GENERIC_SENSOR_INFO_H
