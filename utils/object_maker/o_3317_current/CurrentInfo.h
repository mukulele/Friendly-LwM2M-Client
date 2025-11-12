/*
 * Generated on: 2025-11-12 12:56:17
 * Created by: Sinai RnD
 */

#ifndef WPP_CURRENT_INFO_H
#define WPP_CURRENT_INFO_H

#include "ObjectInfo.h"

#ifdef OBJ_O_3317_CURRENT

namespace wpp {

inline const ObjectInfo CURRENT_OBJ_INFO = {
	/* Name */
	"Current",
	/* Object ID */
	OBJ_ID::CURRENT,
	/* URN */
	"urn:oma:lwm2m:ext:3317:1.1",
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

#endif /* OBJ_O_3317_CURRENT */
#endif // WPP_CURRENT_INFO_H
