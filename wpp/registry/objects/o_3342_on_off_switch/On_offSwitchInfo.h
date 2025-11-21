/*
 * Generated on: 2025-11-21 15:34:43
 * Created by: Sinai RnD
 */

#ifndef WPP_ON_OFF_SWITCH_INFO_H
#define WPP_ON_OFF_SWITCH_INFO_H

#include "ObjectInfo.h"

#ifdef OBJ_O_3342_ON_OFF_SWITCH

namespace wpp {

inline const ObjectInfo ON_OFF_SWITCH_OBJ_INFO = {
	/* Name */
	"on_off switch",
	/* Object ID */
	OBJ_ID::ON_OFF_SWITCH,
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

#endif /* OBJ_O_3342_ON_OFF_SWITCH */
#endif // WPP_ON_OFF_SWITCH_INFO_H
