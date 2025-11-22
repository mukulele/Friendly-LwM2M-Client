/*
 * Generated on: 2025-11-21 15:31:51
 * Created by: Sinai RnD
 */

#ifndef WPP_LOCATION_INFO_H
#define WPP_LOCATION_INFO_H

#include "ObjectInfo.h"

#ifdef OBJ_O_3336_LOCATION

namespace wpp {

inline const ObjectInfo LOCATION_OBJ_INFO = {
	/* Name */
	"Location",
	/* Object ID */
	OBJ_ID::LOCATION,
	/* URN */
	"urn:oma:lwm2m:ext:3336:2.0",
	/* Object version */
	{2,0},
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

#endif /* OBJ_O_3336_LOCATION */
#endif // WPP_LOCATION_INFO_H
