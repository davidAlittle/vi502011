#ifndef ManagerExample_H
#define ManagerExample_H "$Id:$"

#include "CKAll.h"

#include <HD/hd.h>
#include <HDU/hduError.h>
#include <HDU/hduVector.h>

#include "CKBaseManager.h"

//TODO: Generate unique identifiers
#define MANAGER_NAME	"ManagerHaptic"			// Name of the manager
#define MANAGER_UID1	0x48a70386					// First Id of the manager
#define MANAGER_UID2	0x53e632db					// Second Id of the manager


typedef enum{
	MOVE,
	CAM,
	ARM
}Mode;

typedef struct{
	hduVector3Dd position;
}HapticDisplaySate;

typedef struct Haptic_Param{
	// Haptic Arm
	HHD ghHD;
	HDSchedulerHandle gSchedulerCallback;
	HDint gVibrationFreq; /* Hz */
	HDdouble gVibrationAmplitude; /* N */

	// Input variables
	bool g_doExit;
	VxVector g_position_in;
	VxVector g_force;
	Mode g_selecMode;
	float g_speed;

	// Output variables
	bool g_button1; //BLACK
	bool g_button2; //WHITE
	bool g_button3; //INK?
	VxVector g_position_out;

}BB_Haptic_Parameters;


class ManagerExample : public CKBaseManager 
{
public :
	ManagerExample(CKContext* Context);
	virtual ~ManagerExample();

	CKERROR OnCKInit(); // Invoked by Virtools when manager may be initialized
	CKERROR OnCKEnd();

	const bool& getg_button1() ;
	const bool& getg_button2() ;
	const bool& getg_button3() ;
	const VxVector& getg_position_out() ;

	void ExitArmUse();
	bool ArmUse;
	void setArmUse(bool v){ArmUse=v;}
	bool IsArmUse(){return ArmUse;}


	
	// contexte pour écrire dans la console Virtools
	CKContext * context;

public:
	void ExitHandler();
	

	BB_Haptic_Parameters hapticParameters;

	virtual CKDWORD	GetValidFunctionsMask();

};

HDCallbackCode HDCALLBACK touchScene(void *pUserData);
hduVector3Dd shakeBaby(ManagerExample * bm);

#endif

