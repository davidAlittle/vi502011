// Test_InitHaptic.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <HD/hd.h>
#include <HDU/hduError.h>
#include <HDU/hduVector.h>

typedef struct{
	union{
		struct{
			float x,y,z;
		};
		float v[3];
	};
}vec3;

typedef enum{
	MOVE,
	CAM,
	ARM
}Mode;

typedef struct{
	hduVector3Dd position;
}HapticDisplaySate;

// Global variables
	// Haptic Arm
	HHD ghHD;
	HDSchedulerHandle gSchedulerCallback;
	static HDint gVibrationFreq = 100; /* Hz */
	static HDdouble gVibrationAmplitude = 0; /* N */

	// Input variables
	bool g_doExit = false;
	vec3 g_position_in;
	vec3 g_force;
	Mode g_selecMode = MOVE;
	float g_speed = 5;

	// Output variables
	bool g_button1; //BLACK
	bool g_button2; //WHITE
	bool g_button3; //INK
	vec3 g_position_out;

void ExitHandler(){

	hdStopScheduler();
	hdUnschedule(gSchedulerCallback);

}

hduVector3Dd shakeBaby(){

		static const hduVector3Dd direction(0, 1, 0);
		hduVector3Dd force; //current force applied to the arm 
		HDdouble instRate;
		static HDdouble timer = 0;

		gVibrationFreq = (HDint) g_speed * 10;
		gVibrationFreq = gVibrationFreq < 0 ? 0 : gVibrationFreq; 
		gVibrationFreq = gVibrationFreq > 500 ? 500 : gVibrationFreq; 

		/* Use the reciprocal of the instantaneous rate as a timer. */
		hdGetDoublev(HD_INSTANTANEOUS_UPDATE_RATE, &instRate);
		timer += 1.0 / instRate;

		/* Apply a sinusoidal force in the direction of motion. */
		hduVecScale(force, direction, gVibrationAmplitude * sin(timer * gVibrationFreq));
	    
		return force;
}

HDCallbackCode HDCALLBACK touchScene(void *pUserData){
	static const HDdouble stiffness = 0.05;

	hduVector3Dd position;
	hduVector3Dd initialPosition(0, 0, 0);
	hduVector3Dd force((double) g_force.x, (double) g_force.y, (double) g_force.z);
	int button;

	// Get Haptic Arm State
	hdGetIntegerv(HD_CURRENT_BUTTONS,&button);

	g_button1 = (button & HD_DEVICE_BUTTON_1);
	g_button2 = (button & HD_DEVICE_BUTTON_2);
	g_button3 = (button & HD_DEVICE_BUTTON_3);

	printf("\t%i\t%i %i %i\r",button,g_button1,g_button2,g_button3);

	if(g_button2) g_doExit = true;

	hdBeginFrame(ghHD);

	hdGetDoublev(HD_CURRENT_POSITION, position);

	switch(g_selecMode){
		case MOVE:
			{
				hduVector3Dd tempForce = shakeBaby();

				hduVecSubtract(force, initialPosition, position);
				hduVecScaleInPlace(force, stiffness);

				force += tempForce;

				hdSetDoublev(HD_CURRENT_FORCE, force);

				g_position_out.v[0] = (float)position[0];
				g_position_out.v[1] = 0.f;
				g_position_out.v[2] = (float)position[2];

				break;
			}
		case CAM:
			{ 
				hduVecSubtract(force, initialPosition, position);
				hduVecScaleInPlace(force, stiffness);
				hdSetDoublev(HD_CURRENT_FORCE, force);

				g_position_out.v[0] = (float)position[0];
				g_position_out.v[1] = (float)position[1];
				g_position_out.v[2] = 0.f;

				break;
			}
		case ARM :
			{
				g_position_out.v[0] = (float)position[0];
				g_position_out.v[1] = (float)position[1];
				g_position_out.v[2] = (float)position[2];
				
				hduVecScaleInPlace(force, stiffness);
				hdSetDoublev(HD_CURRENT_FORCE, force);
				break;
			}
	}

	hdEndFrame(ghHD);

	return HD_CALLBACK_CONTINUE;
}


int _tmain(int argc, _TCHAR* argv[])
{
	ghHD = HD_INVALID_HANDLE;
	gSchedulerCallback = HD_INVALID_HANDLE;
	bool returnValue = true;
	int devideID = 0;

	g_doExit = false;

	//Initialize Haptic device

	HDErrorInfo error;
    
    // Initialize the device.  This needs to be called before any actions on the
    // device.
    ghHD = hdInitDevice(HD_DEFAULT_DEVICE);
    if (HD_DEVICE_ERROR(error = hdGetError()))
		returnValue = false;
    
    hdEnable(HD_FORCE_OUTPUT);
    hdEnable(HD_MAX_FORCE_CLAMPING);

	// Initialize amplitude for move vribration
	hdGetDoublev(HD_NOMINAL_MAX_CONTINUOUS_FORCE, &gVibrationAmplitude);
	gVibrationAmplitude *= 0.75;
    
	gSchedulerCallback = hdScheduleAsynchronous(touchScene, 0, 
		HD_MAX_SCHEDULER_PRIORITY);

    hdStartScheduler();
    if (HD_DEVICE_ERROR(error = hdGetError()))
		returnValue = false;

	atexit(ExitHandler);

	while(!g_doExit){
		//printf("%i %i %i\n",g_button1,g_button2,g_button3);
	}

	return 0;
}

