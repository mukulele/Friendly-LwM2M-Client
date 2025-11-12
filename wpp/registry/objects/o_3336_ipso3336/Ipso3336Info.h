/*
 * Generated on: 2025-11-12 12:55:46
 * Created by: Sinai RnD
 */

#ifndef WPP_IPSO3336_INFO_H
#define WPP_IPSO3336_INFO_H

#include "ObjectInfo.h"

#ifdef OBJ_O_3336_IPSO3336

namespace wpp {

inline const ObjectInfo IPSO3336_OBJ_INFO = {
	/* Name */
	"IPSO3336",
	/* Object ID */
	OBJ_ID::IPSO3336,
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

#endif /* OBJ_O_3336_IPSO3336 */
#endif // WPP_IPSO3336_INFO_H
