/*
 * Generated on: 2025-11-21 15:31:37
 * Created by: Sinai RnD
 */

#ifndef WPP_POWER_INFO_H
#define WPP_POWER_INFO_H

#include "ObjectInfo.h"

#ifdef OBJ_O_3328_POWER

namespace wpp {

inline const ObjectInfo POWER_OBJ_INFO = {
	/* Name */
	"Power",
	/* Object ID */
	OBJ_ID::POWER,
	/* URN */
	"urn:oma:lwm2m:ext:3328:1.1",
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

#endif /* OBJ_O_3328_POWER */
#endif // WPP_POWER_INFO_H
