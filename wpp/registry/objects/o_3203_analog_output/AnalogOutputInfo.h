/*
 * Generated on: 2025-11-21 15:22:07
 * Created by: Sinai RnD
 */

#ifndef WPP_ANALOG_OUTPUT_INFO_H
#define WPP_ANALOG_OUTPUT_INFO_H

#include "ObjectInfo.h"

#ifdef OBJ_O_3203_ANALOG_OUTPUT

namespace wpp {

inline const ObjectInfo ANALOG_OUTPUT_OBJ_INFO = {
	/* Name */
	"Analog Output",
	/* Object ID */
	OBJ_ID::ANALOG_OUTPUT,
	/* URN */
	"urn:oma:lwm2m:ext:3203:1.1",
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

#endif /* OBJ_O_3203_ANALOG_OUTPUT */
#endif // WPP_ANALOG_OUTPUT_INFO_H
