/*
 * Generated on: 2025-11-21 15:22:27
 * Created by: Sinai RnD
 */

#ifndef WPP_LIGHT_CONTROL_INFO_H
#define WPP_LIGHT_CONTROL_INFO_H

#include "ObjectInfo.h"

#ifdef OBJ_O_3311_LIGHT_CONTROL

namespace wpp {

inline const ObjectInfo LIGHT_CONTROL_OBJ_INFO = {
	/* Name */
	"Light Control",
	/* Object ID */
	OBJ_ID::LIGHT_CONTROL,
	/* URN */
	"urn:oma:lwm2m:ext:3311",
	/* Object version */
	{1,0},
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

#endif /* OBJ_O_3311_LIGHT_CONTROL */
#endif // WPP_LIGHT_CONTROL_INFO_H
