// dllVI50.cpp : définit les fonctions exportées pour l'application DLL.

#include "stdafx.h"
#include <QHHeadersWin32.h>
#include <HD/hd.h>
#include <HDU/hduError.h>
#include <HDU/hduVector.h>

DeviceSpace* OmniSpace;
HHD ghHD;
HDSchedulerHandle gSchedulerCallback;
hduVector3Dd position, positionOnClickButton2, previousPosition;

hduVector3Dd forceActuelle;
hduVector3Dd forceIni;
bool button1;
bool button2;

bool activatedHD = false;
bool camera = false;
bool hand = false;
bool firstActivatedButton2 = true;

HDdouble gVibrationAmplitude;

// fonction "routine" effecture à chaque pas d'itération du bras
extern "C" HDCallbackCode HDCALLBACK compute(void *pUserData){
	hdBeginFrame(ghHD);

	// on récupère la position
	previousPosition = position;
	// on mémorise la position précédente
	hdGetDoublev(HD_CURRENT_POSITION, position);
	// on mémorise la nouvelle
	
	hdSetDoublev(HD_CURRENT_FORCE, forceActuelle);
	// on fixe la force que l'on souhaite au bras

	/* onrécupère l'état des boutons  et on change si besoin de mode*/
	HDint button = 0;
	hdGetIntegerv(HD_CURRENT_BUTTONS,&button);
	
	button1 = ((button & HD_DEVICE_BUTTON_1)==HD_DEVICE_BUTTON_1);
	button2 = ((button & HD_DEVICE_BUTTON_2)==HD_DEVICE_BUTTON_2);

	
	if(button1){
		camera = false;
		hand = true;
		firstActivatedButton2 = true;
	}
	else if(button2){
		camera = true;
		hand = false;
		if(firstActivatedButton2){
			firstActivatedButton2 = false;
			positionOnClickButton2 = position;
		}
	}else{
		camera = false;
		hand = false;	
		firstActivatedButton2 = true;
	}

	hdEndFrame(ghHD);

	return HD_CALLBACK_CONTINUE;
}
// fonction d'initialisation du bras
extern "C" __declspec(dllexport) int init()  
{  
	if(!activatedHD){
		HDErrorInfo error;
		gSchedulerCallback = HD_INVALID_HANDLE;
		ghHD = HD_INVALID_HANDLE;


		ghHD = hdInitDevice(HD_DEFAULT_DEVICE);
		
		if (HD_DEVICE_ERROR(error = hdGetError()))
		{
			return -1;
		}

		hdEnable(HD_FORCE_OUTPUT);
		hdEnable(HD_MAX_FORCE_CLAMPING);

		forceIni[0]= 0;
		forceIni[1]= 0;
		forceIni[2]= 0;

		forceActuelle = forceIni;
		gSchedulerCallback = hdScheduleAsynchronous(compute, (void*)0,HD_MAX_SCHEDULER_PRIORITY);

		hdStartScheduler();

		
		if (HD_DEVICE_ERROR(error = hdGetError()))
			return -2;
		
		activatedHD = true;

		
		return 0;
	}
	return -3;
} 

// fonction de déinitialisation du bras
extern "C" __declspec(dllexport) void exitHD(){
	if(activatedHD){

		activatedHD = false;

		hdStopScheduler();
		hdUnschedule(gSchedulerCallback);

		hdDisableDevice(ghHD);
	}
}

// position absolue de la camera
extern "C" __declspec(dllexport) double getX()  
{  
	return (double) position[0];
} 

// position absolue de la camera
extern "C" __declspec(dllexport) double getY()  
{  
	return (double) position[1];
} 

// position absolue de la camera
extern "C" __declspec(dllexport) double getZ()  
{  
	return (double) position[2];
} 

// retourne la position relative de la camera si on est dans ce mode
extern "C" __declspec(dllexport) double getRelativeXForCamera()  
{  
	if(camera)
		return (double) position[0] - positionOnClickButton2[0];
	return 0;
} 

// retourne la position relative de la camera si on est dans ce mode
extern "C" __declspec(dllexport) double getRelativeYForCamera()  
{  
	if(camera)
		return (double) position[1] - positionOnClickButton2[1];
	return 0;
} 

// retourne la position relative de la camera si on est dans ce mode
extern "C" __declspec(dllexport) double getRelativeZForCamera()  
{  
	if(camera)
		return (double) position[2] - positionOnClickButton2[2];
	return 0;
} 
/*
extern "C" __declspec(dllexport) double getRelativeXForHand()  
{  
	return (double) position[0] - previousPosition[0];
} 

extern "C" __declspec(dllexport) double getRelativeYForHand()  
{  
	return (double) position[1] - previousPosition[1];
} 

extern "C" __declspec(dllexport) double getRelativeZForHand()  
{  
	return (double) position[2] - previousPosition[2];
} 
*/

// appliquer les forces sur le bras en X, Y et Z.
extern "C" __declspec(dllexport) void setForceOnAxisX(double x, double y, double z)  
{  
	forceActuelle[0] = x;
	forceActuelle[1] = y;
	forceActuelle[2] = z;
} 

// etat d'activation du bouton 1
extern "C" __declspec(dllexport) bool isButton1Activate()  
{  
	return  button1;
} 

// etat d'activation du bouton 2
extern "C" __declspec(dllexport) bool isButton2Activate()  
{  
	return button2;
}

// réinitialisation de la force
extern "C" __declspec(dllexport) void reInitForce(){
	forceActuelle = forceIni;
}

// retourne si le booleen si le bras est actif vrai, sinon faux
extern "C" __declspec(dllexport) bool HDIsACtivate(){
	return activatedHD;
}



