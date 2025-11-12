/*
 * Generated on: 2025-11-12 12:56:09
 * Created by: Sinai RnD
 */

#ifndef WPP_DIGITAL_OUTPUT_INFO_H
#define WPP_DIGITAL_OUTPUT_INFO_H

#include "ObjectInfo.h"

#ifdef OBJ_O_3201_DIGITAL_OUTPUT

namespace wpp {

inline const ObjectInfo DIGITAL_OUTPUT_OBJ_INFO = {
	/* Name */
	"Digital Output",
	/* Object ID */
	OBJ_ID::DIGITAL_OUTPUT,
	/* URN */
	"urn:oma:lwm2m:ext:3201:1.1",
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

#endif /* OBJ_O_3201_DIGITAL_OUTPUT */
#endif // WPP_DIGITAL_OUTPUT_INFO_H
