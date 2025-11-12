/*
 * Generated on: 2025-11-12 10:30:05
 * Created by: Sinai RnD
 */

#ifndef WPP_LOAD_INFO_H
#define WPP_LOAD_INFO_H

#include "ObjectInfo.h"

#ifdef OBJ_O_3322_LOAD

namespace wpp {

inline const ObjectInfo LOAD_OBJ_INFO = {
	/* Name */
	"Load",
	/* Object ID */
	OBJ_ID::LOAD,
	/* URN */
	"urn:oma:lwm2m:ext:3322:1.1",
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

#endif /* OBJ_O_3322_LOAD */
#endif // WPP_LOAD_INFO_H
