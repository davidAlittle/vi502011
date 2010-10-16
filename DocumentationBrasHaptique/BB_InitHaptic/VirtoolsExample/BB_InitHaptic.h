#ifndef BB_InitHaptic_H
#define BB_InitHaptic_H "$Id$"

//TODO: Change the following macros

#define BB_NAME				"BB_Haptic Controller"		// Name of the BB
#define BB_VERSION			0x1							// Version pf the BB
#define BB_COMMENT			"BB for Phantom Omni" // Description of the BB
#define BB_CATEGORY			"Haptic Device"				// Category of the BB
#define BB_UID1				0x450f2ce9					// First Id for the BB
#define BB_UID2				0x4441170					// Second Id for the BB

#define AUTHOR_NAME			"Lionel Bennes Alban Gac"	// Name of the author
#define AUTHOR_UID1			0x37412090					// First Id for the author
#define AUTHOR_UID2			0x3b8012a6					// Second Id for the author

CKObjectDeclaration	*FillBehaviorBB_InitHapticDecl();
CKERROR CreateBB_InitHapticPrototype(CKBehaviorPrototype** pproto);
int BB_InitHaptic(const CKBehaviorContext& behcontext);

#endif