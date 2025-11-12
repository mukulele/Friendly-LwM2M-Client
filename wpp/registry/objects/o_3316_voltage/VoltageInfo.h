/*
 * Generated on: 2025-11-12 10:30:04
 * Created by: Sinai RnD
 */

#ifndef WPP_VOLTAGE_INFO_H
#define WPP_VOLTAGE_INFO_H

#include "ObjectInfo.h"

#ifdef OBJ_O_3316_VOLTAGE

namespace wpp {

inline const ObjectInfo VOLTAGE_OBJ_INFO = {
	/* Name */
	"Voltage",
	/* Object ID */
	OBJ_ID::VOLTAGE,
	/* URN */
	"urn:oma:lwm2m:ext:3316:1.1",
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

#endif /* OBJ_O_3316_VOLTAGE */
#endif // WPP_VOLTAGE_INFO_H
