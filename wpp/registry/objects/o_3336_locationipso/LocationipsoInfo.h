/*
 * Generated on: 2025-11-12 11:40:51
 * Created by: Sinai RnD
 */

#ifndef WPP_LOCATIONIPSO_INFO_H
#define WPP_LOCATIONIPSO_INFO_H

#include "ObjectInfo.h"

#ifdef OBJ_O_3336_LOCATIONIPSO

namespace wpp {

inline const ObjectInfo LOCATIONIPSO_OBJ_INFO = {
	/* Name */
	"LocationIpso",
	/* Object ID */
	OBJ_ID::LOCATIONIPSO,
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

#endif /* OBJ_O_3336_LOCATIONIPSO */
#endif // WPP_LOCATIONIPSO_INFO_H
