/*
 * Generated on: 2025-11-12 10:31:02
 * Created by: Sinai RnD
 */

#ifndef WPP_ONOFF_SWITCH_INFO_H
#define WPP_ONOFF_SWITCH_INFO_H

#include "ObjectInfo.h"

#ifdef OBJ_O_3342_ONOFF_SWITCH

namespace wpp {

inline const ObjectInfo ONOFF_SWITCH_OBJ_INFO = {
	/* Name */
	"OnOff switch",
	/* Object ID */
	OBJ_ID::ONOFF_SWITCH,
	/* URN */
	"urn:oma:lwm2m:ext:3342:1.1",
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

#endif /* OBJ_O_3342_ONOFF_SWITCH */
#endif // WPP_ONOFF_SWITCH_INFO_H
